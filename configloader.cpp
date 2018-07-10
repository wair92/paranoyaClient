#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QDebug>
#include <QFile>
#include "configloader.h"

ConfigLoader::ConfigLoader(QString configPath)
{
    QString data;
    QFile file;
    file.setFileName(configPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(file.isOpen()){
        data = file.readAll();
        file.close();
    }
    QJsonDocument config = QJsonDocument::fromJson(data.toUtf8());
    if(config.isNull()){
        setDefaultValules();
    }
    else{
        QJsonObject object = config.object();
        auto ip = object.value(QString("Ip")).toString();
        quint16 port { 0 };
        port = object.value(QString("Port")).toInt();
        if( ip.isEmpty() || port == 0){
            qDebug() << "Invalid config file format ... ";
            setDefaultValules();
        }
        else
        {
            ip_ = ip;
            port_ = port;
            qDebug() << "Loaded data from config file: ";
            qDebug() << "Ip: " << ip_;
            qDebug() << "Port: " << port_;
        }

    }

}

ConfigLoader::~ConfigLoader()
{

}

void ConfigLoader::setDefaultValules()
{
    qDebug() << "Loading config file failed, setting default values.";
    ip_ = "127.0.0.1";
    port_ = 9000;
}
