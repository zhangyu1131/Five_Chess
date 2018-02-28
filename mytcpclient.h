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
    QString getMsg(){return msg;}
public slots:
   void newConnect();
   //void myClose();
   void readMessage();
   void sendMessage(QString msg);
   void displayError(QAbstractSocket::SocketError);
   void startPVPOnlineGame();
private:
   //QString localHostName;
   //quint16 port;
   QString msg;
   quint16 blockSize;
};
#endif // MYTCPCLIENT_H
