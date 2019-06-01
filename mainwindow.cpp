#include "generatehl.h"
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
   HardwareList::Generate();
   /*QString strCommand = "/usr/bin/inxi";
   QStringList args;
   args << "-ADGNCM" <<"--usb" << "--color" << "0";
   QProcess process;
   process.start(strCommand, args);
   if( !process.waitForStarted() || !process.waitForFinished() ) {
           return;
       }

   QString output;
   if (process.readAllStandardError().size() != 0) {
        qDebug() << process.readAllStandardError();
        qDebug() << "Ошибка";
   }

   output = process.readAllStandardOutput();
   QFile hardlist("/home/sunny/Applications/Qt/course_work/resources/hardlist.txt");
   hardlist.open(QIODevice::WriteOnly);
   QTextStream hardliststream(&hardlist);
   hardliststream << output;
   hardlist.close();*/

}
