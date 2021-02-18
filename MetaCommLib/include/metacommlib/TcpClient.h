#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <QObject>
#include <metacommlib/TcpSocketBase.h>
#include <metacommlib/MetaCommLib_global.h>
using namespace std;

namespace mtcl {

/**
 * @brief The TcpClient class. Establish connection to target by using TCP/IP. \n
 * Working in Client mode.
 */
class TcpClient : public TcpSocketBase
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    virtual ~TcpClient();
    /**
     * @brief ConnectTo. Connect to target with IP Address and Port.
     * @param ipaddress
     * @param port
     * @return
     */
    bool ConnectTo(string ipaddress, int port) override;
    /**
     * @brief SendData. Send Data to connected target
     * @param data
     * @return true if socket is open otherwise false.
     */
    bool SendData(const QByteArray& data) override;
private:
};

}

#endif // TCPCLIENT_H
