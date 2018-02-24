#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "netsetupdialog.h"
#include "gamestate.h"
#include<vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    // 监听鼠标移动情况，方便落子
    void mouseReleaseEvent(QMouseEvent* event);
    //监听鼠标移动
    void mouseMoveEvent(QMouseEvent * event);
    //绘制
    void paintEvent(QPaintEvent *event);

private slots:

    void on_DRAW_GAME_MENU_triggered();

    void on_EXIT_GAME_MENU_triggered();

    //三种模式的游戏初始化E
    void initPVPGame();
    void initPVPOnlineGame();
    void initPVEGame();

    void chessByAI();//电脑下棋
    void chessByPerson();//人下棋

private:
    void ConstructMenu();

private:
    Ui::MainWindow *ui;
    NetSetupDialog* netsetupdialog;
    //存储鼠标落子位置
    QPoint chessPoint;
    //存储星位天元标记
    std::vector<QPoint> Tianyuan;
    //游戏状态对象
    GameState *game;
    //游戏模式
    GameModel game_model;

};

#endif // MAINWINDOW_H
