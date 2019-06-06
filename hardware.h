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

    bool operator==(const Hardware& rhs);
    bool operator!=(const Hardware& rhs);
};

class HardwareList {
public:
    static bool get_current(Hardware &hardware);
    static Hardware fromJson(const QString &text);
    static Hardware load();
private:

};

#endif // HARDWARE_H
