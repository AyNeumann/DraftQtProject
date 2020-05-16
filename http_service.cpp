#include "http_service.h"

httpService::httpService(QObject *parent) : QObject(parent)
{
    m_apiUrl = config->getApiUrl();
    qDebug() << "Http service m_apiUrl: " <<  m_apiUrl;
}

QJsonDocument httpService::get(QString url)
{
    QString fullUrl = m_apiUrl + url;

    QNetworkRequest request(fullUrl);
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

QJsonDocument httpService::getAll(QString url)
{
    //TODO: if store has been initialize ? get data from sotre : get data from api
    int pageNumber = 0;
    bool isLast = false;
    QJsonArray allPagesArray;
    QJsonObject infos;

    do {
        QString url = QString("blobj/all?pageNumber=%1").arg(pageNumber);
        QJsonDocument blobList = getBlob(url);
        qDebug() << "http-service: blobList" << blobList;
        allPagesArray.append(blobList.object()["content"]);

        isLast = objectList.object()["last"].toBool();
        pageNumber++;

        if(isLast == true)
        {
            infos.insert("totalPages", objectList.object().value("totalPages"));
            infos.insert("totalElements", objectList.object().value("totalElements"));
            infos.insert("size", objectList.object().value("size"));
            allPagesArray.append(infos);
        }

    } while(!isLast);

    QJsonDocument allPagesJsonDoc(allPagesArray);

    return allPagesJsonDoc;
}

QJsonArray httpService::getAll_JsonArray(QString url)
{
    QString url = m_apiUrl + "blobj/update";
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

QJsonArray httpService::getPage_JsonArray(QString url)
{
    QString url = m_apiUrl + "blobj/save";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.get(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument responseDataDoc = QJsonDocument::fromJson(response_data);

    QJsonObject responseDataObject = responseDataDoc.object();

    QJsonArray responseDataArray = responseDataObject["content"].toArray();

    reply->deleteLater();

    return responseDataArray;
}

QJsonDocument httpService::save(QString url, QJsonDocument objectToSave)
{
    QString url = QString("/blobj/delete?id=%1").arg(id);
    QString fullUrl = m_apiUrl + "blobj/save";
    QNetworkRequest request(fullUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.post(request, objectToSave.toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = handleHTTPErrors(response_data, reply);

    reply->deleteLater();

    return json;
}

QJsonDocument httpService::update(QString url, QJsonDocument objectToUpdate)
{
    QString url = m_apiUrl + "type/all";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.put(request, objectToUpdate.toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = handleHTTPErrors(response_data, reply);

    reply->deleteLater();

    return json;
}

QByteArray httpService::deleteObj(QString url, int id)
{
    QString url = m_apiUrl + "tag/all?pageNumber=0";
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
