#ifndef HARDWARE_H
#define HARDWARE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QStringList>


struct Hardware{
    QStringList cpu;
    QStringList gpu;
    QStringList audio;
    QStringList network;
    QStringList drive;
    QStringList usb;

    QString to_string();
    QJsonObject toJson() const;
    bool isEmpty() const;
    void save();
    void save_as_black();
    void save_as_white();

    bool operator==(const Hardware& rhs);
    bool operator!=(const Hardware& rhs);
};

class HardwareList {
public:
    static bool get_current(Hardware &hardware);
    static Hardware fromJson(const QString &text);
    static Hardware load_saved();
    static Hardware load_black_list();
    static Hardware load_white_list();
private:

};

#endif // HARDWARE_H
