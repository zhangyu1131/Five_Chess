#include "netsetupdialog.h"
#include "ui_netsetupdialog.h"
#include<QDebug>
#include<QMessageBox>

NetSetupDialog::NetSetupDialog(GameState*& game,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetSetupDialog),pvpOnlineGameStatus(false),game(game)
{
    ui->setupUi(this);
    //将主机和客户机两个单选框加入同一个组中
    hostOrClient=new QButtonGroup(this);
    hostOrClient->addButton(ui->HOST_OPTION,0);
    hostOrClient->addButton(ui->CLIENT_OPTION,1);
    //单选框绑定槽函数
    connect(ui->HOST_OPTION,SIGNAL(clicked()),this,SLOT(on_HOSTORCLIENT_clicked()));
    connect(ui->CLIENT_OPTION,SIGNAL(clicked()),this,SLOT(on_HOSTORCLIENT_clicked()));

    //IP输入框设置初值
    ui->IP_ADDRESS_EDIT->setText("127.0.0.1");
    //利用正则表达式使port输入框只接受数字
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->NET_PORT_EDIT );
    ui->NET_PORT_EDIT->setValidator( validator );
    ui->NET_PORT_EDIT->setText("5005");

}

NetSetupDialog::~NetSetupDialog()
{
    delete ui;
}
//确认响应函数
void NetSetupDialog::on_OK_clicked()
{
    qDebug()<< "in ok button";
    QMessageBox::StandardButton btnValue;
    if(hostOrClient->checkedId()==0)//host
    {
        qDebug()<<"host";
        tcpServer=new myTCPSocket(this);
        //监听端口
        if(!tcpServer->listen(QHostAddress::LocalHost,5005))
        {
            qDebug()<<tcpServer->errorString();
            QMessageBox::information(NULL, "server", "listen error");
            close();
        }
        else
            btnValue=QMessageBox::information(NULL, "server", "choose host and listen successfully!");

        if (btnValue == QMessageBox::Ok)
            this->close();
        chessFlag=true;
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(startPVPOnlineGame()));
    }
    else if(hostOrClient->checkedId()==1)//client
    {
        qDebug()<<"client";
        tcpclient=new myTCPClient(this);
        connect(tcpclient,SIGNAL(readyRead()),this,SLOT(readMessage_Client()));
        connect(tcpclient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError_Client(QAbstractSocket::SocketError)));

        btnValue=QMessageBox::information(NULL, "client", "choose client successfully!");
        if (btnValue == QMessageBox::Ok)
            this->close();

        chessFlag=false;
        //发起连接
        tcpclient->newConnect();
        pvpOnlineGameStatus=true;
    }
}
//取消响应函数
void NetSetupDialog::on_CANCEL_clicked()
{
    this->close();
}
//单选框响应函数
void NetSetupDialog::on_HOSTORCLIENT_clicked()
{
    if(hostOrClient->checkedId()==0)//host
    {
        ui->IP_ADDRESS_EDIT->setEnabled(false);
    }
    else if(hostOrClient->checkedId()==1)
    {
        ui->IP_ADDRESS_EDIT->setEnabled(true);
    }

}

/*void NetSetupDialog::sendMessage()
{
    tcpServer->sendMessage();
}*/

void NetSetupDialog::readMessage_Client()
{
    tcpclient->readMessage();
    QString msg=tcpclient->getMsg();
    int color=0,x=0,y=0;
    int i=0;
    for(;i<msg.size();i++)
        if(i=='$')
            break;
    QStringList msgs=msg.split('$');
    color=msgs[0].toInt();
    x=msgs[1].toInt();
    y=msgs[2].toInt();
    if(color==1)
        game->gameMapVec[x][y]=1;
    chessFlag=!chessFlag;
    update();
}

void NetSetupDialog::startPVPOnlineGame()
{
    tcpServer->startPVPOnlineGame();
    pvpOnlineGameStatus=true;
}

void NetSetupDialog::displayError_Client(QAbstractSocket::SocketError)
{
    tcpclient->displayError(QAbstractSocket::SocketError());
}

//获得落子信息，组装成包发送出去
void NetSetupDialog::getChessInfo(int x, int y)
{
    if(hostOrClient->checkedId()==0)//host，黑棋发送消息
    {
        QString msg="1$"+QString::number(x,10)+"$"+QString::number(y,10);
        chessFlag=!chessFlag;
        tcpServer->sendMessage(msg);
    }
    else if(hostOrClient->checkedId()==1)//client，白棋发送消息
    {
        QString msg="2$"+QString::number(x,10)+"$"+QString::number(y,10);
        chessFlag=!chessFlag;
    }
}
