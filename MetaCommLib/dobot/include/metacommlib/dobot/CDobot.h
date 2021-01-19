#ifndef CDOBOT_H
#define CDOBOT_H

#include "CDobotConnector.h"
#include "CDobotProtocol.h"
#include "CDobotCommunicator.h"
#include <metacommlib/MetaCommLib_global.h>
#include <QObject>

class METACOMMLIB_EXPORT QCoreApplication;

class METACOMMLIB_EXPORT CDobot : public QObject
{
    Q_OBJECT
public:
    static CDobot *instance(void);
    void exec(void);
private:
    CDobot(QObject *parent = 0);
    virtual ~CDobot();
public:
    CDobotConnector *connector;
    QThread *connectorTargetThread;
    CDobotProtocol *protocol;
    QThread *protocolTargetThread;
    CDobotCommunicator *communicator;
    QThread *communicatorTargetThread;
    QCoreApplication *app;
};

#endif // CDOBOT_H
