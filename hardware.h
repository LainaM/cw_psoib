#ifndef HARDWARE_H
#define HARDWARE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QStringList>


struct Hardware{
    QString cpu;
    QStringList gpu;
    QStringList audio;
    QStringList network;
    QStringList drive;
    QStringList usb;

    QString to_string();
    QJsonObject toJson() const;
    void save();
};

class HardwareList {
public:
    static bool get_current(Hardware &hardware);
    static void Parse();
private:

};

#endif // HARDWARE_H
