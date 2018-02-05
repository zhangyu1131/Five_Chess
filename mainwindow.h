#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "netsetupdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_NEW_GAME_MENU_triggered();

    void on_DRAW_GAME_MENU_triggered();

    void on_EXIT_GAME_MENU_triggered();

private:
    Ui::MainWindow *ui;
    NetSetupDialog* netsetupdialog;
};

#endif // MAINWINDOW_H
