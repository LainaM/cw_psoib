#ifndef WHITELIST_H
#define WHITELIST_H

#include <QDialog>

namespace Ui {
class WhiteList;
}

class WhiteList : public QDialog
{
    Q_OBJECT

public:
    explicit WhiteList(QWidget *parent = nullptr);
    ~WhiteList();

private:
    Ui::WhiteList *ui;
};

#endif // WHITELIST_H
