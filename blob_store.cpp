#include "blob_store.h"

blobStore::blobStore(QObject *parent) : QObject(parent)
{
    m_isUpToDate = false;
}

QString blobStore::storeBlobs(QJsonDocument &blobsToSave)
{
    qDebug() << "Storing blobs...";
    QString returnMsg;

    QString blobStore = QDir::currentPath() + QDir::separator() + "blobStore.json";
    QFile file(blobStore);

    if(!file.open(QIODevice::WriteOnly))
    {
        returnMsg = file.errorString();
        return returnMsg;
    }

    file.write(blobsToSave.toJson());
    file.close();

    m_isUpToDate = true;

    returnMsg = "OK";

    return returnMsg;
}

QString blobStore::storeBlobsInTempStore(QJsonDocument &blobsToSaveJson)
{
    QJsonArray blobsArray;
    QString returnMsg;

    QString blobStore = QDir::currentPath() + QDir::separator() + "tempBlobStore.json";
    QFile file(blobStore);

    QJsonObject blobsObj = blobsToSaveJson.object();
    blobsArray = blobsObj["content"].toArray();
    QJsonDocument blobsToSaveFromArray(blobsArray);

    if(!file.open(QIODevice::WriteOnly))
    {
        returnMsg = file.errorString();
        return returnMsg;
    }

    file.write(blobsToSaveJson.toJson());
    file.close();

    returnMsg = "OK";

    return returnMsg;
}

QJsonDocument blobStore::getBlobs()
{

    qDebug() << "Get all blobs from STORE";
    QJsonDocument storeContentJson;
    QJsonParseError jsonError;
    QByteArray storeContentArray;

    QString blobStore = QDir::currentPath() + QDir::separator() + "blobStore.json";
    QFile file(blobStore);

    if(!file.exists())
    {
        qWarning() << "Blob store file not found";
    }

    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open Blob store file: " << file.errorString();
    }

    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        storeContentArray.append(line);
    }

    file.close();

    storeContentJson = QJsonDocument::fromJson(storeContentArray, &jsonError);

    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << QString("JsonError: %1").arg(jsonError.errorString());
        qDebug() << jsonError.offset;
    }
    else if(storeContentJson.isEmpty())
    {
        qDebug() << "storeContentJsonArray is Empty";
    }

    return storeContentJson;
}

bool blobStore::getIsUpToDate()
{
    return m_isUpToDate;
}

void blobStore::setIsUpToDate(bool isUpToDate)
{
    m_isUpToDate = isUpToDate;
}
