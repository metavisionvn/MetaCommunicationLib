#ifndef JANOME_H
#define JANOME_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/IRobot.h>
#include <metacommlib/TcpSocketBase.h>
#include <QTimer>
#include <string>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT Janome : public IRobot
{
    Q_OBJECT
public:
    Janome();
    virtual ~Janome();
    void SetConnectionAddress(string ipAddress, int port);
    bool GetCurrentPosition(double &x, double &y, double &z);
    bool MovePosition(double x, double y, double z);
    bool MovePosition(unique_ptr<IRobotPosition> position) override;

    friend class JanomeDecodeMsg;
signals:
    //Get response from dobot handler
    void OnReceivedMsg(const QString& message);
    //Internal Used. Send message to dobot handler (cross-thread with socket)
    void OnSendMsgChanged(const QByteArray& msg);
protected:
    //update position form decoder
    void SetPosition(double x, double y, double z);
    virtual void OnStart() override;
    virtual void OnStop() override;
    virtual void OnDoWork() override;
    virtual bool UpdateCurrentPosition() override;
private slots:
    void HandleOnSocketConnectionChanged(bool isConnected);
    void HandleSocketErrorChanged(const QString& errorMsg);
    void HandleReceivedMsgChanged(const QByteArray &message);
    void HandleReconnectTimerChanged();
private:
    bool Initialize();
    string mIPAddress;
    int mPort;
    JanomeDecodeMsg mDecoder;
    TcpSocketBase *mSocket;
    QTimer* mAutoReconnectTimer;
};

}

#endif // JANOME_H
