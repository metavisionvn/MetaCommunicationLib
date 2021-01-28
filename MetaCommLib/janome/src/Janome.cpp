#include <metacommlib/janome/Janome.h>
#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/janome/JanomePosition.h>

namespace mtcl {

Janome::Janome()
    : IRobot()
    , mIPAddress("")
    , mPort(0)
    , mDecoder(this)
{
    mCurrentPosition = new JanomePosition();
}

Janome::~Janome()
{

}

void Janome::SetConnectionAddress(string ipAddress, int port)
{
    mIPAddress = ipAddress;
    mPort = port;
}

bool Janome::GetCurrentPosition(double &x, double &y, double &z)
{
    JanomePosition* position = dynamic_cast<JanomePosition*>(mCurrentPosition);
    if (position != nullptr)
    {
        position->GetPosition(x, y, z);
        return true;
    }
    return false;
}

bool Janome::MovePosition(double x, double y, double z)
{
    return true;
}

bool Janome::MovePosition(unique_ptr<IRobotPosition> position)
{
    JanomePosition* pos = dynamic_cast<JanomePosition*>(position.get());
    if (pos != nullptr)
    {
        return MovePosition(pos->GetPosX(), pos->GetPosY(), pos->GetPosZ());
    }
    return false;
}

void Janome::SetPosition(double x, double y, double z)
{
    JanomePosition *pos = dynamic_cast<JanomePosition*>(mCurrentPosition);
    if (pos != nullptr)
    {
        pos->SetPosition(x, y, z);
    }
    emit OnPositionChanged();
}

bool Janome::UpdateCurrentPosition()
{
    //Request to get current position
    return true;
}

bool Janome::Initialize()
{
    return false;
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
