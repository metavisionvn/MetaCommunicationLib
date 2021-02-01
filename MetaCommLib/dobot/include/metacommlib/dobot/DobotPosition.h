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
    virtual double GetPosThetaInDegs() override;
    virtual double GetPosZ();

    virtual void SetPosX(double x) override;
    virtual void SetPosY(double y) override;
    virtual void SetPosThetaInDegs(double thetaInDegs) override;
    virtual void SetPosZ(double z);

    virtual void GetPosition(double &x, double &y, double &z, double &thetaInDegs);
    void GetJointAngle(double &j1, double &j2, double &j3, double &j4);

    void SetPosition(double x, double y, double z, double thetaInDegs);
    bool SetJointAngle(double joingAngle[], int numberJoint = 4);

};
}

#endif // DOBOTPOSITION_H
