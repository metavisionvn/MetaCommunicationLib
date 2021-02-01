#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <QObject>
#include <metacommlib/TcpSocketBase.h>
#include <metacommlib/MetaCommLib_global.h>
using namespace std;

namespace mtcl {

class TcpClient : public TcpSocketBase
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    virtual ~TcpClient();
    bool ConnectTo(string ipaddress, int port) override;
    bool SendData(const QByteArray& data) override;
private:
};

}

#endif // TCPCLIENT_H
