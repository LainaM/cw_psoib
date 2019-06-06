#include "hardware.h"
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDir>
#include <QException>
#include <stdexcept>


bool HardwareList::get_current(Hardware &hardware)
{
    QString strCommand = "/usr/bin/inxi";
    QStringList args;
    args << "-ADGNCM" <<"--usb" << "--color" << "0";
    QProcess process;
    process.start(strCommand, args);
    if( !process.waitForStarted() || !process.waitForFinished() ) {
        return false;
    }

    QString error = process.readAllStandardError();
    if (!error.isEmpty()) {
        qDebug() << "Ошибка";
        qDebug() << error;
        qDebug() << process.readAllStandardOutput();
        return false;
    }

    QString output = process.readAllStandardOutput();
//    qDebug().noquote() << output << "\n\n";

    QRegularExpression reg_cpu("model: (?<cpu>[^>]+) bits");
    QRegularExpressionMatch match_cpu = reg_cpu.match(output);
    if (match_cpu.hasMatch()) {
        hardware.cpu.append(match_cpu.captured("cpu"));
    }

    QStringRef gpu_text = output.midRef(output.indexOf("Graphics:"));
    gpu_text = gpu_text.left(gpu_text.indexOf("Audio:"));
    while (!gpu_text.isEmpty() && gpu_text.contains("Device")){
        QStringRef device = gpu_text.mid(gpu_text.indexOf("Device-") + 10);
        gpu_text = device.mid(device.indexOf(" \n") + 1);
        device = device.left(device.indexOf("driver:"));
        hardware.gpu.append(device.toString());
     }

    QStringRef audio_text = output.midRef(output.indexOf("Audio:"));
    audio_text = audio_text.left(audio_text.indexOf("Network:"));
    while (!audio_text.isEmpty() && audio_text.contains("Device")){
        QStringRef device = audio_text.mid(audio_text.indexOf("Device-") + 10);
        audio_text = device.mid(device.indexOf(" \n") + 1);
        device = device.left(device.indexOf("driver:"));
        hardware.audio.append(device.toString());
    }

    QStringRef network_text = output.midRef(output.indexOf("Network:"));
    network_text = network_text.left(network_text.indexOf("Drives:"));
    while (!network_text.isEmpty() && network_text.contains("Device")){
        QStringRef device = network_text.mid(network_text.indexOf("Device-") + 10);
        network_text = device.mid(device.indexOf(" \n") + 1);
        device = device.left(device.indexOf("driver:"));
        hardware.network.append(device.toString());
    }

    QStringRef drive_text = output.midRef(output.indexOf("Drives:"));
    drive_text = drive_text.left(drive_text.indexOf("USB:"));
    while (!drive_text.isEmpty() && drive_text.contains("ID")){
        QStringRef device = drive_text.mid(drive_text.indexOf("ID-") + 10);
        drive_text = device.mid(device.indexOf(" \n") + 1);
        device = device.mid(device.indexOf("vendor: ") + 8);
        device = device.left(device.indexOf("size:"));
        hardware.drive.append(device.toString().replace(" model:", ""));
    }

    QStringRef usb_text = output.midRef(output.indexOf("USB:"));
    while (!usb_text.isEmpty() && usb_text.contains("Device")){
        QStringRef device = usb_text.mid(usb_text.indexOf("Device-") + 10);
        usb_text = device.mid(device.indexOf(" \n") + 1);
        device = device.mid(device.indexOf("info: ") + 6);
        device = device.left(device.indexOf("type:"));
        hardware.usb.append(device.toString());
    }


//    qDebug().noquote() << hardware.to_string();
    return true;
}

QString Hardware::to_string(){
    QString text;
    text = "CPU:\t" + cpu.join("\n\t") + "\n";
    text += "GPU:\t" + gpu.join("\n\t") + "\n";
    text += "AUDIO:\t" + audio.join("\n\t") + "\n";
    text += "NETWORK:\t" + network.join("\n\t") + "\n";
    text += "DRIVE:\t" + drive.join("\n\t") + "\n";
    text += "USB:\t" + usb.join("\n\t");

    return text;
}

