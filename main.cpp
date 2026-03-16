#include <QCoreApplication>
#include <bks_proxy_server.h>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    BksProxyServer *server = new BksProxyServer(QCoreApplication::applicationDirPath() +
            "/client_server/config.ini", "BksProxyServer");
    return a.exec();
}
