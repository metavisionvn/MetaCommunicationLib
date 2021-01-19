#include <metacommlib/janome/JanomeSocket.h>

namespace mtcl {

JanomeSocket::JanomeSocket(QObject *parent)
    : QObject(parent)
    , mSocket(nullptr)
    , mIsConnected(false)
{
    mSocket = new QTcpSocket(parent);

    connect(mSocket, SIGNAL(connected()), this, SLOT(HandleConnectedChanged()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(HandleDisconnectChanged()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(HandleDataReceivedChanged()));
    connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(HandleErrorChanged(QAbstractSocket::SocketError)));
}

JanomeSocket::~JanomeSocket()
{
    if (mSocket)
    {
        if (mSocket->isOpen())
            mSocket->close();
        delete mSocket;
        mSocket = nullptr;
    }
}

void JanomeSocket::Disconnect()
{
    if (mSocket != nullptr && mSocket->isOpen())
        mSocket->close();
}

bool JanomeSocket::SendData(const QByteArray &data)
{
    if (mSocket != 0 && mSocket->isOpen())
    {
        mSocket->write(data);
        mSocket->flush();
        return true;
    }
    return false;
}

void JanomeSocket::HandleSendMessage(const QByteArray &message)
{
    bool status = false;
    if (mSocket != nullptr && mSocket->isOpen())
    {
        mSocket->write(message);
        mSocket->flush();
        status = true;
    }
    emit OnSendMessageChanged(status);
}

void JanomeSocket::HandleConnectedChanged()
{
    mIsConnected = true;
    emit OnConnectionChanged(true);
}

void JanomeSocket::HandleDisconnectChanged()
{
    mIsConnected = false;
    emit OnConnectionChanged(false);
}

void JanomeSocket::HandleErrorChanged(QAbstractSocket::SocketError error)
{
    emit OnErrorChanged(QVariant::fromValue(error).toString());
}

void JanomeSocket::HandleDataReceivedChanged()
{
    if (mSocket != nullptr)
    {
        QByteArray buffer = mSocket->readAll();
        emit OnReceivedMsgChanged(buffer);
    }
}

}
