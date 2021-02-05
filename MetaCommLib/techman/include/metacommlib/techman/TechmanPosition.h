#ifndef TECHMANPOSITION_H
#define TECHMANPOSITION_H

#include <metacommlib/IRobotPosition.h>
#include <metacommlib/CommLibDefines.h>

namespace mtcl {

class METACOMMLIB_EXPORT TechmanPosition : public IRobotPosition
{
public:
    TechmanPosition();
    virtual ~TechmanPosition();

    double GetPosX();
    double GetPosY();
    double GetPosThetaInDegs();
    double GetPosZ();
    void SetPosX(double x);
    void SetPosY(double y);
    void SetPosThetaInDegs(double thetaInDegs);
    void SetPosZ(double z);
};

}
#endif // TECHMANPOSITION_H
