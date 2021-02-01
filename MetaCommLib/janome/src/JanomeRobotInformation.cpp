#include <metacommlib/janome/JanomeRobotInformation.h>

namespace mtcl {

JanomeRobotInformation::JanomeRobotInformation()
{

}

JanomeRobotInformation::~JanomeRobotInformation()
{

}

void mtcl::JanomeRobotInformation::SetMechanicalType(mtcl::JanomeMechanicalType type)
{
    mMechanicalType = type;
}

JanomeMechanicalType JanomeRobotInformation::GetMechanicalType()
{
    return mMechanicalType;
}

void JanomeRobotInformation::SetSeries(string series)
{
    //Support JR3200, JR3300, JR3400, JR3500, JR3600
    // and JC3SingleSided, JCDoubleSided
    mRobotSeries = series;
}

string JanomeRobotInformation::GetSeries()
{
    return mRobotSeries;
}

void JanomeRobotInformation::SetNumberAxisMechanism(int no)
{
    mNumberAxisMechanism = no;
}

int JanomeRobotInformation::GetNumberAxisMechanism()
{
    return mNumberAxisMechanism;
}

void JanomeRobotInformation::SetXAxisStroke(double v)
{
    mXAxisStroke = v;
}

double JanomeRobotInformation::GetXAxisStroke()
{
    return mXAxisStroke;
}

void JanomeRobotInformation::SetYAxisStroke(double v)
{
    mYAxisStroke = v;
}

double JanomeRobotInformation::GetYAxisStroke()
{
    return mYAxisStroke;
}

void JanomeRobotInformation::SetZAxisStroke(double v)
{
    mZAxisStroke = v;
}

double JanomeRobotInformation::GetZAxisStroke()
{
    return mZAxisStroke;
}

void JanomeRobotInformation::SetRAxisStroke(double v)
{
    mRAxisStroke = v;
}

double JanomeRobotInformation::GetRAxisStroke()
{
    return mRAxisStroke;
}

JanomeRobotInformation &JanomeRobotInformation::operator=(const JanomeRobotInformation &robotInform)
{
    mMechanicalType = robotInform.mMechanicalType;
    mRobotSeries = robotInform.mRobotSeries;
    mNumberAxisMechanism = robotInform.mNumberAxisMechanism;
    mXAxisStroke = robotInform.mXAxisStroke;
    mYAxisStroke = robotInform.mYAxisStroke;
    mZAxisStroke = robotInform.mZAxisStroke;
    mRAxisStroke = robotInform.mRAxisStroke;
    return (*this);
}

ostream &operator<<(ostream &os, const JanomeRobotInformation &robotInform)
{
    os << "Robot Series: " << robotInform.mRobotSeries << "\n"
       << "Robot Stroke: " << robotInform.mXAxisStroke << " " << robotInform.mYAxisStroke << " " << robotInform.mZAxisStroke << " " << robotInform.mRAxisStroke << "\n"
       << "Number Axis: " << robotInform.mNumberAxisMechanism << endl;
    return os;

}

}
