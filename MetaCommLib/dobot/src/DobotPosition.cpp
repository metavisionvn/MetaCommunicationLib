#include <metacommlib/dobot/DobotPosition.h>
#include <cstring>

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

void DobotPosition::SetPosX(double x)
{
    mptrPosition[0] = x;
}

void DobotPosition::SetPosY(double y)
{
    mptrPosition[1] = y;
}

void DobotPosition::SetPosT(double thetaInDegs)
{
    mptrPosition[3] = thetaInDegs;
}

void DobotPosition::SetPosZ(double z)
{
    mptrPosition[2] = z;
}

void DobotPosition::GetPosition(double &x, double &y, double &z, double &thetaInDegs)
{
    x = GetPosX();
    y = GetPosY();
    z = GetPosZ();
    thetaInDegs = GetPosT();
}

void DobotPosition::GetJointAngle(double &j1, double &j2, double &j3, double &j4)
{
    j1 = mptrPosition[4];
    j2 = mptrPosition[5];
    j3 = mptrPosition[6];
    j4 = mptrPosition[7];
}

void DobotPosition::SetPosition(double x, double y, double z, double thetaInDegs)
{
    mptrPosition[0] = x;
    mptrPosition[1] = y;
    mptrPosition[2] = z;
    mptrPosition[3] = thetaInDegs;
}

bool DobotPosition::SetJointAngle(double joingAngle[], int numberJoint)
{
    if (numberJoint != 4)
        return false;
    double *ptr = mptrPosition;
    ptr += 4;
    memcpy(ptr, joingAngle, sizeof(double)*numberJoint);
    return true;
}



}

