#include "hardware.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    QApplication::quit();
}

void MainWindow::on_configButton_clicked()
{
   Hardware hardware;
   if (!HardwareList::get_current(hardware)){
       return;
   }

   hardware.save();
}
