#include <metacommlib/janome/Janome.h>
#include <metacommlib/janome/JanomeDecodeMsg.h>

namespace mtcl {

Janome::Janome()
    : IRobot()
    , mIPAddress("")
    , mPort(0)
    , mDecoder(this)
{

}

Janome::~Janome()
{

}

void Janome::SetConnectionAddress(string ipAddress, int port)
{
    mIPAddress = ipAddress;
    mPort = port;
}

bool Janome::GetCurrentPosition(double &x, double &y, double &thetaInDegs)
{
    return true;
}

bool Janome::MovePosition(double x, double y, double thetaInDegs)
{
    return true;
}

void Janome::SetPosition(double x, double y, double z)
{

}

void Janome::SetPosition(double x, double y, double z, double thetaIDegs)
{

}

bool Janome::UpdateCurrentPosition()
{
    return true;
}

void Janome::OnStart()
{
    if (GetConnectionStatus() != RobotConnect_Connected)
    {
        RobotConnectionStatus status = RobotConnect_Undefined;
    }
}

void Janome::OnStop()
{

}

void Janome::OnDoWork()
{
    UpdateCurrentPosition();
}

}
