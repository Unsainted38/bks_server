#ifndef TCPTRANSPORT_H
#define TCPTRANSPORT_H

#include "abstract_network_transport.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QQueue>
#include <QTimer>
#include <QDateTime>

class TcpTransport : public AbstractNetworkTransport {
    QTcpSocket *socket;
    QHostAddress hostAddress;
    quint16 port = 7777;
    QHostAddress listenIp;
    QString name = "tcp_client";
    QMutex mutex;
    QQueue<QByteArray> queue;
    QTimer *heartbeatTimer;
    QTimer *reconnectTimer;
    bool connected = false;
    QDateTime lastActivity;
    QByteArray heartbeatPacket;

public:
    explicit TcpTransport(QString configPath, QString section, QObject *parent = nullptr);
    bool write(const QByteArray &packet) override;

    bool close() override;
    void setupTransport() override;
    void loadConfig() override;
signals:
    //void connectedChanged(bool);
private:
    void resetTimers();
    void scheduleReconnect();

public slots:
    bool open() override;
    void heartbeat() override;
private slots:
    void reconnect();
    void processQueue() override;
    void onReadSocket();
    void onConnected();
    void onDisconnected();
    void onErrorOccured(QAbstractSocket::SocketError err);
};

#endif // TCPTRANSPORT_H
