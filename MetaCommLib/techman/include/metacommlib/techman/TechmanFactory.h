#ifndef TECHMANFACTORY_H
#define TECHMANFACTORY_H

#include <metacommlib/IRobotFactory.h>

namespace mtcl {

class METACOMMLIB_EXPORT TechmanFactory : public IRobotFactory
{
public:
    TechmanFactory();
    virtual ~TechmanFactory();
    shared_ptr<IRobot> CreateRobot() override;
    shared_ptr<IRobotUserControl> CreateRobotUserControl(QWidget *parent = nullptr) override;
};

}

#endif // TECHMANFACTORY_H
