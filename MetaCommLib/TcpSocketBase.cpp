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
    cout << "[TcpSocketBase::HandleConnectedChanged]" << endl;
    mIsConnected = true;
    emit OnConnectionChanged(true);
}

void TcpSocketBase::HandleDisconnectChanged()
{
    cout << "[TcpSocketBase::HandleDisconnectChanged]" << endl;
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
