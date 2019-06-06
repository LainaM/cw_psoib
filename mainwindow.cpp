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
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_treeWidget_customContextMenuRequested(QPoint)));
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
   Hardware saved_hardware = HardwareList::load_saved();
   Hardware black_list = HardwareList::load_black_list();
   Hardware white_list = HardwareList::load_white_list();

   ui->treeWidget->clear();

   add_elements("Процессор", current_hardware.cpu, saved_hardware.cpu, black_list.cpu, white_list.cpu);
   add_elements("Видеоустройства", current_hardware.gpu, saved_hardware.gpu, black_list.gpu, white_list.gpu);
   add_elements("Жесткие диски", current_hardware.drive, saved_hardware.drive, black_list.drive, white_list.drive);
   add_elements("Съемные носители", current_hardware.usb, saved_hardware.usb, black_list.usb, white_list.usb);
   add_elements("Аудиоустройства", current_hardware.audio, saved_hardware.audio, black_list.audio, white_list.audio);
   add_elements("Сетевые устройства", current_hardware.network, saved_hardware.network, black_list.network, white_list.network);

   if (current_hardware == saved_hardware) {
        ui->pushButton->setVisible(false);
   } else {
       ui->pushButton->setVisible(true);
    }
}


void MainWindow::addToBlackList()
{
    QTreeWidgetItem* item = ui->treeWidget->itemAt(possion);
    QString item_name = item->text(0).trimmed();
    if (HARDWARE_ELEMENTS.contains( item_name )) {
        return;
    }
    QString parent = item->parent()->text(0).trimmed();
    Hardware black_list = HardwareList::load_black_list();
    addElementByParent(parent, item_name, black_list);
    black_list.save_as_black();
    return;
}

void MainWindow::delFromBlackList()
{
    QTreeWidgetItem* item = ui->treeWidget->itemAt(possion);
    QString item_name = item->text(0).trimmed();
    if (HARDWARE_ELEMENTS.contains( item_name )) {
        return;
    }
    QString parent = item->parent()->text(0).trimmed();
    Hardware black_list = HardwareList::load_black_list();
    delElementByParent(parent, item_name, black_list);
    black_list.save_as_black();
    return;
}

void MainWindow::addToWhiteList()
{
    QTreeWidgetItem* item = ui->treeWidget->itemAt(possion);
    QString item_name = item->text(0).trimmed();
    if (HARDWARE_ELEMENTS.contains( item_name )) {
        return;
    }
    QString parent = item->parent()->text(0).trimmed();
    Hardware white_list = HardwareList::load_white_list();
    addElementByParent(parent, item_name, white_list);
    white_list.save_as_white();
    return;
}

void MainWindow::delFromWhiteList()
{
    QTreeWidgetItem* item = ui->treeWidget->itemAt(possion);
    QString item_name = item->text(0).trimmed();
    if (HARDWARE_ELEMENTS.contains( item_name )) {
        return;
    }
    QString parent = item->parent()->text(0).trimmed();
    Hardware white_list = HardwareList::load_white_list();
    delElementByParent(parent, item_name, white_list);
    white_list.save_as_white();
    return;
}


void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    possion = pos;
    QTreeWidgetItem* item = ui->treeWidget->itemAt(pos);
    QString item_name = cleanItem(item->text(0).trimmed());
    if (HARDWARE_ELEMENTS.contains( item_name )) {
        return;
    }
    QString parent = item->parent()->text(0).trimmed();
    Hardware black_list = HardwareList::load_black_list();
    Hardware white_list = HardwareList::load_white_list();
    QMenu* menu = new QMenu(this);

    QAction* addBL = new QAction(tr("Добавить в черный список"), this);
    QAction* delBL = new QAction(tr("Удалить из черного списока"), this);
    QAction* addWL = new QAction(tr("Добавить в белый список"), this);
    QAction* delWL = new QAction(tr("Удалить из белого списка"), this);

    connect(addBL, SIGNAL(triggered()), this, SLOT(addToBlackList()));
    connect(addWL, SIGNAL(triggered()), this, SLOT(addToWhiteList()));
    connect(delBL, SIGNAL(triggered()), this, SLOT(delFromBlackList()));
    connect(delWL, SIGNAL(triggered()), this, SLOT(delFromWhiteList()));

    QList<QAction *> acts;

    if (inHardware(parent, item_name, black_list)) {
        acts.append(delBL);
    } else if (inHardware(parent, item_name, white_list)) {
        acts.append(delWL);
    } else {
        acts.append(addBL);
        acts.append(addWL);
    }

    menu->addActions(acts);
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

