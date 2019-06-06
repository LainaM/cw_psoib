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
#include "blacklist.h"
#include "whitelist.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
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

   QTreeWidgetItem *cpu_itm = new QTreeWidgetItem();
   cpu_itm->setText(0, "Процессор");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(cpu_itm);
   QTreeWidgetItem *child = new QTreeWidgetItem();
   child->setText(0, hardware.cpu);
   cpu_itm->addChild(child);

   QTreeWidgetItem *gpu_itm = new QTreeWidgetItem();
   gpu_itm->setText(0, "Видеоустройства");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(gpu_itm);

   for (auto const &item : hardware.gpu){
       QTreeWidgetItem *child = new QTreeWidgetItem();
       child->setText(0, item);
       gpu_itm->addChild(child);
   }

   QTreeWidgetItem *drive_itm = new QTreeWidgetItem();
   drive_itm->setText(0, "Жесткие диски");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(drive_itm);

   for (auto const &item : hardware.drive){
       QTreeWidgetItem *child = new QTreeWidgetItem();
       child->setText(0, item);
       drive_itm->addChild(child);
   }

   QTreeWidgetItem *usb_itm = new QTreeWidgetItem();
   usb_itm->setText(0, "Съемные носители");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(usb_itm);

   for (auto const &item : hardware.usb){
       QTreeWidgetItem *child = new QTreeWidgetItem();
       child->setText(0, item);
       usb_itm->addChild(child);
   }

   QTreeWidgetItem *audio_itm = new QTreeWidgetItem();
   audio_itm->setText(0, "Аудиоустройства");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(audio_itm);

   for (auto const &item : hardware.audio){
       QTreeWidgetItem *child = new QTreeWidgetItem();
       child->setText(0, item);
       audio_itm->addChild(child);
   }

   QTreeWidgetItem *network_itm = new QTreeWidgetItem();
   network_itm->setText(0, "Сетевые устройства");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(network_itm);

   for (auto const &item : hardware.network){
       QTreeWidgetItem *child = new QTreeWidgetItem();
       child->setText(0, item);
       network_itm->addChild(child);
   }

   /*QFile hardlist(QDir::homePath() + "/.hardlist.txt");
   if ((hardlist.exists()) && (hardlist.open(QIODevice::ReadOnly))){
       QString str = "";
       while (!hardlist.atEnd()){
           str = str + hardlist.readLine();
       }*/


   }

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);
    QAction* addBL = new QAction(tr("Добавить в черный список"), this);
    QAction* addWL = new QAction(tr("Добавить в белый список"), this);
    connect(addBL, SIGNAL(triggered()), this, SLOT(slotAddBL()));
    connect(addWL, SIGNAL(triggered()), this, SLOT(slotAddWL()));
    menu->addAction(addBL);
    menu->addAction(addWL);
    menu->popup(ui->treeWidget->viewport()->mapToGlobal(pos));
}


void MainWindow::on_action_2_triggered()
{
    WhiteList whitelist;
    whitelist.setModal(true);
    whitelist.exec();
}

void MainWindow::on_action_3_triggered()
{
    BlackList blacklist;
    blacklist.setModal(true);
    blacklist.exec();
}
