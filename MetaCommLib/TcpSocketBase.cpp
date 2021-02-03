#include <metacommlib/TcpSocketBase.h>

namespace mtcl {

TcpSocketBase::TcpSocketBase(QObject *parent)
    : QObject(parent)
    , mSocket(nullptr)
    , mIsConnected(false)
{

}

TcpSocketBase::~TcpSocketBase()
{
    if (mSocket)
    {
        if (mSocket->isOpen())
            mSocket->close();
        delete mSocket;
        mSocket = nullptr;
    }
}

void TcpSocketBase::Disconnect()
{
    if (mSocket != nullptr && mSocket->isOpen())
        mSocket->close();
}

qptrdiff TcpSocketBase::SocketDescriptor() const
{
    if (mSocket != nullptr)
        return mSocket->socketDescriptor();
    else
        throw std::runtime_error("Socket is not valid");
}

void TcpSocketBase::HandleSendMessage(const QByteArray &message)
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

void TcpSocketBase::HandleConnectedChanged()
{
    mIsConnected = true;
    emit OnConnectionChanged(true);
}

void TcpSocketBase::HandleDisconnectChanged()
{
    mIsConnected = false;
    emit OnConnectionChanged(false);
}

void TcpSocketBase::HandleErrorChanged(QAbstractSocket::SocketError error)
{
    emit OnErrorChanged(QVariant::fromValue(error).toString());
}

void TcpSocketBase::HandleDataReceivedChanged()
{
    if (mSocket != nullptr)
    {
        QByteArray buffer = mSocket->readAll();
        emit OnReceivedMsgChanged(buffer);
    }
}

}
