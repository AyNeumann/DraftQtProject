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

    QString storeBlobs(QJsonDocument &blobsToSave);

    QString test_storeBlobs(QByteArray &blobsToSave);

    QString storeBlobsInTempStore(QJsonDocument &blobsToSave);
    QJsonDocument getBlobs();

    bool getIsUpToDate();
    void setIsUpToDate(bool isUpToDate);

private:
    bool m_isUpToDate;

signals:

};

#endif // BLOBSTORE_H
