#ifndef JANOMEPOSITION_H
#define JANOMEPOSITION_H

#include <metacommlib/IRobotPosition.h>
#include <metacommlib/CommLibDefines.h>

namespace mtcl {

class JanomePosition : public IRobotPosition
{
public:
    JanomePosition();
    virtual ~JanomePosition();

    virtual double GetPosX() override;
    virtual double GetPosY() override;
    virtual double GetPosT() override;
    virtual double GetPosZ();

    virtual void SetPosX(double x) override;
    virtual void SetPosY(double y) override;
    virtual void SetPosT(double thetaInDegs) override;
    virtual void SetPosZ(double z);

    virtual void SetPosition(double x, double y, double z);
    virtual void GetPosition(double &x, double &y, double &z);


};

}

#endif // JANOMEPOSITION_H
