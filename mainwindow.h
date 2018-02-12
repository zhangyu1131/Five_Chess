#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "netsetupdialog.h"
#include"gamestate.h"

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
    void mousePressEvent(QMouseEvent *event);
    //绘制
    void paintEvent(QPaintEvent *event);

private slots:

    void on_DRAW_GAME_MENU_triggered();

    void on_EXIT_GAME_MENU_triggered();

    void on_actionPVP_triggered();

private:
    Ui::MainWindow *ui;
    NetSetupDialog* netsetupdialog;
    //存储鼠标点击位置
    QPoint chessPoint;
    //游戏状态对象
    GameState *game;
};

#endif // MAINWINDOW_H
