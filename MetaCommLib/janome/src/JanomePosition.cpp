#include <metacommlib/janome/JanomePosition.h>

namespace mtcl {

JanomePosition::JanomePosition()
    : IRobotPosition()
{
    //xyzr
    mptrPosition = new double[4];
}

JanomePosition::~JanomePosition()
{

}

double JanomePosition::GetPosX()
{
    return mptrPosition[0];
}

double JanomePosition::GetPosY()
{
    return mptrPosition[1];
}

double JanomePosition::GetPosThetaInDegs()
{
    return mptrPosition[3];
}

double JanomePosition::GetPosZ()
{
    return mptrPosition[2];
}

void JanomePosition::SetPosX(double x)
{
    mptrPosition[0] = x;
}

void JanomePosition::SetPosY(double y)
{
    mptrPosition[1] = y;
}

void JanomePosition::SetPosThetaInDegs(double thetaInDegs)
{
    mptrPosition[3] = thetaInDegs;
}

void JanomePosition::SetPosZ(double z)
{
     mptrPosition[2] = z;
}

void JanomePosition::SetPosition(double x, double y, double z, double thetaInDegs)
{
    mptrPosition[0] = x;
    mptrPosition[1] = y;
    mptrPosition[2] = z;
    mptrPosition[3] = thetaInDegs;
}

void JanomePosition::GetPosition(double &x, double &y, double &z, double &thetaInDegs)
{
    x = mptrPosition[0];
    y = mptrPosition[1];
    z = mptrPosition[2];
    thetaInDegs = mptrPosition[3];
}

}
