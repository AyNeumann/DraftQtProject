#include "http_service.h"

static QNetworkAccessManager nam;

httpService::httpService(QObject *parent) : QObject(parent)
{

}

QJsonDocument httpService::getBlob(QString url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.get(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = handleHTTPErrors(response_data, reply);

    reply->deleteLater();

    return json;
}

QJsonDocument httpService::updateBlob(QJsonDocument blobJToUpdate)
{
    QString url = QString("http://localhost:8080/blobj/update");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.put(request, blobJToUpdate.toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = handleHTTPErrors(response_data, reply);

    reply->deleteLater();

    return json;
}

QJsonDocument httpService::saveBlob(QJsonDocument blobJToSave)
{
    QString url = QString("http://localhost:8080/blobj/save");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.post(request, blobJToSave.toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = handleHTTPErrors(response_data, reply);

    reply->deleteLater();

    return json;
}

QByteArray httpService::deleteBlob(int id)
{
    QString url = QString("http://localhost:8080/blobj/delete?id=%1").arg(id);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.deleteResource(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    reply->deleteLater();

    return response_data;
}

QJsonArray httpService::getAllBlobTypes()
{
    QString url = QString("http://localhost:8080/type/all");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.get(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument types = QJsonDocument::fromJson(response_data);

    QJsonArray typesArray = types.array();

    reply->deleteLater();

    return typesArray;
}

QJsonArray httpService::getAllTags()
{
    QString url = QString("http://localhost:8080/tag/all?pageNumber=0");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.get(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument tagsDocument = QJsonDocument::fromJson(response_data);

    QJsonObject tagsObject = tagsDocument.object();

    QJsonArray tagsArray = tagsObject["content"].toArray();

    /*for(const QJsonValue &tag : tagsArray) {
        QJsonObject obj = tag.toObject();
    }*/

    reply->deleteLater();

    return tagsArray;
}

QJsonDocument httpService::handleHTTPErrors(QByteArray response_data, QNetworkReply *reply)
{
    QJsonDocument json;

    if(reply->error() != QNetworkReply::NoError) {
        QJsonDocument errorJson = QJsonDocument::fromJson(response_data);
        QJsonObject errorObject = errorJson.object();
        QJsonObject errorMsg;
        errorMsg.insert("Message", errorObject.value("message"));
        errorMsg.insert("HttpError", errorObject.value("httpErrorNumber"));
        QJsonDocument errorMsgJson(errorMsg);
        json = errorMsgJson;
    } else {
        json = QJsonDocument::fromJson(response_data);
    }

    return json;
}
