#include <metacommlib/techman/TechmanPosition.h>

namespace mtcl {

TechmanPosition::TechmanPosition()
    : IRobotPosition()
{
    //x, y, z, thetaInDegs
    mptrPosition = new double[4];
}

TechmanPosition::~TechmanPosition()
{

}

double TechmanPosition::GetPosX()
{
    return mptrPosition[0];
}

double TechmanPosition::GetPosY()
{
    return mptrPosition[1];
}

double TechmanPosition::GetPosThetaInDegs()
{
    return mptrPosition[3];
}

double TechmanPosition::GetPosZ()
{
    return mptrPosition[2];
}

void TechmanPosition::SetPosX(double x)
{
    mptrPosition[0] = x;
}

void TechmanPosition::SetPosY(double y)
{
    mptrPosition[1] = y;
}

void TechmanPosition::SetPosThetaInDegs(double thetaInDegs)
{
    mptrPosition[3] = thetaInDegs;
}

void TechmanPosition::SetPosZ(double z)
{
    mptrPosition[2] = z;
}

void TechmanPosition::SetPosition(double x, double y, double z, double thetaInDegs)
{
    mptrPosition[0] = x;
    mptrPosition[1] = y;
    mptrPosition[2] = z;
    mptrPosition[3] = thetaInDegs;
}

void TechmanPosition::GetPosition(double &x, double &y, double &z, double &thetaInDegs)
{
    x = mptrPosition[0];
    y = mptrPosition[1];
    z = mptrPosition[2];
    thetaInDegs = mptrPosition[3];
}

}
