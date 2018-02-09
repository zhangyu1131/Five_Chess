#include"mytcpclient.h"

myTCPClient::myTCPClient(QObject *parent)
{

}

void myTCPClient::newConnect()
{
    blockSize=0;
    //取消现有连接
    this->abort();
    this->connectToHost("127.0.0.1",5005);
    qDebug()<<"client send new connection.";
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
}

void myTCPClient::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<this->errorString();
}