void MainWindow::addElementByParent(const QString &parent, const QString &item, Hardware &hardware)
{
    QString clened_item = cleanItem(item);
    if (parent == "Процессор") {
        hardware.cpu.append(clened_item);
    } else if (parent == "Видеоустройства") {
        hardware.gpu.append(clened_item);
    } else if (parent == "Аудиоустройства") {
        hardware.audio.append(clened_item);
    } else if (parent == "Жесткие диски") {
        hardware.drive.append(clened_item);
    } else if (parent == "Съемные носители") {
        hardware.usb.append(clened_item);
    } else if (parent == "Сетевые устройства") {
        hardware.network.append(clened_item);
    }
}

void MainWindow::delElementByParent(const QString &parent, const QString &item, Hardware &hardware)
{
    QString clened_item = cleanItem(item);
    if (parent == "Процессор") {
        hardware.cpu.removeAll(clened_item);
    } else if (parent == "Видеоустройства") {
        hardware.gpu.removeAll(clened_item);
    } else if (parent == "Аудиоустройства") {
        hardware.audio.removeAll(clened_item);
    } else if (parent == "Жесткие диски") {
        hardware.drive.removeAll(clened_item);
    } else if (parent == "Съемные носители") {
        hardware.usb.removeAll(clened_item);
    } else if (parent == "Сетевые устройства") {
        hardware.network.removeAll(clened_item);
    }
}

bool MainWindow::inHardware(const QString &parent, const QString &item, Hardware &hardware)
{
    QString clened_item = cleanItem(item);
    if (parent == "Процессор") {
        return hardware.cpu.contains(item);
    } else if (parent == "Видеоустройства") {
        return hardware.gpu.contains(item);
    } else if (parent == "Аудиоустройства") {
        return hardware.audio.contains(item);
    } else if (parent == "Жесткие диски") {
        return hardware.drive.contains(item);
    } else if (parent == "Съемные носители") {
        return hardware.usb.contains(item);
    } else if (parent == "Сетевые устройства") {
        return hardware.network.contains(item);
    }
    throw std::runtime_error("WRONG");
}

QString MainWindow::cleanItem(QString item)
{
    item = item.replace(" (добавлено)", "");
    item = item.replace(" в черном списке", "");
    item = item.replace(" не в белом списке", "");
    item = item.replace(" (удалено)", "");
    return item.trimmed();
}


void MainWindow::add_elements(const QString &item_name, const QStringList &current, const QStringList &saved, const QStringList &bl, const QStringList &wl){
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, item_name);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->addTopLevelItem(itm);

    for (auto const &item : current){
        QTreeWidgetItem *child = new QTreeWidgetItem();
        QString element = item.trimmed();
        if (!saved.contains(item)) {
            qDebug() << item;
            element += " (добавлено)";
            child->setTextColor(0, QColor(0,0,128,255));
        }
        if (bl.contains(item.trimmed())) {
            element += " в черном списке";
        }
        if (!wl.isEmpty() && !wl.contains(item.trimmed())) {
            element += " не в белом списке";
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
            child->setTextColor(0, QColor(255,0,0,255));
            itm->addChild(child);
        }
    }
    ui->treeWidget->expandItem(itm);
}
