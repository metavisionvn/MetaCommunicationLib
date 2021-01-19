#include <metacommlib/IRobotPosition.h>

namespace mtcl {

IRobotPosition::IRobotPosition()
    : mptrPosition(nullptr)
{

}

IRobotPosition::~IRobotPosition()
{
    if (mptrPosition != nullptr)
        delete [] mptrPosition;
}

}
