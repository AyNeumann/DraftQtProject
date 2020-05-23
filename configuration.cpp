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
    //getConfigurationJsonFile();
    getConfigurationXmlFile();
}

void configuration::getConfigurationJsonFile()
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
}

void configuration::getConfigurationXmlFile()
{
    QDomDocument xmlBOM;
    QFile file("configuration.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
    }
    xmlBOM.setContent(&file);
    file.close();

    QByteArray data = xmlBOM.toByteArray();

    QXmlStreamReader reader;
    reader.addData(data);
    while(reader.readNextStartElement())
    {
        if (reader.name() == "api_url")
        {
            m_apiUrl = reader.readElementText();
        }
    }
}
