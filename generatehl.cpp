#include "generatehl.h"
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDir>

struct Hardware{
    QString cpu;
    QString gpu1;
    QString gpu2;
    QString audio;
    QStringList usb;

    QString to_string(){
        return cpu + "\n" + gpu1 + "\n" + "\n" + gpu2 + "]" + "\n" + audio + "\n";
    }
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
    qDebug() << "Yes match:" << match_cpu.hasMatch();
    if (match_cpu.hasMatch()) {
        hardware.cpu = match_cpu.captured("cpu");
        qDebug() << "Matched CPU" << hardware.cpu;
    }

    QRegularExpression reg_gpu1("Device\\-1: (?<graphics>[^>]+) Graphics");
    QRegularExpressionMatch match_gpu1 = reg_gpu1.match(output);
    qDebug() << "Yes match" << match_gpu1.hasMatch();
    if (match_gpu1.hasMatch()) {
        hardware.gpu1 = match_gpu1.captured("graphics");
        qDebug() << "Matched GPU1" << hardware.gpu1;
    }

    QRegularExpression reg_gpu2("Device\\-2: (?<graphics>[^>]+)]");
    QRegularExpressionMatch match_gpu2 = reg_gpu2.match(output);
    qDebug() << "Yes match" << match_gpu2.hasMatch();
    if (match_gpu2.hasMatch()) {
        hardware.gpu2 = match_gpu2.captured("graphics");
        qDebug() << "Matched GPU2" << hardware.gpu2;
    }

    QRegularExpression reg_audio("Device\\-1: (?<audio>[^>]+) Audio driver:");
    QRegularExpressionMatch match_audio = reg_audio.match(output);
    qDebug() << "Yes match:" << match_audio.hasMatch();
    if (match_audio.hasMatch()) {
        hardware.audio = match_audio.captured("audio");
        qDebug() << "Matched Audio" << hardware.audio;
    }

    QRegularExpression reg_usb("Device\\-[\\d]: (?<hub>[^>]+)");
    QRegularExpressionMatch match_usb = reg_usb.match(output);
    qDebug() << "Yes match" << match_usb.hasMatch();
    if (match_usb.hasMatch()) {
        hardware.usb.append(match_usb.captured("hub"));
        qDebug() << "Matched USB" << hardware.usb;
    }

    QFile hardlist(QDir::homePath() + "/.hardlist.txt");
    hardlist.open(QIODevice::WriteOnly);
    QTextStream hardliststream(&hardlist);
    hardliststream << output;
    hardlist.close();
}
