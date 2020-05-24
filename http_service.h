#ifndef HTTPSERVICE_H
#define HTTPSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QApplication>
#include <QNetworkReply>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "configuration.h"

class httpService : public QObject
{
    Q_OBJECT
public:
    explicit httpService(QObject *parent = nullptr);

    QJsonDocument get(QString &url);
    QJsonDocument getAll(QString &url);
    QJsonArray getAll_JsonArray(QString &url);
    QJsonArray getPage_JsonArray(QString &url);
    QJsonDocument save(QString &url, QJsonDocument &blobJToSave);
    QJsonDocument update(QString &url, QJsonDocument &blobJToUpdate);
    QByteArray deleteObj(QString &url, int &id);

private:
    QJsonDocument handleHTTPErrors(QByteArray &response_data, QNetworkReply *reply);

    configuration *config = new configuration(this);
    QNetworkAccessManager nam;
    QString m_apiUrl;

signals:

};

#endif // HTTPSERVICE_H
