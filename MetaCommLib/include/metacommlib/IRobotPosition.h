#ifndef ROBOTPOSITION_H
#define ROBOTPOSITION_H

#include <metacommlib/MetaCommLib_global.h>

namespace mtcl
{
/**
 * @brief The RobotPosition class.
 * Position of robot include x, y, r (theta)
 */
class METACOMMLIB_EXPORT IRobotPosition
{
public:
    IRobotPosition();
    virtual ~IRobotPosition();
    virtual double GetPosX() = 0;
    virtual double GetPosY() = 0;
    virtual double GetPosT() = 0;
    virtual void GetPosition(double &x, double &y, double &thetaInDegs) = 0;
protected:
    double *mptrPosition;
};
}

#endif // ROBOTPOSITION_H
