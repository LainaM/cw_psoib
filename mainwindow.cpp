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
    ui->pushButton->setVisible(false);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
    on_configButton_clicked();
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
   Hardware current_hardware;
   if (!HardwareList::get_current(current_hardware)){
       return;
   }
   Hardware saved_hardware = HardwareList::load();

   ui->treeWidget->clear();

   QTreeWidgetItem *cpu_itm = new QTreeWidgetItem();
   cpu_itm->setText(0, "Процессор");
   ui->treeWidget->setColumnCount(1);
   ui->treeWidget->addTopLevelItem(cpu_itm);
   QTreeWidgetItem *child = new QTreeWidgetItem();
   QString cpu = current_hardware.cpu;
   if (current_hardware.cpu != saved_hardware.cpu) {
       cpu += " (изменен)";
   }
    child->setText(0, cpu);
   cpu_itm->addChild(child);
   ui->treeWidget->expandItem(cpu_itm);

   add_elements("Видеоустройства", current_hardware.gpu, saved_hardware.gpu);
   add_elements("Жесткие диски", current_hardware.drive, saved_hardware.drive);
   add_elements("Съемные носители", current_hardware.usb, saved_hardware.usb);
   add_elements("Аудиоустройства", current_hardware.audio, saved_hardware.audio);
   add_elements("Сетевые устройства", current_hardware.network, saved_hardware.network);

   Hardware locked_hardware = HardwareList::load();

   if (current_hardware == locked_hardware) {
        ui->pushButton->setVisible(false);
   } else {
       ui->pushButton->setVisible(true);
    }
   }

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);
    QAction* addBL = new QAction(tr("Добавить в черный список"), this);
    QAction* addWL = new QAction(tr("Добавить в белый список"), this);
    connect(addBL, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Обработчик вызова диалога редактирования
    connect(addWL, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Обработчик удаления записи
    /* Устанавливаем действия в меню */
    menu->addAction(addBL);
    menu->addAction(addWL);
    menu->popup(ui->treeWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::on_pushButton_clicked()
{
    Hardware hardware;
    if (!HardwareList::get_current(hardware)){
        return;
    }

    hardware.save();
    on_configButton_clicked();
}


void MainWindow::add_elements(const QString &item_name, const QStringList &current, const QStringList &saved){
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, item_name);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->addTopLevelItem(itm);

    for (auto const &item : current){
        QTreeWidgetItem *child = new QTreeWidgetItem();
        QString element = item;
        if (!saved.contains(element)) {
            element += " (добавлено)";
            child->setTextColor(0, QColor(0,0,128,255));
        }
        child->setText(0, element);
        itm->addChild(child);
    }
    for (auto const &item : saved) {
        QString element = item;
        if (!current.contains(element)) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            element += " (удалено)";
            child->setText(0, element);
            child->setTextColor(0, QColor(200,0,100,255));
            itm->addChild(child);
        }
    }
    ui->treeWidget->expandItem(itm);
}
