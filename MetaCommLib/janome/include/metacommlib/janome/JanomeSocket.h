#ifndef JANOMESOCKET_H
#define JANOMESOCKET_H

#include <QObject>
#include <metacommlib/MetaCommLib_global.h>
#include <QTcpSocket>
#include <string>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT JanomeSocket : public QObject
{
    Q_OBJECT
public:
    explicit JanomeSocket(QObject *parent = nullptr);
    virtual ~JanomeSocket();
    void Disconnect();
    bool ConnectTo(string ip, int port);
    bool SendData(const QByteArray& data);
signals:
    void OnConnectionChanged(bool isConnected);
    void OnErrorChanged(const QString& errorMsg);
    void OnReceivedMsgChanged(const QByteArray &data);
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
#endif // JANOMESOCKET_H
