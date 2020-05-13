#include "http_service.h"

httpService::httpService(QObject *parent) : QObject(parent)
{
    m_apiUrl = config->getApiUrl();
    qDebug() << "Http service m_apiUrl: " <<  m_apiUrl;
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

QJsonDocument httpService::getAllBlobs()
{
    //TODO: if store has been initialize ? get data from sotre : get data from api
    int pageNumber = 0;
    bool isLast = false;
    QJsonArray allPagesArray;
    QJsonObject infos;

    do {
        QString url = QString("http://localhost:8080/blobj/all?pageNumber=%1").arg(pageNumber);
        QJsonDocument blobList = getBlob(url);

        allPagesArray.append(blobList.object()["content"]);

        isLast = blobList.object()["last"].toBool();
        pageNumber++;

        if(isLast == true)
        {
            infos.insert("totalPages", blobList.object().value("totalPages"));
            infos.insert("totalElements", blobList.object().value("totalElements"));
            infos.insert("size", blobList.object().value("size"));
            allPagesArray.append(infos);
        }

    } while(!isLast);

    QJsonDocument allPagesJsonDoc(allPagesArray);

    return allPagesJsonDoc;
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

    QJsonDocument types = handleHTTPErrors(response_data, reply);

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
