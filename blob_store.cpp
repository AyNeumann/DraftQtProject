#include "blob_store.h"

blobStore::blobStore(QObject *parent) : QObject(parent)
{

}

QString blobStore::storeBlobs(QJsonDocument &blobsToSave)
{
    QString returnMsg;

    QString blobStore = QDir::currentPath() + QDir::separator() + "blobStore.txt";
    QFile file(blobStore);

    if(!file.open(QIODevice::WriteOnly))
    {
        returnMsg = file.errorString();
        return returnMsg;
    }

    QTextStream stream(&file);
    stream << QString(blobsToSave.toJson());
    file.close();

    returnMsg = "OK";

    return returnMsg;
}

QString blobStore::storeBlobsInTempStore(QJsonDocument &blobsToSaveJson)
{
    QJsonArray blobsArray;
    QString returnMsg;

    QString blobStore = QDir::currentPath() + QDir::separator() + "tempBlobStore.txt";
    QFile file(blobStore);

    QJsonObject blobsObj = blobsToSaveJson.object();
    blobsArray = blobsObj["content"].toArray();
    QJsonDocument blobsToSaveFromArray(blobsArray);

    if(!file.open(QIODevice::WriteOnly))
    {
        returnMsg = file.errorString();
        return returnMsg;
    }

    QTextStream stream(&file);
    stream << QString(blobsToSaveFromArray.toJson());
    file.close();

    returnMsg = "OK";

    return returnMsg;
}
