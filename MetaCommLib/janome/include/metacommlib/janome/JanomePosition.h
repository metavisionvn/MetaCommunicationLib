#ifndef JANOMEPOSITION_H
#define JANOMEPOSITION_H

#include <metacommlib/IRobotPosition.h>
#include <metacommlib/CommLibDefines.h>

namespace mtcl {

/**
 * @brief The JanomePosition class. Position of Janome robot
 */
class METACOMMLIB_EXPORT JanomePosition : public IRobotPosition
{
public:
    JanomePosition();
    virtual ~JanomePosition();

    virtual double GetPosX() override;
    virtual double GetPosY() override;
    virtual double GetPosThetaInDegs() override;
    virtual double GetPosZ();

    virtual void SetPosX(double x) override;
    virtual void SetPosY(double y) override;
    virtual void SetPosThetaInDegs(double thetaInDegs) override;
    virtual void SetPosZ(double z);

    virtual void SetPosition(double x, double y, double z, double thetaInDegs);
    virtual void GetPosition(double &x, double &y, double &z, double &thetaInDegs);
};

}

#endif // JANOMEPOSITION_H
