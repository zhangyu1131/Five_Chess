#ifndef NETSETUPDIALOG_H
#define NETSETUPDIALOG_H

#include <QDialog>

namespace Ui {
class NetSetupDialog;
}

class NetSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetSetupDialog(QWidget *parent = 0);
    ~NetSetupDialog();

private slots:
    void on_OK_clicked();

private:
    Ui::NetSetupDialog *ui;
};

#endif // NETSETUPDIALOG_H
