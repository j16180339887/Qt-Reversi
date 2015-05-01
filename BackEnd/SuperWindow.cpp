#include "SuperWindow.h"
#include "ui_SuperWindow.h"

SuperWindow::SuperWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SuperWindow)
{
    ui->setupUi(this);
}

SuperWindow::~SuperWindow()
{
    delete ui;
}
