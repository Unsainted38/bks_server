#ifndef MSHPRCOMMAND_H
#define MSHPRCOMMAND_H

#include <QObject>
#include <QMap>
#include "abstract_command.h"


class MShPRCommand : public AbstractCommand {
    Q_OBJECT
    QString m_addr;
    QString m_cmd;
public:
    explicit MShPRCommand(QString addr, QString cmd, ValueType type);
    QByteArray makeWriteCommand() override;
    QByteArray makeReadCommand() override;
    void setAddress(QString addr);
signals:

};

#endif // MSHPRCOMMAND_H
