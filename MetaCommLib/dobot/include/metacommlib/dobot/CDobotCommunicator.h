#ifndef CDOBOTCOMMUNICATOR_H
#define CDOBOTCOMMUNICATOR_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <metacommlib/dobot/ComPlatform/ProtocolID.h>
#include <metacommlib/dobot/DobotType.h>
#include <metacommlib/dobot/ComPlatform/Message.h>
#include <metacommlib/MetaCommLib_global.h>

class METACOMMLIB_EXPORT CDobotCommunicator : public QObject
{
    Q_OBJECT
private:
    enum StateMachine {
        IdleState,
        SendingGetLeftSpaceCmdState,
        WaitLeftSpaceAckState,
        SendingCmdState,
        WaitCmdAckState
    };

    typedef struct tagMessageHandler {
        bool *isFinished;
        int *result;
        Message *message;
    }MessageHandler;

public:
    CDobotCommunicator();
    virtual ~CDobotCommunicator();

    Q_INVOKABLE void onInit(void);

    Q_SLOT void onNewConnectStatus(bool connected);

    Q_SIGNAL void messageToSend(const Message &message);
    Q_SLOT void onMessageReady(const Message &message);
private:

    Q_INVOKABLE void setCmdTimeout(void *isFinishedAddr, void *resultAddr, unsigned int timeout);
    Q_INVOKABLE void insertMessage(void *isFinishedAddr, void *resultAddr, void *messageAddr);
private:
    Q_SLOT void stateMachine(void);
private:
    bool connected;
    int currentState;
    uint32_t leftSpace;
    unsigned int timeout;
    QTime execTime;
    bool isNewMessageArrived;
    Message newMessage;
    QTimer *timer;
    QVector<MessageHandler> messageHandlers;
};

#endif // CDOBOTCOMMUNICATOR_H
