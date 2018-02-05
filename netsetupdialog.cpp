#include "netsetupdialog.h"
#include "ui_netsetupdialog.h"
#include<QDebug>

NetSetupDialog::NetSetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetSetupDialog)
{
    ui->setupUi(this);
}

NetSetupDialog::~NetSetupDialog()
{
    delete ui;
}

void NetSetupDialog::on_OK_clicked()
{
    qDebug()<< ui->IP_ADDRESS_EDIT->text();
}
