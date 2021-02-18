#ifndef TCPSOCKETBASE_H
#define TCPSOCKETBASE_H

#include <string>
#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <metacommlib/MetaCommLib_global.h>
using namespace std;

namespace mtcl {

/**
 * @brief The TcpSocketBase class. Abstract class for TCP/IP connection
 */
class METACOMMLIB_EXPORT TcpSocketBase : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocketBase(QObject *parent = nullptr);
    virtual ~TcpSocketBase();
    /**
     * @brief Disconnect. Close connection
     */
    virtual void Disconnect();
    /**
     * @brief ConnectTo. Open connection and connect to target
     * @param ip
     * @param port
     * @return false if connection is failed otherwise true. \n
     * If connection takes time to establish, one temporary result returned. \n
     * Later, one signal is emitted on ConnectionChanged.
     */
    virtual bool ConnectTo(string ip, int port) = 0;
    /**
     * @brief SendData. Send data to target.
     * @param data
     * @return
     */
    virtual bool SendData(const QByteArray& data) = 0;
    /**
     * @brief SocketDescriptor
     * @return
     * @exception socket invalid
     */
    virtual qptrdiff SocketDescriptor() const;
signals:
    /**
     * @brief OnConnectionChanged. Inform on Connection status
     * @param isConnected
     */
    void OnConnectionChanged(bool isConnected);
    /**
     * @brief OnErrorChanged
     * @param errorMsg
     */
    void OnErrorChanged(const QString& errorMsg);
    /**
     * @brief OnReceivedMsgChanged
     * @param message
     */
    void OnReceivedMsgChanged(const QByteArray& message);
    /**
     * @brief OnSendMessageChanged
     * @param status
     */
    void OnSendMessageChanged(const bool status);
public slots:
    /**
     * @brief HandleSendMessage. Using slot to send message to target.
     * To handle cross-thread issue.
     * @param message
     */
    void HandleSendMessage(const QByteArray &message);
    /**
     * @brief HandleConnectedChanged
     */
    void HandleConnectedChanged();
    /**
     * @brief HandleDisconnectChanged
     */
    void HandleDisconnectChanged();
    /**
     * @brief HandleErrorChanged
     */
    void HandleErrorChanged(QAbstractSocket::SocketError);
    /**
     * @brief HandleDataReceivedChanged
     */
    void HandleDataReceivedChanged();
protected:
    QTcpSocket *mSocket;
    bool mIsConnected;
};

}


#endif // TCPSOCKETBASE_H
