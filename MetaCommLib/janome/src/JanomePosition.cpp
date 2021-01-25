#include <metacommlib/janome/JanomePosition.h>

namespace mtcl {

JanomePosition::JanomePosition()
    : IRobotPosition()
{
    //xyz
    mptrPosition = new double[3];
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

double JanomePosition::GetPosT()
{
    throw NotImplemented();
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

void JanomePosition::SetPosT(double thetaInDegs)
{
    (void)thetaInDegs;
    throw NotImplemented();
}

void JanomePosition::SetPosZ(double z)
{
     mptrPosition[2] = z;
}

void JanomePosition::SetPosition(double x, double y, double z)
{
    mptrPosition[0] = x;
    mptrPosition[1] = y;
    mptrPosition[2] = z;
}

void JanomePosition::GetPosition(double &x, double &y, double &z)
{
    x = mptrPosition[0];
    y = mptrPosition[1];
    z = mptrPosition[2];
}

}
