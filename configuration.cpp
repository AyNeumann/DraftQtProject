#include "configuration.h"

configuration::configuration(QObject *parent) : QObject(parent)
{
    init();
}

QString configuration::getApiUrl()
{
    return m_apiUrl;
}

void configuration::init()
{
    QString config;
    QString configFile = QDir::currentPath() + QDir::separator() + "configuration.txt";
    QFile file(configFile);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
    }
    config = file.readAll();
    file.close();

    qDebug() << config;

    QJsonDocument configJson = QJsonDocument::fromJson(config.toUtf8());
    QJsonObject configObject = configJson.object();

    m_apiUrl = configObject["apiUrl"].toString();

    qDebug() << "configuration m_apiUrl: " << m_apiUrl;
}
