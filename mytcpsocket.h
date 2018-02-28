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
    QString getMsg(){return msg;}
public slots:
   bool newConnect();
   void myClose();
   void sendMessage(QString msg);
   void readMessage();
   void startPVPOnlineGame();
private:
   QString msg;
   //QString localHostName;
   //quint16 port;
};



#endif // MYTCPSOCKET_H
