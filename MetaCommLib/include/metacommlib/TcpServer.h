#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <metacommlib/TcpSocketBase.h>
using namespace std;

namespace mtcl {

/**
 * @brief The TcpServer class. Establish connection to target by using TCP/IP. \n
 * Working in Server mode.
 */
class TcpServer : public TcpSocketBase
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    virtual ~TcpServer();
    /**
     * @brief Disconnect. Close opened socket.
     * No connection accepted after.
     */
    virtual void Disconnect() override;
    /**
     * @brief ConnectTo. Open socket to listen incoming connection
     * @param ipaddress (not use)
     * @param port
     * @return
     */
    bool ConnectTo(string ipaddress, int port) override;
    /**
     * @brief SendData. Not Implemented
     * @param data
     * @return
     */
    virtual bool SendData(const QByteArray &data) override;
signals:
    void OnStatusChanged(const bool status);
private slots:
    void HandleNewConnection();
private:
    QTcpServer *mServer;
};

}

#endif // TCPSERVER_H
