#ifndef BKSPROXYSERVER_H
#define BKSPROXYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSettings>
#include <QDataStream>
#include <QIODevice>
#include <devices/bks_device.h>
#include <network_transport/abstract_network_transport.h>
#include <network_transport/abstract_network_transport_factory.h>
#include <network_transport/network_transport_locker.h>
#include <QCoreApplication>

const quint8 HEADER = 0x55;

struct BKS_CMD {
    static const quint8 FX1 = 0x01;
    static const quint8 FX2 = 0x02;
    static const quint8 FC1 = 0x03;
    static const quint8 FC2 = 0x04;
    static const quint8 BAUD = 0x05;
    static const quint8 STATUS = 0x06;
};

class BksProxyServer : public QObject {
    Q_OBJECT
    QString configPath;
    QString section;
    QTcpServer *server;
    QHostAddress ServerAddress;
    QHostAddress ListenIp;
    quint16 port;
    QString name;
    BksDevice *device;
    SerialCircularRequester *requester;
    AbstractNetworkTransport *transport;
    NetworkTransportLocker *locker;

    QByteArray buffer;
    QTcpSocket *currentClient;
    quint64 transportId;
public:
    explicit BksProxyServer(QString configPath, QString section, QObject *parent = nullptr);
    void sendReply(QByteArray reply);
    void setBksDevice(BksDevice *implement);

signals:
    void translateData(QByteArray);
private:
    void loadConfig();
    void ParseData(QByteArray data);
    void ProcessBksCmd(QByteArray packet);
    void makeResponse(quint8 cmdId);
public slots:
    void onNewConnection();
    void onDataReady();
};

#endif // BKSPROXYSERVER_H
