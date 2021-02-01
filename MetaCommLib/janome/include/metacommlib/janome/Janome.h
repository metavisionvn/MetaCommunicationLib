#ifndef JANOME_H
#define JANOME_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/IRobot.h>
#include <metacommlib/TcpSocketBase.h>
#include <metacommlib/janome/JanomeEncodedMsg.h>
#include <metacommlib/janome/JanomeRobotInformation.h>
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
    bool GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs);
    bool MovePosition(double x, double y, double z, double thetaInDegs);
    bool MovePosition(unique_ptr<IRobotPosition> position) override;

    friend class JanomeDecodeMsg;
signals:
    //Get response from dobot handler
    void OnReceivedMsg(const QByteArray& message);
    //Internal Used. Send message to dobot handler (cross-thread with socket)
    void OnSendMsgChanged(const QByteArray& msg);
    void OnRobotInformUpdated();
protected:

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
    //update position form decoder
    void SetPosition(double x, double y, double z, double thetaInDegs);
    void SetRobotInformation(const JanomeRobotInformation& robotInformation);
    string mIPAddress;
    int mPort;
    JanomeDecodeMsg mDecoder;
    JanomeEncodedMsg mEncoder;

    TcpSocketBase *mSocket;
    QTimer* mAutoReconnectTimer;
    JanomeRobotInformation mRobotInformation;
};

}

#endif // JANOME_H
