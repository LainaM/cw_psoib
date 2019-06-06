#include "blacklist.h"
#include "ui_blacklist.h"

BlackList::BlackList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackList)
{
    ui->setupUi(this);
}

BlackList::~BlackList()
{
    delete ui;
}
