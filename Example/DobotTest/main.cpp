#include "MainWindow.h"

#include <QApplication>
#include <metacommlib/dobot/DobotFactory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    mtcl::DobotFactory dobotFactory;
    shared_ptr<mtcl::IRobot> robot = dobotFactory.CreateRobot();
    shared_ptr<mtcl::IRobotUserControl> userControl = dobotFactory.CreateRobotUserControl(&w);
    userControl->SetRobot(robot);

    w.AttachDobotWidget(userControl.get());

    w.show();
    return a.exec();
}
