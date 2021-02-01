#ifndef TCPSOCKETBASE_H
#define TCPSOCKETBASE_H

#include <string>
#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <metacommlib/MetaCommLib_global.h>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT TcpSocketBase : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocketBase(QObject *parent = nullptr);
    virtual ~TcpSocketBase();
    virtual void Disconnect();
    virtual bool ConnectTo(string ip, int port) = 0;
    virtual bool SendData(const QByteArray& data) = 0;
signals:
    void OnConnectionChanged(bool isConnected);
    void OnErrorChanged(const QString& errorMsg);
    void OnReceivedMsgChanged(const QByteArray& message);
    void OnSendMessageChanged(const bool status);
public slots:
    void HandleSendMessage(const QByteArray &message);
    void HandleConnectedChanged();
    void HandleDisconnectChanged();
    void HandleErrorChanged(QAbstractSocket::SocketError);
    void HandleDataReceivedChanged();
protected:

protected:
    QTcpSocket *mSocket;
    bool mIsConnected;
};

}


#endif // TCPSOCKETBASE_H
