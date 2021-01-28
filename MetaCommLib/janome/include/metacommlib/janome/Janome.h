#ifndef JANOME_H
#define JANOME_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/IRobot.h>
#include <metacommlib/janome/JanomeSocket.h>
#include <string>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT Janome : public IRobot
{
public:
    Janome();
    virtual ~Janome();
    void SetConnectionAddress(string ipAddress, int port);
    bool GetCurrentPosition(double &x, double &y, double &z);
    bool MovePosition(double x, double y, double z);
    bool MovePosition(unique_ptr<IRobotPosition> position) override;

    friend class JanomeDecodeMsg;
protected:
    //update position form decoder
    void SetPosition(double x, double y, double z);
    virtual void OnStart() override;
    virtual void OnStop() override;
    virtual void OnDoWork() override;
    virtual bool UpdateCurrentPosition() override;
private:
    bool Initialize();
    string mIPAddress;
    int mPort;
    JanomeDecodeMsg mDecoder;
};

}

#endif // JANOME_H
