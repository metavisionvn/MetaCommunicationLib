#include <metacommlib/techman/Techman.h>

namespace mtcl {

Techman::Techman()
{

}

Techman::~Techman()
{

}

void Techman::SetConnectionAddress(string ipAddress, int port)
{
    mIPAddress = ipAddress;
    mPort = port;
}

void Techman::GetConnectionAddress(string &ipAddress, int &port)
{
    ipAddress = mIPAddress;
    port = mPort;
}

bool Techman::GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs)
{
    return false;
}

bool Techman::MovePosition(double x, double y, double z, double thetaInDegs)
{
    return false;
}

bool Techman::MovePosition(double x, double y)
{
    return false;
}

bool Techman::MovePosition(double x, double y, double thetaInDegrees)
{
    return false;
}

bool Techman::MovePosition(unique_ptr<IRobotPosition> position)
{
    return false;
}

bool Techman::CmdReturnToHome()
{
    return false;
}

bool Techman::CmdJogStart(int index)
{
    return false;
}

bool Techman::CmdJogStop()
{
    return false;
}

void Techman::HandleOnSocketConnectionChanged(bool isConnected)
{

}

void Techman::HandleSocketErrorChanged(const QString &errorMsg)
{

}

void Techman::HandleReceivedMsgChanged(const QByteArray &message)
{

}

void Techman::HandleReconnectTimerChanged()
{

}

bool Techman::UpdateCurrentPosition()
{
    return false;
}

bool Techman::OnStart()
{
    return false;
}

void Techman::OnStop()
{

}

void Techman::OnDoWork()
{

}

}
