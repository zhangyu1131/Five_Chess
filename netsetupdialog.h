#ifndef NETSETUPDIALOG_H
#define NETSETUPDIALOG_H

#include <QDialog>
#include<QButtonGroup>
#include <QtNetwork>
#include"mytcpsocket.h"
#include"mytcpclient.h"
#include "gamestate.h"

namespace Ui {
class NetSetupDialog;
}

class NetSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetSetupDialog(GameState*& game, QWidget *parent = 0);
    ~NetSetupDialog();
    void getChessInfo(int x,int y);
private slots:
    void on_OK_clicked();

    void on_CANCEL_clicked();

    void on_HOSTORCLIENT_clicked();

    //void sendMessage();
    void readMessage_Client();
    void displayError_Client(QAbstractSocket::SocketError);
    void startPVPOnlineGame();

private:
    GameState* game;
    Ui::NetSetupDialog *ui;
    //tcpserver
    myTCPSocket* tcpServer;
    //tcpclient
    myTCPClient* tcpclient;
public:
    bool pvpOnlineGameStatus;
    //单选按钮组
    QButtonGroup * hostOrClient;
    //下棋轮次标志
    bool chessFlag;
};

#endif // NETSETUPDIALOG_H
