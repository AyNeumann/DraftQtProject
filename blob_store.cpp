#include "blob_store.h"

blobStore::blobStore(QObject *parent) : QObject(parent)
{

}

QString blobStore::storeAllBlobs(QJsonDocument *blobsToSaveJson)
{
    QString returnMsg;
    QString blobStore = QDir::currentPath() + QDir::separator() + "blobStore.txt";
    QFile file(blobStore);

    QJsonObject blobsObj = blobsToSaveJson->object();
    QJsonArray blobsArray = blobsObj["content"].toArray();
    QJsonDocument blobsToSave(blobsArray);

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
