#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QPixmap p("C:/Users/zashr/OneDrive/Desktop/graph.dot.png");
    ui->label->setPixmap(p.scaled(200,200,Qt::KeepAspectRatio));
}

Dialog::~Dialog()
{
    delete ui;
}