QJsonObject Hardware::toJson() const {
    return {
        {"cpu", QJsonArray::fromStringList(cpu)},
        {"gpu", QJsonArray::fromStringList(gpu)},
        {"audio", QJsonArray::fromStringList(audio)},
        {"network", QJsonArray::fromStringList(network)},
        {"drive", QJsonArray::fromStringList(drive)},
        {"usb", QJsonArray::fromStringList(usb)}
    };
}

bool Hardware::isEmpty() const
{
    return cpu.isEmpty() && gpu.isEmpty() && audio.isEmpty() && network.isEmpty() && drive.isEmpty() && usb.isEmpty();
}

Hardware HardwareList::fromJson(const QString &text) {
    QJsonObject json = QJsonDocument::fromJson(text.toUtf8()).object();
    Hardware hardware;
    for (auto item: json["cpu"].toArray()){
        hardware.cpu.append(item.toString());
    }
    for (auto item: json["audio"].toArray()){
        hardware.audio.append(item.toString());
    }
    for (auto item: json["gpu"].toArray()){
        hardware.gpu.append(item.toString());
    }
    for (auto item: json["drive"].toArray()){
        hardware.drive.append(item.toString());
    }
    for (auto item: json["network"].toArray()){
        hardware.network.append(item.toString());
    }
    for (auto item: json["usb"].toArray()){
        hardware.usb.append(item.toString());
    }
    return hardware;
}


void Hardware::save()
{
    QFile hardlist(QDir::homePath() + "/.hardlist.txt");
    if(!hardlist.open(QIODevice::WriteOnly)){
        throw std::runtime_error("Cant save in hardlist.txt");
    }
    QTextStream hardliststream(&hardlist);
    hardliststream << QJsonDocument(this->toJson()).toJson();
    hardlist.close();
}

void Hardware::save_as_black()
{
    QFile hardlist(QDir::homePath() + "/.black_list.txt");
    if(!hardlist.open(QIODevice::WriteOnly)){
        throw std::runtime_error("Cant save in hardlist.txt");
    }
    QTextStream hardliststream(&hardlist);
    hardliststream << QJsonDocument(this->toJson()).toJson();
    hardlist.close();
}

void Hardware::save_as_white()
{
    QFile hardlist(QDir::homePath() + "/.white_list.txt");
    if(!hardlist.open(QIODevice::WriteOnly)){
        throw std::runtime_error("Cant save in hardlist.txt");
    }
    QTextStream hardliststream(&hardlist);
    hardliststream << QJsonDocument(this->toJson()).toJson();
    hardlist.close();
}


Hardware HardwareList::load_saved()
{
    QFile hardlist(QDir::homePath() + "/.hardlist.txt");
    if (!hardlist.open(QIODevice::ReadOnly | QIODevice::Text)){
        return Hardware();
    }
    QTextStream text(&hardlist);
    Hardware hardware = HardwareList::fromJson(text.readAll());
    hardlist.close();
    return hardware;
}

Hardware HardwareList::load_black_list()
{
    QFile hardlist(QDir::homePath() + "/.black_list.txt");
    if (!hardlist.open(QIODevice::ReadOnly | QIODevice::Text)){
        return Hardware();
    }
    QTextStream text(&hardlist);
    Hardware hardware = HardwareList::fromJson(text.readAll());
    hardlist.close();
    return hardware;
}

Hardware HardwareList::load_white_list()
{
    QFile hardlist(QDir::homePath() + "/.white_list.txt");
    if (!hardlist.open(QIODevice::ReadOnly | QIODevice::Text)){
        return Hardware();
    }
    QTextStream text(&hardlist);
    Hardware hardware = HardwareList::fromJson(text.readAll());
    hardlist.close();
    return hardware;
}


bool Hardware::operator==(const Hardware& rhs){
    return cpu == rhs.cpu &&
           gpu == rhs.gpu &&
           audio == rhs.audio &&
           network == rhs.network &&
           drive == rhs.drive &&
           usb == rhs.usb;
}

bool Hardware::operator!=(const Hardware& rhs){
    return !(*this == rhs);
}
