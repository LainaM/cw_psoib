#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hardware.h"

#include <QMainWindow>
#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();
    void on_configButton_clicked();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_pushButton_clicked();
    void addToBlackList();
    void delFromBlackList();
    void addToWhiteList();
    void delFromWhiteList();

private:
    Ui::MainWindow *ui;
    void add_elements(const QString &item_name, const QStringList &current, const QStringList &saved);

    const QStringList HARDWARE_ELEMENTS = {"Видеоустройства", "Процессор", "Жесткие диски", "Съемные носители",
                                           "Аудиоустройства", "Сетевые устройства"};

    void addElementByParent(const QString &parent, const QString &item, Hardware &hardware);
    void delElementByParent(const QString &parent, const QString &item, Hardware &hardware);
    bool inHardware(const QString &parent, const QString &item, Hardware &hardware);
    QPoint possion;
};

#endif // MAINWINDOW_H
