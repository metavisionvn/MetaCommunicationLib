#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <metacommlib/TcpSocketBase.h>
using namespace std;

namespace mtcl {

class TcpServer : public TcpSocketBase
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    virtual ~TcpServer();
    virtual void Disconnect() override;
    bool ConnectTo(string ipaddress, int port) override;
signals:
    void OnStatusChanged(const bool status);
private slots:
    void HandleNewConnection();
private:
    QTcpServer *mServer;
};

}

#endif // TCPSERVER_H
