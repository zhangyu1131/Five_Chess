#include "netsetupdialog.h"
#include "ui_netsetupdialog.h"
#include<QDebug>

NetSetupDialog::NetSetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetSetupDialog)
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
    if(hostOrClient->checkedId()==0)//host
    {
        qDebug()<<"host";
        tcpServer=new myTCPSocket(this);
        //监听端口
        if(!tcpServer->listen(QHostAddress::LocalHost,5005))
        {
            qDebug()<<tcpServer->errorString();
            close();
        }
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(sendMessage()));
    }
    else if(hostOrClient->checkedId()==1)//client
    {
        qDebug()<<"client";
        tcpclient=new myTCPClient(this);
        connect(tcpclient,SIGNAL(readyRead()),this,SLOT(readMessage()));
        connect(tcpclient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
        //发起连接
        tcpclient->newConnect();

    }


}
//取消响应函数
void NetSetupDialog::on_CANCEL_clicked()
{

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

void NetSetupDialog::sendMessage()
{
    tcpServer->sendMessage();
}

void NetSetupDialog::readMessage()
{
    tcpclient->readMessage();
}


void NetSetupDialog::displayError(QAbstractSocket::SocketError)
{
    tcpclient->displayError(QAbstractSocket::SocketError());
}
