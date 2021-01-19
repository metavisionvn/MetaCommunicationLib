#ifndef DOBOTPOSITION_H
#define DOBOTPOSITION_H

#include <metacommlib/IRobotPosition.h>

namespace mtcl {

class METACOMMLIB_EXPORT DobotPosition : public IRobotPosition
{
public:
    DobotPosition();
    virtual ~DobotPosition();
    virtual double GetPosX() override;
    virtual double GetPosY() override;
    virtual double GetPosT() override;
    double GetPosZ();
    virtual void GetPosition(double &x, double &y, double &thetaInDegs) override;
    virtual void GetPosition(double &x, double &y, double &thetaInDegs, double &z);
    void GetJointAngle(double &j1, double &j2, double &j3, double &j4);

    void SetPosition(double x, double y, double thetaInDegs) override;
    void SetPosition(double x, double y, double thetaInDegs, double z);
    bool SetJointAngle(double joingAngle[], int numberJoint = 4);
};
}

#endif // DOBOTPOSITION_H
