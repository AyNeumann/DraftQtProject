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

    QJsonDocument getBlob(QString url);
    QJsonDocument getAllBlobs();
    QJsonDocument updateBlob(QJsonDocument blobJToUpdate);
    QJsonDocument saveBlob(QJsonDocument blobJToSave);
    QByteArray deleteBlob(int id);

    QJsonArray getAllBlobTypes();
    QJsonArray getAllTags();

private:
    QJsonDocument handleHTTPErrors(QByteArray response_data, QNetworkReply *reply);

    configuration *config = new configuration(this);
    QNetworkAccessManager nam;
    QString m_apiUrl;

signals:

};

#endif // HTTPSERVICE_H
