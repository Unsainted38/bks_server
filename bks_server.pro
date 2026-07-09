COMMON_LIB_PATH = $$clean_path($$PWD/third_party/common_lib)

!exists($$COMMON_LIB_PATH/common_lib.pri) {
    error("common_lib not found. Run: git submodule update --init --recursive")
}

include($$COMMON_LIB_PATH/common_lib.pri)

QT       += core network
QT -= gui
CONFIG   += console
CONFIG   += c++14
CONFIG -= app_bundle



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += bks_proxy_server.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += bks_proxy_server.h
