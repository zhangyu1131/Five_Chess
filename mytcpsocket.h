#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include<QtNetwork>

class myTCPSocket: public QTcpServer
{
public:
    explicit myTCPSocket(QObject *parent = 0);
    ~myTCPSocket(){}
    //QString getHostName();
    //quint16 getPort();
public slots:
   bool newConnect();
   void myClose();
   void sendMessage();
private:
   //QString localHostName;
   //quint16 port;
};



#endif // MYTCPSOCKET_H
