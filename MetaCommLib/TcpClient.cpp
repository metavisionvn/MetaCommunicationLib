#include <metacommlib/TcpClient.h>
#include <QDataStream>
#include <iostream>
using namespace std;

namespace mtcl {

TcpClient::TcpClient(QObject *parent)
    : TcpSocketBase(parent)
{
    mSocket = new QTcpSocket(parent);

    connect(mSocket, SIGNAL(connected()), this, SLOT(HandleConnectedChanged()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(HandleDisconnectChanged()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(HandleDataReceivedChanged()));
    connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(HandleErrorChanged(QAbstractSocket::SocketError)));
}

TcpClient::~TcpClient()
{

}

bool TcpClient::ConnectTo(string ipaddress, int port)
{
    mSocket->connectToHost(QString::fromStdString(ipaddress), port);
    return true;
}

bool TcpClient::SendData(const QByteArray &data)
{
    if (mSocket != 0 && mSocket->isOpen())
    {
        mSocket->write(data);
        mSocket->flush();
        return true;
    }
    return false;
}

}

