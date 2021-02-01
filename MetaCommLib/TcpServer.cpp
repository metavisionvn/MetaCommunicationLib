#include <metacommlib/TcpServer.h>

namespace mtcl {

TcpServer::TcpServer(QObject *parent)
    : TcpSocketBase(parent)
{
    mServer = new QTcpServer();
    connect(mServer, SIGNAL(newConnection()), this, SLOT(HandleNewConnection()));
}

TcpServer::~TcpServer()
{
    if (mServer)
    {
        mServer->close();
        delete mServer;
        mServer = nullptr;
        //socket belong to server and server handle socket's live time.
        //Set mSocket to nullptr to handle dangling pointer.
        mSocket = nullptr;
    }
}

void TcpServer::Disconnect()
{
    if (mServer)
    {
        mServer->close();
    }
    TcpSocketBase::Disconnect();
}

bool TcpServer::ConnectTo(string ipaddress, int port)
{
    (void)ipaddress;
    bool ret = true;
    if(!mServer->listen(QHostAddress::Any, port))
    {
        ret = false;
    }
    emit OnStatusChanged(ret);
    return ret;
}

bool TcpServer::SendData(const QByteArray &data)
{
    return false;
}

void TcpServer::HandleNewConnection()
{
    QTcpSocket *socket = mServer->nextPendingConnection();
    if (mIsConnected)
    {
        socket->close();
        //socket belong to server. Both Explicit delete and implicit delete working
        delete socket;
        socket = nullptr;
    }
    else
    {
        mIsConnected = true;
        mSocket = socket;
        connect(mSocket, SIGNAL(disconnected()), this, SLOT(HandleDisconnectChanged()));
        connect(mSocket, SIGNAL(readyRead()), this, SLOT(HandleDataReceivedChanged()));
        connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(HandleErrorChanged(QAbstractSocket::SocketError)));
    }
    emit OnConnectionChanged(true);
}

}
