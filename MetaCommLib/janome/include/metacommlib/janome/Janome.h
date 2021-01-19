#ifndef JANOME_H
#define JANOME_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/IRobot.h>
#include <string>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT Janome : public IRobot
{
public:
    Janome();
    virtual ~Janome();
    void SetConnectionAddress(string ipAddress, int port);
    virtual bool GetCurrentPosition(double &x, double &y, double &thetaInDegs) override;
    virtual bool MovePosition(double x, double y, double thetaInDegs) override;

protected:
    virtual bool UpdateCurrentPosition() override;
    virtual void OnStart() override;
    virtual void OnStop() override;
    virtual void OnDoWork() override;
private:
    string mIPAddress;
    int mPort;

};

}

#endif // JANOME_H