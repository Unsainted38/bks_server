#include "mshpr_command.h"

MShPRCommand::MShPRCommand(QString addr, QString cmd, ValueType type)
    : AbstractCommand(type),
      m_addr(addr),
      m_cmd(cmd) {

}

QByteArray MShPRCommand::makeWriteCommand() {
    QByteArray payload;

    if((m_cmd == "AT1" || m_cmd == "ADR") && m_valQStr.size() == 1) {
        m_valQStr.push_front("0");
    }

    payload = "<" + m_addr.toUtf8() + "/" + m_cmd.toUtf8() + "=" + m_valQStr.toUtf8() + "*";

    quint8 checksum = 0;

    for(auto b : payload) {
        checksum += static_cast<quint8>(b);
    }

    checksum &= 0xFF;
    QByteArray csHex;
    csHex.append(checksum);
    QByteArray packet;
    QDataStream o(&packet, QIODevice::WriteOnly);
    o.writeRawData(payload.constData(), payload.size());
    o.writeRawData(csHex.constData(), csHex.size());
    o.writeRawData("\r", 1);
    qDebug() << packet.constData();
    return packet;
}
void dumpAscii(const QByteArray &data) {
    QString out;
    out.reserve(data.size() * 4);

    for(unsigned char b : data) {
        if(b >= 0x20 && b <= 0x7E) {
            out += QChar(b);
        } else if(b == '\r') {
            out += "\\r";
        } else if(b == '\n') {
            out += "\\n";
        } else {
            out += QString("\\x%1").arg((unsigned char)b, 2, 16, QLatin1Char('0'));
        }
    }

    qDebug() << out;
}

QByteArray MShPRCommand::makeReadCommand() {
    QByteArray payload = "<" + m_addr.toUtf8() + "/" + m_cmd.toUtf8() + "*";
    //qDebug() << payload.constData();
    quint8 checksum = 0;

    for(auto b : payload) {
        checksum += static_cast<quint8>(b);
    }

    checksum &= 0xFF;

    QByteArray csHex; //= QByteArray::number(checksum, 16).rightJustified(2, '0').toUpper();
    csHex.append(checksum);
    QByteArray packet;
    packet.append(payload);
    packet.append(csHex);
    packet.append('\r');
    QString debugStr;

    for(auto b : payload) {
        if(b >= 0x20 && b <= 0x7E) {
            debugStr += QChar(b);
        } else if(b == '\r') {
            debugStr += "\\r";
        } else {
            debugStr += QString("\\x%1").arg((unsigned char)b, 2, 16, QLatin1Char('0'));
        }
    }

    //qDebug() << debugStr;
    //dumpAscii(packet);
    return packet;
}

void MShPRCommand::setAddress(QString addr) {
    m_addr = addr;
}
