#ifndef ROBOTPOSITION_H
#define ROBOTPOSITION_H

#include <metacommlib/MetaCommLib_global.h>

namespace mtcl
{
/**
 * @brief The RobotPosition abstract class.
 * Position of robot include x, y, z, r (theta)
 * If one axis not supported, one exception thrown.
 */
class METACOMMLIB_EXPORT IRobotPosition
{
public:
    IRobotPosition();
    virtual ~IRobotPosition();
    virtual double GetPosX() = 0;
    virtual double GetPosY() = 0;
    virtual double GetPosThetaInDegs() = 0;

    virtual void SetPosX(double x) = 0;
    virtual void SetPosY(double y) = 0;
    virtual void SetPosThetaInDegs(double thetaInDegs) = 0;

protected:
    double *mptrPosition;
};
}

#endif // ROBOTPOSITION_H
