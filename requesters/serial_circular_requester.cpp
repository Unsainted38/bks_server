#include "serial_circular_requester.h"
#ifdef MYABSTRACTCONNECT_H
SerialCircularRequester::SerialCircularRequester(MyAbstractConnect *transport, NetworkTransportLocker *locker, int pollIntervalMs, QObject *parent)
    : QObject(parent),
      m_connect(transport),
      m_locker(locker),
      timer(new QTimer(this)) {
    timer->setInterval(pollIntervalMs);
    connect(timer, &QTimer::timeout, this, &SerialCircularRequester::processNext);
    connect(m_connect, &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::translateData);

}
#else
SerialCircularRequester::SerialCircularRequester(AbstractNetworkTransport *transport, NetworkTransportLocker *locker, int pollIntervalMs, QObject *parent)
    : QObject(parent),
      m_transport(transport),
      m_locker(locker),
      timer(new QTimer(this)) {
    timer->setInterval(pollIntervalMs);
    connect(timer, SIGNAL(timeout()), this, SLOT(processNext()));
    connect(m_transport, SIGNAL(translateData(QByteArray)), this, SIGNAL(translateData(QByteArray)));
}
#endif

void SerialCircularRequester::addCommand(AbstractCommand *cmd) {
    if(m_readCommands.contains(cmd)) {
        return;
    }

    m_readCommands.append(cmd);
}

void SerialCircularRequester::addExtraCommand(AbstractCommand *cmd) {
    m_writeCommands.enqueue(cmd);
}

void SerialCircularRequester::removeCommands() {
    m_readCommands.clear();
    //m_writeCommands.clear();
}

void SerialCircularRequester::startRequest() {
    timer->start();
}

void SerialCircularRequester::processNext() {
    if(m_locker->isLocked()) {
        return;
    }

    AbstractCommand *cmd = nullptr;

    if(!m_writeCommands.isEmpty()) {
        cmd = m_writeCommands.dequeue();

        if(cmd) {
            m_locker->lock();
#ifdef MYABSTRACTCONNECT_H
            m_connect->writeData(cmd->makeWriteCommand());
            connect(m_connect, &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::unlock);
#else
            m_transport->write(cmd->makeWriteCommand());
            connect(m_transport, SIGNAL(translateData(QByteArray)), this, SLOT(unlock()));
#endif

        }
    } else if(!m_readCommands.isEmpty()) {
        cmd = m_readCommands[m_readIndex];
        m_readIndex = (m_readIndex + 1) % m_readCommands.size();

        if(cmd) {
            m_locker->lock();
#ifdef MYABSTRACTCONNECT_H
            m_connect->writeData(cmd->makeReadCommand());
            connect(m_connect,  &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::unlock);
#else
            m_transport->write(cmd->makeReadCommand());
            connect(m_transport, SIGNAL(translateData(QByteArray)), this, SLOT(unlock()));
#endif

        }
    }

}

void SerialCircularRequester::unlock() {
    m_locker->unlock();
#ifdef MYABSTRACTCONNECT_H
    disconnect(m_connect, &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::unlock);
#else
    disconnect(m_transport, SIGNAL(translateData(QByteArray)), this, SLOT(unlock()));
#endif
}
