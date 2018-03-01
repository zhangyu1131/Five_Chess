#include "netsetupdialog.h"
#include "ui_netsetupdialog.h"
#include<QDebug>
#include<QMessageBox>
#include<QStringList>
#include<exception>

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
        black=true;
        qDebug()<<"host";
        tcpServer=new QTcpServer(this);
        tcpclient=new QTcpSocket(this);
        int port=ui->NET_PORT_EDIT->text().toInt();
        //监听端口
        if(!tcpServer->listen(QHostAddress::LocalHost,port))
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
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(startPVPOnlineGame_Server()));
        //connect(tcpServer->clientConnection,SIGNAL(readyRead()),this,SLOT(readMessage_Server()));
    }
    else if(hostOrClient->checkedId()==1)//client
    {
        black=false;
        qDebug()<<"client";
        tcpclient=new QTcpSocket(this);
        chessFlag=false;
        //发起连接
        blockSize=0;
        //取消现有连接
        tcpclient->abort();

        int port=ui->NET_PORT_EDIT->text().toInt();
        //判断ip地址是否正确
        QString ip=ui->IP_ADDRESS_EDIT->text();
        QStringList list=ip.split('.');
        if(list.size()!=4)
        {
            QMessageBox::information(NULL, "client", "please input correct ip address");
            return;
        }
        for(QList<QString>::Iterator it = list.begin();it!=list.end();it++)
        {
            try{
                int num=(*it).toInt();
                if(num<0||num>255)
                {
                    QMessageBox::information(NULL, "client", "please input correct ip address");
                    return;
                }
            }catch(QString)
            {
                QMessageBox::information(NULL, "client", "please input correct ip address");
                return;
            }
        }
        btnValue=QMessageBox::information(NULL, "client", "choose client successfully!");
        if (btnValue == QMessageBox::Ok)
            this->close();
        tcpclient->connectToHost(ip,port);
        if(!tcpclient->waitForConnected(3000))
        {
            QMessageBox::information(NULL, "server", "wait too long, connection fails.");
            return;
        }
        connect(tcpclient,SIGNAL(readyRead()),this,SLOT(readMessage()));
        connect(tcpclient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
        pvpOnlineGameStatus=true;

        QMessageBox::information(NULL, "client", "Start playing chess online!");
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

void NetSetupDialog::sendMessage(QString msg)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_0);
    out<<(quint16)0;
    out<<msg;
    out.device()->seek(0);
    out<<(quint16)(block.size()-sizeof(quint16));

    tcpclient->write(block);
}

void NetSetupDialog::readMessage()
{
    QDataStream in(tcpclient);
    in.setVersion (QDataStream::Qt_4_0);
    //如果刚开始接受，即blocksize==0
    if(blockSize==0)
    {
        //如果消息小于两个字节，说明消息有错
        if(tcpclient->bytesAvailable()<(int)sizeof(quint16))
            return;
        in>>blockSize;
    }
    //没有接收到全部数据也return
    if(tcpclient->bytesAvailable()<blockSize)
        return;
    in>>msg;
    qDebug() << "ser message" << msg;
    blockSize=0;

    int color=0,x=0,y=0;
    QStringList msgs=msg.split('$');
    color=msgs[0].toInt();
    x=msgs[1].toInt();
    y=msgs[2].toInt();
    if(color==1)
        game->gameMapVec[x][y]=1;
    else if(color==2)
        game->gameMapVec[x][y]=-1;
    chessFlag=!chessFlag;
}


void NetSetupDialog::startPVPOnlineGame_Server()
{
    tcpclient=tcpServer->nextPendingConnection();
    blockSize=0;
    if(!tcpclient->waitForConnected())
        return;
    connect(tcpclient,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpclient,SIGNAL(disconnected()),tcpclient,SLOT(deleteLater()));
    connect(tcpclient, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(displayError(QAbstractSocket::SocketError)));
    pvpOnlineGameStatus=true;
    game->gameStatus=PLAYING;
    //QMessageBox::information(NULL, "server", QString(clientConnection->state()));
    QMessageBox::information(NULL, "server", "Start playing chess online!");

    //tcpServer->startPVPOnlineGame();
}

void NetSetupDialog::displayError(QAbstractSocket::SocketError)
{
    if(black)
        QMessageBox::information(NULL, "server", "The other side has left the room, please reconnect for another game.");
    else
        QMessageBox::information(NULL, "client", "The other side has left the room, please reconnect for another game.");
    closeConnection();
    game->gameStatus=CLOSED;
    game->clearMap();
}
//获得落子信息，组装成包发送出去
void NetSetupDialog::getChessInfo(int x, int y)
{
    if(hostOrClient->checkedId()==0)//host，黑棋发送消息
    {
        QString msg="1$"+QString::number(x,10)+"$"+QString::number(y,10);
        chessFlag=!chessFlag;
        sendMessage(msg);
    }
    else if(hostOrClient->checkedId()==1)//client，白棋发送消息
    {
        QString msg="2$"+QString::number(x,10)+"$"+QString::number(y,10);
        chessFlag=!chessFlag;
        sendMessage(msg);
    }
    update();
}


void NetSetupDialog::closeConnection()
{
    if(black)
        tcpServer->close();
    tcpclient->close();
}
