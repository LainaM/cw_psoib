#include "whitelist.h"
#include "ui_whitelist.h"

WhiteList::WhiteList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WhiteList)
{
    ui->setupUi(this);
}

WhiteList::~WhiteList()
{
    delete ui;
}
