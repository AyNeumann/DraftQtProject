#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtXml>

#include <QDebug>

class configuration : public QObject
{
    Q_OBJECT
public:
    explicit configuration(QObject *parent = nullptr);

    QString getApiUrl();

private:

    QString m_apiUrl;

    void init();

signals:

};

#endif // CONFIGURATION_H
