#include "MainWindow.h"

#include <QApplication>
#include <metacommlib/dobot/DobotFactory.h>
#include <metacommlib/IRobotFactory.h>
#include <metacommlib/janome/JanomeFactory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    shared_ptr<mtcl::IRobotFactory> robotFactory = shared_ptr<mtcl::IRobotFactory>(new mtcl::DobotFactory());
    shared_ptr<mtcl::IRobot> robot = robotFactory->CreateRobot();
    shared_ptr<mtcl::IRobotUserControl> userControl = robotFactory->CreateRobotUserControl(&w);
    userControl->SetRobot(robot);

    shared_ptr<mtcl::IRobotFactory> robotFactory2 = shared_ptr<mtcl::IRobotFactory>(new mtcl::JanomeFactory());
    shared_ptr<mtcl::IRobot> robot2 = robotFactory2->CreateRobot();
    shared_ptr<mtcl::IRobotUserControl> userControl2 = robotFactory2->CreateRobotUserControl(&w);
    userControl2->SetRobot(robot2);

    w.AttachDobotWidget(userControl.get());
    w.AttachJanomeWidget(userControl2.get());

    w.show();
    return a.exec();
}
