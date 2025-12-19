#include "bks_proxy_server.h"

BksProxyServer::BksProxyServer(QString configPath, QString section, QObject *parent)
    : QObject(parent),
      configPath(configPath),
      section(section) {
    loadConfig();
    transport = AbstractNetworkTransportFactory::getInstance(
                    QCoreApplication::applicationDirPath() +
                    "/transport/config.ini", "BksTransport");
    locker = new NetworkTransportLocker(1000, 40);
    server = new QTcpServer(this);
    requester = new SerialCircularRequester(transport, locker);
    device = new BksDevice(requester, QCoreApplication::applicationDirPath() + "/device/config.ini", "BksDevice");

    if(server->listen(QHostAddress::Any, port)) {
        qDebug() << name << "started on ip" << ServerAddress.toString() << ":" << port;
    } else {
        qDebug() << name << "start failed on ip" << ServerAddress.toString() << ":" << port;
    }

    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void BksProxyServer::sendReply(QByteArray reply) {
    QString clientIp = currentClient->peerAddress().toString();

    if(currentClient->peerAddress().protocol() == QAbstractSocket::IPv6Protocol &&
            clientIp.startsWith("::ffff:")) {
        clientIp = clientIp.mid(7); // Deleting "::ffff:"
    }

    currentClient->write(reply);
}

void BksProxyServer::setBksDevice(BksDevice *implement) {
    device = implement;
}

void BksProxyServer::loadConfig() {
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(section);
    ServerAddress = QHostAddress(settings.value("HostIp", QHostAddress::LocalHost).toString());
    ListenIp = QHostAddress(settings.value("ListenIp", QHostAddress::Any).toString());
    port = settings.value("Port", 6666).toUInt();
    name = settings.value("Name", "tcp_proxy").toString();
    settings.endGroup();
}

void BksProxyServer::ParseData(QByteArray data) {
    buffer.append(data);
    int headerIndex = buffer.indexOf(HEADER);

    if(headerIndex < 0) {
        return;
    }

    buffer.remove(0, headerIndex);
    headerIndex = 0;

    if(buffer.size() < 3) {
        return;
    }

    quint8 packetLength = buffer[2];

    if(buffer.size() < packetLength + 3) {
        return;
    }

    QByteArray packet = buffer.mid(headerIndex, packetLength + 3);
    buffer.remove(buffer.indexOf(packet), packet.size());
    ProcessBksCmd(packet);
}

void BksProxyServer::ProcessBksCmd(QByteArray packet) {
    QDataStream in(&packet, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    quint8 header;
    quint8 length;
    quint8 cmdId;
    quint8 value;
    in >> header
       >> cmdId
       >> length
       >> value;





    switch(cmdId) {
        case BKS_CMD::FX1:
            device->setFX1(value);
            break;

        case BKS_CMD::FX2:
            device->setFX2(value);
            break;

        case BKS_CMD::FC1:
            device->setFC1(value);
            break;

        case BKS_CMD::FC2:
            device->setFC2(value);
            break;

        case BKS_CMD::BAUD:
            device->setBaud(value);
            break;
    }

    makeResponse(cmdId);
}

void BksProxyServer::makeResponse(quint8 cmdId) {
    QByteArray reply;
    QDataStream out(&reply, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out << HEADER
        << cmdId;

    switch(cmdId) {
        case BKS_CMD::FC1:
            out << (quint8)1;
            out << device->getFC1();
            break;

        case BKS_CMD::FC2:
            out << (quint8)1;
            out << device->getFC2();
            break;

        case BKS_CMD::FX1:
            out << (quint8)1;
            out << device->getFX1();
            break;

        case BKS_CMD::FX2:
            out << (quint8)1;
            out << device->getFX2();
            break;

        case BKS_CMD::STATUS:
            out << (quint8)4;
            out << device->getFC1()
                << device->getFC2()
                << device->getFX1()
                << device->getFX2();
            break;

        default:
            return;
    }

    sendReply(reply);
}

void BksProxyServer::onNewConnection() {
    QTcpSocket *clientSocket = server->nextPendingConnection();
    QString clientIp = clientSocket->peerAddress().toString();

    // Преобразование IPv4-mapped IPv6 в обычный IPv4
    if(clientSocket->peerAddress().protocol() == QAbstractSocket::IPv6Protocol &&
            clientIp.startsWith("::ffff:")) {
        clientIp = clientIp.mid(7); // Deleting "::ffff:"
    }

    qDebug() << "New client connected from IP -> " + clientIp;

    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onDataReady()));
    connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
}

void BksProxyServer::onDataReady() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());

    if(clientSocket) {
        currentClient = clientSocket;
        ParseData(currentClient->readAll());
    }
}
