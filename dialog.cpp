#include "dialog.h"
#include "ui_dialog.h"

/**
 * @see: https://amin-ahmadi.com/2016/01/17/how-to-send-and-receive-json-requests-in-qt/
 * @see: https://makina-corpus.com/blog/metier/archives/access-json-webservice-qt-c
 * @see: Qt official doc: https://doc.qt.io/qt-5/qnetworkaccessmanager.html
 * @brief Dialog::Dialog
 * @param parent
 */
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    init();
}

Dialog::~Dialog()
{
    delete ui;
}

static QNetworkAccessManager nam;

void Dialog::init()
{
    connect(ui->pB_GetAll, &QPushButton::clicked, this, &Dialog::getAllBlobJs);
    connect(ui->pB_GetAllAndSave, &QPushButton::clicked, this, &Dialog::getAllBlobJs);
    connect(ui->pB_GetById, &QPushButton::clicked, this, &Dialog::getBlobJById);
    connect(ui->pB_Save, &QPushButton::clicked, this, &Dialog::saveBlobJ);
    connect(ui->pB_SaveBlobJForm, &QPushButton::clicked, this, &Dialog::saveBlobJFromForm);
    connect(ui->pB_Delete, &QPushButton::clicked, this, &Dialog::deleteBlobJ);

    QPixmap pixmap(":/icons/save32x32.png");
    QIcon ButtonIcon(pixmap);
    ui->pB_GetAllAndSave->setIcon(ButtonIcon);

    ui->pB_GetAllAndSave->setEnabled(false);
}

void Dialog::getAllBlobJs()
{
    QString url = QString("http://localhost:8080/blobj/all?pageNumber=%1").arg(ui->sB_PageNumber->value());
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.get(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    displayResponse(&json);

    //***********************************************************************

    /*QString blobJ;

    QString strReply = (QString)reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(strReply.toUtf8());

    QJsonObject jsonObject = json.object();

    QJsonArray jsonArray = jsonObject["content"].toArray();

    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        blobJ.append(
                "id: " + obj["id"].toVariant().toString() +
                " name: " + obj["name"].toString() +
                " sign: " + obj["sign"].toString() +
                " count: " + obj["count"].toVariant().toString() +
                " rank: " + obj["rank"].toVariant().toString() +
                " type: " + obj["type"].toString() +
                " tag: " + obj["tag"].toVariant().toString() +
                " linkedBlobJ: " +obj["linkedBlobJ"].toVariant().toString()
                     );

        qDebug() << blobJ;
        blobJ.clear();
    }*/



    reply->deleteLater();
}

void Dialog::getBlobJById()
{
    QString url = QString("http://localhost:8080/blobj/byId?id=%1").arg(ui->sb_GetIdNumber->value());
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.get(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    displayResponse(&json);

    reply->deleteLater();
}

void Dialog::saveBlobJ()
{
    QString url = QString("http://localhost:8080/blobj/save");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString BlobJAsText = ui->pTE_View->toPlainText();

    qDebug() << "BlobJ As Text: " << BlobJAsText;

    QJsonDocument BlobJAsJson = QJsonDocument::fromJson(BlobJAsText.toUtf8());

    QNetworkReply *reply = nam.post(request, QJsonDocument(BlobJAsJson).toJson());


    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    //qDebug() << json.toJson();

    QString strJson(json.toJson());

    ui->pTE_View->document()->setPlainText(strJson);

    reply->deleteLater();
}

void Dialog::deleteBlobJ()
{
    QString url = QString("http://localhost:8080/blobj/delete?id=%1").arg(ui->sb_DeleteIdNumber->value());
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.deleteResource(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    // qDebug() << "Response data: " << response_data;

    ui->pTE_View->document()->setPlainText(response_data);

    reply->deleteLater();
}

void Dialog::displayResponse(QJsonDocument *json)
{
    QString strJson(json->toJson());

    ui->pTE_View->document()->setPlainText(strJson);
}

void Dialog::saveBlobJFromForm()
{
    QJsonObject blobJToSave
    {
        {"name", ui->lE_BlobJName->text()},
        {"sign", ui->lE_BlobJSign->text()},
        {"count", ui->sB_BlobJCount->value()},
        {"rank", ui->sB_BlobJRank->value()},
        {"type", ui->lE_BlobJType->text()},
    };

    qDebug() << blobJToSave;

    QString url = QString("http://localhost:8080/blobj/save");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.post(request, QJsonDocument(blobJToSave).toJson());


    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    QString strJson(json.toJson());

    ui->pTE_View->document()->setPlainText(strJson);

    reply->deleteLater();
}
