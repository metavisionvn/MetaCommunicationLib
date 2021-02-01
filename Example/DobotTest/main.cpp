#include "MainWindow.h"

#include <QApplication>
#include <metacommlib/dobot/DobotFactory.h>
#include <metacommlib/IRobotFactory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    shared_ptr<mtcl::IRobotFactory> robotFactory = shared_ptr<mtcl::IRobotFactory>(new mtcl::DobotFactory());
    shared_ptr<mtcl::IRobot> robot = robotFactory->CreateRobot();
    shared_ptr<mtcl::IRobotUserControl> userControl = robotFactory->CreateRobotUserControl(&w);
    userControl->SetRobot(robot);

    w.AttachDobotWidget(userControl.get());

    w.show();
    return a.exec();
}
