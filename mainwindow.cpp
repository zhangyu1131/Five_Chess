#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

//新游戏按钮响应，调用网络设置对话框
void MainWindow::on_NEW_GAME_MENU_triggered()
{
    qDebug()<<"in new game triggered";
    netsetupdialog=new NetSetupDialog(this);
    netsetupdialog->setModal(true);
    netsetupdialog->setFixedSize(400,300);
    netsetupdialog->show();
}
//和棋请求函数
void MainWindow::on_DRAW_GAME_MENU_triggered()
{
     qDebug()<<"in draw game triggered";
}
//退出游戏函数
void MainWindow::on_EXIT_GAME_MENU_triggered()
{
     qDebug()<<"in exit game triggered";
}
