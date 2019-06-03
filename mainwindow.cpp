#include "hardware.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->clear();
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

   QTreeWidgetItem *itm = new QTreeWidgetItem();
   itm->setText(0, "Процессор");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(itm);

   QTreeWidgetItem *itm2 = new QTreeWidgetItem();
   itm2->setText(0, "Видеоустройства");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(itm2);

   QTreeWidgetItem *itm3 = new QTreeWidgetItem();
   itm3->setText(0, "Жесткие диски");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(itm3);

   QTreeWidgetItem *itm4 = new QTreeWidgetItem();
   itm4->setText(0, "Съемные носители");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(itm4);

   QTreeWidgetItem *itm5 = new QTreeWidgetItem();
   itm5->setText(0, "Аудиоустройства");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(itm5);

   QTreeWidgetItem *itm6 = new QTreeWidgetItem();
   itm6->setText(0, "Сетевые устройства");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(itm6);


   /*QFile hardlist(QDir::homePath() + "/.hardlist.txt");
   if ((hardlist.exists()) && (hardlist.open(QIODevice::ReadOnly))){
       QString str = "";
       while (!hardlist.atEnd()){
           str = str + hardlist.readLine();
       }*/


   }
