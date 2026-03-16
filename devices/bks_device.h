#ifndef BKSDEVICE_H
#define BKSDEVICE_H

#include <QObject>
#include <QTimer>

#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "cmd/mshpr_command.h"
#include "parsers/mshpr_parser.h"

enum BksBaud {
    Baud4800 = 0,
    Baud9600 = 1,
    Baud19200 = 2,
    Baud38400 = 3
};


struct BKS_COMMANDS {
    static constexpr const char *STATUS_CMD = "STA";
    static constexpr const char *FC_LOW_FREQ_CMD = "FC1";
    static constexpr const char *FC_HIGH_FREQ_CMD = "FC2";
    static constexpr const char *FX_LOW_FREQ_CMD = "FX1";
    static constexpr const char *FX_HIGH_FREQ_CMD = "FX2";
    static constexpr const char *ADDRESS_CMD = "ADR";
    static constexpr const char *BAUD_CMD = "BRG";
};
class BksDevice : public QObject {
    Q_OBJECT

    QString m_configPath;
    QString m_section;
    SerialCircularRequester *m_requester;
    MShPRParser *m_parser;
    AbstractNetworkTransport *m_transport;
    AbstractCommand *StatusCommand, *FC1Command, *FC2Command, *FX1Command, *FX2Command, *AddressCommand, *BaudCommand;
    QString m_deviceAddr = "00";
    bool m_statusOnline = false;
    quint8 m_FC1 = 0;
    quint8 m_FC2 = 0;
    quint8 m_FX1 = 0;
    quint8 m_FX2 = 0;
    quint8 m_baud = BksBaud::Baud19200;
    QString m_lastAnswer;
    QTimer *m_timer;
public:
    explicit BksDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    void loadConfig();
    quint8 getFC1();
    quint8 getFC2();
    quint8 getFX1();
    quint8 getFX2();
    quint8 getDeviceAddress();
    QString getLastAnswer();
    bool getStatusOnline();
    void setFC1(quint8 FC1);
    void setFC2(quint8 FC2);
    void setFX1(quint8 FX1);
    void setFX2(quint8 FX2);
    void setDeviceAddress(quint8 value);
    void setBaud(BksBaud baud);
    void setBaud(quint8 baud);
signals:

private:

private slots:
    void processData(QString addr, QMap<QString, int> fieldsMap);
    void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // BKSDEVICE_H
