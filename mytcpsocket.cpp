#include "mytcpsocket.h"

#include <QString>
#include <QDebug>
#include <QObject>

myTCPSocket::myTCPSocket(QObject *parent):QTcpServer(parent)
{

}


void myTCPSocket::sendMessage()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_0);
    out<<(quint16)0;
    out<<tr("hello tcp");
    out.device()->seek(0);
    out<<(quint16)(block.size()-sizeof(quint16));

    QTcpSocket *clientConnection=this->nextPendingConnection();
    connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
    clientConnection->write(block);
    clientConnection->disconnectFromHost();

    qDebug()<<"The server send message sucessfully.";
}
