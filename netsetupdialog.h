#ifndef NETSETUPDIALOG_H
#define NETSETUPDIALOG_H

#include <QDialog>
#include<QButtonGroup>
#include <QtNetwork>
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
    bool getSide(){return black;}
    void closeConnection();
private slots:
    void on_OK_clicked();

    void on_CANCEL_clicked();

    void on_HOSTORCLIENT_clicked();

    void sendMessage(QString msg);

    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void startPVPOnlineGame_Server();

private:
    GameState* game;
    Ui::NetSetupDialog *ui;
    //tcpserver
    QTcpServer* tcpServer;
    //tcpclient
    QTcpSocket* tcpclient;
    quint16 blockSize;
    QString msg;
public:
    bool pvpOnlineGameStatus;
    //单选按钮组
    QButtonGroup * hostOrClient;
    //下棋轮次标志
    bool chessFlag;
    //下棋方标志
    bool black;
};

#endif // NETSETUPDIALOG_H
