#ifndef BLACKLIST_H
#define BLACKLIST_H

#include <QDialog>

namespace Ui {
class BlackList;
}

class BlackList : public QDialog
{
    Q_OBJECT

public:
    explicit BlackList(QWidget *parent = nullptr);
    ~BlackList();

private:
    Ui::BlackList *ui;
};

#endif // BLACKLIST_H
