#include "mytcpsocket.h"

#include <QString>
#include <QDebug>
#include <QObject>
#include<QMessageBox>

myTCPSocket::myTCPSocket(QObject *parent):QTcpServer(parent)
{

}


void myTCPSocket::startPVPOnlineGame()
{
    QMessageBox::information(NULL, "server", "Start playing chess online!");
}

void myTCPSocket::sendMessage(QString msg)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_0);
    out<<(quint16)0;
    out<<msg;
    out.device()->seek(0);
    out<<(quint16)(block.size()-sizeof(quint16));

    QTcpSocket *clientConnection=this->nextPendingConnection();
    connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
    clientConnection->write(block);
    //clientConnection->disconnectFromHost();

    //qDebug()<<"The server send message sucessfully.";
    //QMessageBox::information(NULL, "server", "send over");
}

void myTCPSocket::readMessage()
{

}
