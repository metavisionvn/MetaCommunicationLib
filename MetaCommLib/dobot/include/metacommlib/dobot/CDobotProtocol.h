#ifndef CDOBOTPROTOCOL_H
#define CDOBOTPROTOCOL_H

#include <QObject>
#include <metacommlib/dobot/ComPlatform/Message.h>
#include <metacommlib/MetaCommLib_global.h>

class QIODevice;

class METACOMMLIB_EXPORT CDobotProtocol : public QObject
{
    Q_OBJECT
public:
    CDobotProtocol();
    virtual ~CDobotProtocol();

    Q_INVOKABLE void onInit(void);

    Q_SLOT void onNewConnectStatus(bool connected);

    Q_SIGNAL void bytesToWrite(const char *data, qint64 maxSize);
    Q_SLOT void onBytesWritten(void);
    Q_SLOT void onBytesReady(QByteArray array);

    Q_SLOT void sendMessage(const Message &message);
    Q_SIGNAL void messageReady(const Message &message);
private:
    Q_SLOT void periodicTask(void);
private:
    bool connected;
    bool readyWrite;

    QScopedArrayPointer<uint8_t> txBuffer;
    QScopedPointer<ProtocolHandler> protocolHandler;
    QScopedArrayPointer<uint8_t> txRawByteBuffer;
    QScopedArrayPointer<uint8_t> rxRawByteBuffer;
    QScopedArrayPointer<Packet> txPacketBuffer;
    QScopedArrayPointer<Packet> rxPacketBuffer;
};

#endif // CDOBOTPROTOCOL_H
