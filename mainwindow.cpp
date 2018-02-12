#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限

const int kAIDelay = 700; // AI下棋的思考时间

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setFixedSize(kBoardMargin * 2 + kBlockSize * 14, kBoardMargin * 3 + kBlockSize * 14);
    ui->setupUi(this);
    setMouseTracking(true);//开启跟踪鼠标

    game=new GameState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//PVP选择
void MainWindow::on_actionPVP_triggered()
{
    qDebug()<<"in new game triggered";
    netsetupdialog=new NetSetupDialog();
    netsetupdialog->setModal(true);
    netsetupdialog->setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    netsetupdialog->setFixedSize(400,300);
    netsetupdialog->show();
}
/*
//新游戏按钮响应，调用网络设置对话框
void MainWindow::on_NEW_GAME_MENU_triggered()
{

}*/
//和棋请求函数
void MainWindow::on_DRAW_GAME_MENU_triggered()
{
     qDebug()<<"in draw game triggered";
}
//退出游戏函数
void MainWindow::on_EXIT_GAME_MENU_triggered()
{
     qDebug()<<"in exit game triggered";
     this->close();
}

//鼠标按键响应
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()&Qt::LeftButton)//按下左键
    {
        qDebug()<<event->localPos();
        int x=event->x();
        int y=event->y();
        if(x<50||x>700||y<205||y>770)
            return;
        int j=(int)((x-50)/46.42857+0.5);
        int i=(int)((y-200)/40.35714+0.5);
        game->gameMapVec[i][j]=1;
        qDebug()<<i<<" "<<j;
        chessPoint.setX(event->x());
        chessPoint.setY(event->y());
        //更新显示
        update();
    }
}

//落子绘制棋子
void MainWindow::paintEvent(QPaintEvent *event)
{
    qDebug()<<"in paintEvent";

    QPainter painter(this);
    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    for (int i = 0; i < 15; i++)
    {
        painter.drawLine(kBoardMargin,2*kBoardMargin+kBlockSize*i,size().width()-kBoardMargin,2*kBoardMargin+kBlockSize*i);
        painter.drawLine(kBoardMargin+kBlockSize*i,2*kBoardMargin,kBoardMargin+kBlockSize*i,size().height()-kBoardMargin);
        //painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
        //painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
    }
    /*QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);
    painter.setBrush(brush);
    painter.drawEllipse(12,12,150,150);*/


    // 绘制棋子
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
        {
            if (game->gameMapVec[i][j] == 1)
            {
                int x=(i*46.42857)+50;
                int y=(j*40.35714)+200;
                QPoint point(x,y);
            }
            /*else if (game->gameMapVec[i][j] == -1)
            {
                int x=(i*46.42857)+50;
                int y=(j*40.35714)+200;
                QPoint point(x,y);
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(point,15,15);
            }*/
        }
}
