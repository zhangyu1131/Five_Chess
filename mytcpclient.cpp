#include"mytcpclient.h"
#include <QMessageBox>

myTCPClient::myTCPClient(QObject *parent)
{

}

void myTCPClient::newConnect()
{
    blockSize=0;
    //取消现有连接
    this->abort();

    this->connectToHost("127.0.0.1",5005);
    startPVPOnlineGame();
    qDebug()<<"client send new connection.";
}

void myTCPClient::sendMessage(QString msg)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_0);
    out<<(quint16)0;
    out<<msg;
    out.device()->seek(0);
    out<<(quint16)(block.size()-sizeof(quint16));

   // this->write(msg);
}

void myTCPClient::readMessage()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_0);

    //如果刚开始接受，即blocksize==0
    if(blockSize==0)
    {
        //如果消息小于两个字节，说明消息有错
        if(this->bytesAvailable()<(int)sizeof(quint16))
            return;

        in>>blockSize;
    }
    //没有接收到全部数据也return
    if(this->bytesAvailable()<blockSize)
        return;
    in>>msg;
    qDebug()<<"The client has received the msg: "+msg;
   // QMessageBox::information(NULL, "client", msg);
}

void myTCPClient::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<this->errorString();
    QMessageBox::information(NULL, "client", this->errorString());
}

void myTCPClient::startPVPOnlineGame()
{
    QMessageBox::information(NULL, "client", "Start playing chess online!");

}
