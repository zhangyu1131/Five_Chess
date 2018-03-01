#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include<vector>
#include"gamestate.h"
#include <QTimer>
#include<QMessageBox>


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

    game=new GameState();
    game->gameStatus=CLOSED;
    initPVPGame();
}



MainWindow::~MainWindow()
{
    delete ui;
}

//游戏模式选择
void MainWindow::initPVPGame()
{
    if(game->gameModel==PVPONLINE&&game->gameStatus==PLAYING)
        netsetupdialog->closeConnection();
    game->gameModel=PVP;
    game->gameStatus=CLOSED;
    game->StartGame();
    qDebug()<<"init pvp";
    update();
}
void MainWindow::initPVPOnlineGame()
{
    qDebug()<<"PVP online triggered";
    netsetupdialog=new NetSetupDialog(game);
    netsetupdialog->setModal(true);
    netsetupdialog->setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    netsetupdialog->setFixedSize(400,300);
    netsetupdialog->show();

    game->gameStatus=CLOSED;
    game->gameModel=PVPONLINE;
    game->StartGame();
    if(netsetupdialog->black)
        netsetupdialog->chessFlag=true;
    else
        netsetupdialog->chessFlag=false;
    qDebug()<<"init pvponline";
    update();
}
void MainWindow::initPVEGame()
{
    if(game->gameModel==PVPONLINE&&game->gameStatus==PLAYING)
        netsetupdialog->closeConnection();
    game->gameModel=PVE;
    game->gameStatus=CLOSED;
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
     if(game->gameModel==PVPONLINE)
         netsetupdialog->closeConnection();
     this->close();
}

//鼠标响应
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug()<<"in mouse move";
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
    //调整i,j，使i和j在0到15范围内，即落在棋盘内
    int i=(int)((x-30)/40.0+0.5);
    int j=(int)((y-60)/40.0+0.5);
    //qDebug()<<i<<" "<<j;
    chessPoint.setX(i);
    chessPoint.setY(j);
    update();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    if(game->gameStatus==PLAYING)
    {
        if(game->gameModel==PVP)
        {
            chessByPerson();
            chessByPerson();
        }
        else if(game->gameModel==PVE)
        {
            if(game->playerFlag)
                chessByPerson();
            if(!game->isWin(chessPoint.x(),chessPoint.y()))
                QTimer::singleShot(700, this, SLOT(chessByAI()));
        }
        else if(game->gameModel==PVPONLINE)
        {
            chessByPerson();
        }
    }
    update();
}

//绘制界面函数
void MainWindow::paintEvent(QPaintEvent *)
{
   // qDebug()<<"in paintEvent";

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
        game->gameMapVec[chessPoint.x()][chessPoint.y()]==0)
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

    //判断输赢
    if(game->gameMapVec[chessPoint.x()][chessPoint.y()]==1||game->gameMapVec[chessPoint.x()][chessPoint.y()]==-1)
    {
        //qDebug()<<"in judge";
        if(game->isWin(chessPoint.x(),chessPoint.y())&& game->gameStatus == PLAYING)
        {
            game->gameStatus=END;//结束
            QString str="";
            if(game->gameMapVec[chessPoint.x()][chessPoint.y()]==1)
                str="black";
            else if(game->gameMapVec[chessPoint.x()][chessPoint.y()]==-1)
                str="white";

            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + " win!");

            // 重置游戏状态，否则容易死循环
            if (btnValue == QMessageBox::Ok)
            {
                if(game->gameModel==PVP)
                    initPVPGame();
                else if(game->gameModel==PVE)
                    initPVEGame();
                else if(game->gameModel==PVPONLINE)
                {
                    //清空上一局棋盘
                    game->clearMap();
                    game->gameStatus=PLAYING;
                    if(netsetupdialog->getSide())//黑
                        netsetupdialog->chessFlag=true;
                    else
                        netsetupdialog->chessFlag=false;
                }
            }
        }
    }

}

void MainWindow::chessByPerson()
{
    if(game->gameModel!=PVPONLINE)
    {
        if(game->playerFlag&&game->gameMapVec[chessPoint.x()][chessPoint.y()]==0)
        {
            game->gameMapVec[chessPoint.x()][chessPoint.y()]=1;//黑棋
            game->playerFlag=!game->playerFlag;
        }
        else if(!game->playerFlag&&game->gameMapVec[chessPoint.x()][chessPoint.y()]==0)
        {
            game->gameMapVec[chessPoint.x()][chessPoint.y()]=-1;//白棋
            game->playerFlag=!game->playerFlag;
        }
    }
    else//pvp online 模式，操作与其他两种模式不同
    {
        if(netsetupdialog->pvpOnlineGameStatus&&netsetupdialog->chessFlag)
        {
            if(netsetupdialog->hostOrClient->checkedId()==0&&game->gameMapVec[chessPoint.x()][chessPoint.y()]==0)//host 黑棋
                game->gameMapVec[chessPoint.x()][chessPoint.y()]=1;
            else if(netsetupdialog->hostOrClient->checkedId()==1&&game->gameMapVec[chessPoint.x()][chessPoint.y()]==0)
                game->gameMapVec[chessPoint.x()][chessPoint.y()]=-1;//白棋
            netsetupdialog->getChessInfo(chessPoint.x(),chessPoint.y());
        }
    }
    update();
}

void MainWindow::chessByAI()
{
    chessPoint=game->putChess(chessPoint);
    update();
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
