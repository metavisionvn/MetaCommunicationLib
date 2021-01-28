#ifndef JANOMEFACTORY_H
#define JANOMEFACTORY_H

#include <metacommlib/IRobotFactory.h>

namespace mtcl {

class METACOMMLIB_EXPORT JanomeFactory : public IRobotFactory
{
public:
    JanomeFactory();
    virtual ~JanomeFactory();
    shared_ptr<IRobot> CreateRobot() override;
    shared_ptr<IRobotUserControl> CreateRobotUserControl(QWidget *parent = nullptr) override;
};

}

#endif // JANOMEFACTORY_H
