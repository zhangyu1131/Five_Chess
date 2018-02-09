#ifndef NETSETUPDIALOG_H
#define NETSETUPDIALOG_H

#include <QDialog>
#include<QButtonGroup>
#include <QtNetwork>
#include"mytcpsocket.h"
#include"mytcpclient.h"

namespace Ui {
class NetSetupDialog;
}

class NetSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetSetupDialog(QWidget *parent = 0);
    ~NetSetupDialog();

private slots:
    void on_OK_clicked();

    void on_CANCEL_clicked();

    void on_HOSTORCLIENT_clicked();

    void sendMessage();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);

private:
    Ui::NetSetupDialog *ui;
    //单选按钮组
    QButtonGroup * hostOrClient;
    //tcpserver
    myTCPSocket* tcpServer;
    //tcpclient
    myTCPClient* tcpclient;
};

#endif // NETSETUPDIALOG_H
