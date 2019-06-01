#include "generatehl.h"
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


struct Hardware{
    QString cpu;
    QStringList gpu;
    QString audio;
    QStringList usb;

    QString to_string(){
        QString text;
        text = cpu + "\n" + audio + "\n";
        text += gpu.join("\n");
        text += usb.join("\n");
        return text;
    }
    QJsonObject toJson() const {
        return {
            {"cpu", cpu},
            {"gpu", QJsonArray::fromStringList(gpu)},
            {"audio", audio},
            {"usb", QJsonArray::fromStringList(usb)}
        };
    };
};


void HardwareList::Generate()
{
    QString strCommand = "/usr/bin/inxi";
    QStringList args;
    args << "-ADGNCM" <<"--usb" << "--color" << "0";
    QProcess process;
    process.start(strCommand, args);
    if( !process.waitForStarted() || !process.waitForFinished() ) {
        return;
    }

    QString output;
    QString error = process.readAllStandardError();
    if (!error.isEmpty()) {
        qDebug() << "Ошибка";
        qDebug() << error;
        qDebug() << process.readAllStandardOutput();
    }

    output = process.readAllStandardOutput();

    Hardware hardware;

    QRegularExpression reg_cpu("model: (?<cpu>[^>]+) bits");
    QRegularExpressionMatch match_cpu = reg_cpu.match(output);
    if (match_cpu.hasMatch()) {
        hardware.cpu = match_cpu.captured("cpu");
    }

    QRegularExpression reg_gpu1("Device\\-1: (?<graphics>[^>]+) Graphics");
    QRegularExpressionMatch match_gpu1 = reg_gpu1.match(output);
    if (match_gpu1.hasMatch()) {
        hardware.gpu.append(match_gpu1.captured("graphics"));
    }

    QRegularExpression reg_gpu2("Device\\-2: (?<graphics>[^>]+)]");
    QRegularExpressionMatch match_gpu2 = reg_gpu2.match(output);
    if (match_gpu2.hasMatch()) {
        hardware.gpu.append(match_gpu2.captured("graphics"));
    }

    QRegularExpression reg_audio("Device\\-1: (?<audio>[^>]+) Audio driver:");
    QRegularExpressionMatch match_audio = reg_audio.match(output);
    if (match_audio.hasMatch()) {
        hardware.audio = match_audio.captured("audio");
    }

//    QRegularExpression reg_usb("Device\\-[\\d]: (?<hub>[^>]+)");
//    QRegularExpressionMatch match_usb = reg_usb.match(output);
//    if (match_usb.hasMatch()) {
//        hardware.usb.append(match_usb.captured("hub"));
//    }

    qDebug().noquote() << hardware.to_string();

    QFile hardlist(QDir::homePath() + "/.hardlist.txt");
    hardlist.open(QIODevice::WriteOnly);
    QTextStream hardliststream(&hardlist);
    hardliststream << QJsonDocument(hardware.toJson()).toJson();
    hardlist.close();
}
