#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include<vector>
#include"gamestate.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),chessPoint(QPoint(7,7))
{
    Tianyuan.push_back(QPoint(3,3));
    Tianyuan.push_back(QPoint(3,11));
    Tianyuan.push_back(QPoint(7,7));
    Tianyuan.push_back(QPoint(11,11));
    Tianyuan.push_back(QPoint(11,3));
    setFixedSize(kBoardMargin * 2 + kBlockSize * 14, kBoardMargin * 3 + kBlockSize * 14);
    setMouseTracking(true);//开启跟踪鼠标

    //手动添加菜单
    ConstructMenu();

    //ui->setupUi(this);

    game=new GameState();
    initPVPGame();
}



MainWindow::~MainWindow()
{
    delete ui;
}

//游戏模式选择
void MainWindow::initPVPGame()
{
    game->gameModel=PVP;
    game->StartGame();
    qDebug()<<"init pvp";
    update();
}
void MainWindow::initPVPOnlineGame()
{
    qDebug()<<"PVP online triggered";
    netsetupdialog=new NetSetupDialog();
    netsetupdialog->setModal(true);
    netsetupdialog->setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    netsetupdialog->setFixedSize(400,300);
    netsetupdialog->show();

    game->gameModel=PVPONLINE;
    game->StartGame();
    qDebug()<<"init pvponline";
    update();
}
void MainWindow::initPVEGame()
{
    game->gameModel=PVE;
    game->StartGame();
    qDebug()<<"init pve";
    update();
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
     this->close();
}

//鼠标响应
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"in mouse move";
    int x=event->x();
    int y=event->y();
    //调整坐标，格子边长是40
    if(x<30)
        x=30;
    if(x>590)
        x=590;
    if(y<60)
        y=60;
    if(y>620)
        y=620;

    int i=(int)((x-30)/40.0+0.5);
    int j=(int)((y-60)/40.0+0.5);
    qDebug()<<i<<" "<<j;
    chessPoint.setX(i);
    chessPoint.setY(j);
    update();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(game->playerFlag&&game->gameMapVec[chessPoint.x()][chessPoint.y()]==0)
    {
        game->gameMapVec[chessPoint.x()][chessPoint.y()]=1;
        game->playerFlag=!game->playerFlag;
    }
    else if(!game->playerFlag&&game->gameMapVec[chessPoint.x()][chessPoint.y()]==0)
    {
        game->gameMapVec[chessPoint.x()][chessPoint.y()]=-1;
        game->playerFlag=!game->playerFlag;
    }
    update();
}

//绘制界面函数
void MainWindow::paintEvent(QPaintEvent *event)
{
    qDebug()<<"in paintEvent";

    QPainter painter(this);
    // 绘制棋盘和星位天元
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    for (int i = 0; i < 15; i++)
    {
        painter.drawLine(kBoardMargin,2*kBoardMargin+kBlockSize*i,size().width()-kBoardMargin,2*kBoardMargin+kBlockSize*i);
        painter.drawLine(kBoardMargin+kBlockSize*i,2*kBoardMargin,kBoardMargin+kBlockSize*i,size().height()-kBoardMargin);
    }
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    for(int i=0;i<5;i++)
        painter.drawRect(kBoardMargin + kBlockSize * Tianyuan[i].x() - kMarkSize / 2, 2*kBoardMargin + kBlockSize * Tianyuan[i].y() - kMarkSize / 2, kMarkSize, kMarkSize);

    // 绘制落子标记(防止鼠标出框越界)
    if (chessPoint.y() >= 0 && chessPoint.y() < 15 &&
        chessPoint.x() >= 0 && chessPoint.x() < 15 &&
        game->gameMapVec[chessPoint.y()][chessPoint.x()]==0)
    {
        if (game->playerFlag)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin + kBlockSize * chessPoint.x() - kMarkSize / 2, 2*kBoardMargin + kBlockSize * chessPoint.y() - kMarkSize / 2, kMarkSize, kMarkSize);
    }


    // 绘制棋子
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
        {
            if (game->gameMapVec[i][j] == 1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * i-kRadius, 2*kBoardMargin + kBlockSize * j - kRadius,2*kRadius,2*kRadius);
            }
            else if (game->gameMapVec[i][j] == -1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * i-kRadius, 2*kBoardMargin + kBlockSize * j - kRadius,2*kRadius,2*kRadius);
            }
        }
}


//私有实现函数
void MainWindow::ConstructMenu()
{
    //手动添加菜单
    QMenu *gameMenu = menuBar()->addMenu(tr("游戏")); // menuBar默认是存在的，直接加菜单就可以了
    QAction *exitGame=new QAction("退出游戏",this);

    QMenu *newGameMenu=gameMenu->addMenu("新游戏");
    QAction *actionPVP = new QAction("PVP", this);
    connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
    newGameMenu->addAction(actionPVP);

    QAction *actionPVPOnline = new QAction("PVP(Online)", this);
    connect(actionPVPOnline, SIGNAL(triggered()), this, SLOT(initPVPOnlineGame()));
    newGameMenu->addAction(actionPVPOnline);

    QAction *actionPVE = new QAction("PVE", this);
    connect(actionPVE, SIGNAL(triggered()), this, SLOT(initPVEGame()));
    newGameMenu->addAction(actionPVE);

    connect(exitGame,SIGNAL(triggered()),this,SLOT(on_EXIT_GAME_MENU_triggered()));
    gameMenu->addAction(exitGame);
}
