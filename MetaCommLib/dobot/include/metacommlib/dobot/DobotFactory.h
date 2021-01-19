#ifndef DOBOTFACTORY_H
#define DOBOTFACTORY_H

#include <metacommlib/IRobotFactory.h>

namespace mtcl {

class METACOMMLIB_EXPORT DobotFactory : public IRobotFactory
{
public:
    DobotFactory();
    virtual ~DobotFactory();
    shared_ptr<IRobot> CreateRobot() override;
    shared_ptr<IRobotUserControl> CreateRobotUserControl(QWidget* parent = nullptr) override;
};

}
#endif // DOBOTFACTORY_H
