greaterThan(QT_MAJOR_VERSION, 4) {
    QT += serialport widgets
} else {
    CONFIG += serialport
    QT += gui
}
QT       += core network
CONFIG   += console
CONFIG   += c++17
CONFIG -= app_bundle

include(/home/user/develop/host/projects/common/libs/app.pri)
include(/home/user/develop/host/projects/common/libs/serial.pri)

ROOT_APS = spps/data_servers
ROOT_SUBDIRS = spps/data_servers



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += bks_proxy_server.cpp \
        algorithm/binary_coded_decimals_converter.cpp \
        algorithm/crc16.cpp \
        algorithm/crc8.cpp \
        cmd/abstract_command.cpp \
        cmd/mshpr_command.cpp \
        devices/bks_device.cpp \
        main.cpp \
        network_transport/abstract_network_transport.cpp \
        network_transport/abstract_network_transport_factory.cpp \
        network_transport/serial_transport.cpp \
        network_transport/tcp_transport.cpp \
        parsers/mshpr_parser.cpp \
        requesters/serial_circular_requester.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += bks_proxy_server.h \
    algorithm/binary_coded_decimals_converter.h \
    algorithm/crc16.h \
    algorithm/crc8.h \
    cmd/abstract_command.h \
    cmd/mshpr_command.h \
    devices/bks_device.h \
    network_transport/abstract_network_transport.h \
    network_transport/abstract_network_transport_factory.h \
    network_transport/network_transport_locker.h \
    network_transport/serial_transport.h \
    network_transport/tcp_transport.h \
    parsers/mshpr_parser.h \
    requesters/serial_circular_requester.h
