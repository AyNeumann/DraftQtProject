#ifndef BLOBSTORE_H
#define BLOBSTORE_H

#include <QObject>
#include <QIODevice>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QMessageBox>

#include <QDebug>

class blobStore : public QObject
{
    Q_OBJECT
public:
    explicit blobStore(QObject *parent = nullptr);

    QString storeAllBlobs(QJsonDocument *BlobsToSave);

signals:

};

#endif // BLOBSTORE_H
