#include <metacommlib/dobot/DobotPosition.h>

namespace mtcl {

DobotPosition::DobotPosition()
    : IRobotPosition()
{
    //x, y, z, theta, jointAngle[4]
    mptrPosition = new double[8];
}

DobotPosition::~DobotPosition()
{

}

double DobotPosition::GetPosX()
{
    return mptrPosition[0];
}

double DobotPosition::GetPosY()
{
    return mptrPosition[1];
}

double DobotPosition::GetPosT()
{
    return mptrPosition[3];
}

double DobotPosition::GetPosZ()
{
    return mptrPosition[2];
}

void DobotPosition::GetPosition(double &x, double &y, double &thetaInDegs)
{
    x = GetPosX();
    y = GetPosY();
    thetaInDegs = GetPosT();
}

void DobotPosition::GetPosition(double &x, double &y, double &thetaInDegs, double &z)
{
    x = GetPosX();
    y = GetPosY();
    thetaInDegs = GetPosT();
    z = GetPosZ();
}

}

