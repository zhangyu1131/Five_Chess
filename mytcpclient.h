#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H


#include<QtNetwork>
#include<QAbstractSocket>

class myTCPClient: public QTcpSocket
{
public:
    explicit myTCPClient(QObject *parent = 0);
    ~myTCPClient(){}
    //QString getHostName();
    //quint16 getPort();
public slots:
   void newConnect();
   //void myClose();
   void readMessage();
   void displayError(QAbstractSocket::SocketError);
private:
   //QString localHostName;
   //quint16 port;
   QString msg;
   quint16 blockSize;
};
#endif // MYTCPCLIENT_H
