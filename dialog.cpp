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
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    QWidget::setWindowTitle("Aapi Draft Qt Client");

    ui->splitter_2->setStretchFactor(1,3);

    connect(ui->pB_GetAll, &QPushButton::clicked, this, &Dialog::getAllBlobJs);
    connect(ui->pB_GetAllAndSave, &QPushButton::clicked, this, &Dialog::getAllBlobJs);
    connect(ui->pB_GetById, &QPushButton::clicked, this, &Dialog::getSender);
    connect(ui->pB_GetByCount, &QPushButton::clicked, this, &Dialog::getBlobByCount);
    connect(ui->pB_Save, &QPushButton::clicked, this, &Dialog::saveBlob);
    connect(ui->pB_SaveBlobForm, &QPushButton::clicked, this, &Dialog::saveBlobFromForm);
    connect(ui->pB_Delete, &QPushButton::clicked, this, &Dialog::deleteBlob);
    connect(ui->rB_ExactCount, &QRadioButton::clicked, this, &Dialog::checkCountRadioButton);
    connect(ui->rB_MaxCount, &QRadioButton::clicked, this, &Dialog::checkCountRadioButton);
    connect(ui->rB_MinCount, &QRadioButton::clicked, this, &Dialog::checkCountRadioButton);
    connect(ui->rB_MinMaxCount, &QRadioButton::clicked, this, &Dialog::checkCountRadioButton);
    connect(ui->pB_GetByName, &QPushButton::clicked, this, &Dialog::getBlobByName);
    connect(ui->pB_GetByType, &QPushButton::clicked, this, &Dialog::getBlobByType);

    QPixmap pixmap(":/icons/save32x32.png");
    QIcon ButtonIcon(pixmap);
    ui->pB_GetAllAndSave->setIcon(ButtonIcon);

    ui->pB_GetAllAndSave->setEnabled(false);

    connect(ui->pB_ViewBlob_AddTag, &QPushButton::clicked, this, &Dialog::getSender);
    connect(ui->pB_AddTag, &QPushButton::clicked, this, &Dialog::addTagToBlob);
    connect(ui->pB_Get_UpdateBlobForm, &QPushButton::clicked, this, &Dialog::getBlobForUpdate);

    QJsonArray types = getAllBlobJTypes();

    for(int i=0; i< types.count(); ++i){
        ui->cB_BlobType_SaveForm->addItem(types.at(i).toString());
        ui->cB_BlobJType_Get->addItem(types.at(i).toString());
        ui->cB_BlobType_UpdateForm->addItem(types.at(i).toString());
    }

    QJsonArray tags = getAllTags();

    for(int i=0; i< tags.count(); ++i){
        ui->cB_TagName_AddTag->addItem(tags.at(i)["name"].toString());
    }

    ui->pB_UpdateBlobForm->setEnabled(false);
}

void Dialog::getAllBlobJs()
{
    QString url = QString("http://localhost:8080/blobj/all?pageNumber=%1").arg(ui->sB_PageNumber->value());

    QJsonDocument blobJList = getBlobJFromDB(url);

    displayResponse(&blobJList);
}

void Dialog::getBlobById(QString btnName)
{
    int arg = 0;

    if(btnName == "Get By Id")
    {
        arg = ui->sb_GetIdNumber->value();
    }

    if(btnName == "View")
    {
        arg = ui->sB_BlobId_AddTag->value();
    }

    if(arg == 0)
    {
        return;
    }

    QString url = QString("http://localhost:8080/blobj/byId?id=%1").arg(arg);

    QJsonDocument blobJ = getBlobJFromDB(url);

    displayResponse(&blobJ);
}

void Dialog::getBlobByCount()
{
    QString url = "";

    if(ui->rB_ExactCount->isChecked())
    {
        url = QString("http://localhost:8080/blobj/byCount?count=%1")
                .arg(ui->sB_BlobJCount1->value());
    }
    if(ui->rB_MinCount->isChecked())
    {
        url = QString("http://localhost:8080/blobj/byCountMin?minCount=%1")
                .arg(ui->sB_BlobJCount1->value());
    }
    if(ui->rB_MaxCount->isChecked())
    {
        url = QString("http://localhost:8080/blobj/byCountMax?maxCount=%1")
                .arg(ui->sB_BlobJCount1->value());
    }
    if(ui->rB_MinMaxCount->isChecked())
    {
        url = QString("http://localhost:8080/blobj/byCountTranche?minCount=%1&maxCount=%2")
                .arg(ui->sB_BlobJCount1->value()).arg(ui->sB_BlobJCount2->value());
    }

    QJsonDocument blobJList = getBlobJFromDB(url);

    displayResponse(&blobJList);
}

void Dialog::getBlobByName()
{
    QString url = QString("http://localhost:8080/blobj/byName?name=%1").arg(ui->lE_BlobJName_Get->text());

    QJsonDocument blobJList = getBlobJFromDB(url);

    displayResponse(&blobJList);
}

void Dialog::getBlobByType()
{
    QString url = QString("http://localhost:8080/blobj/byType?type=%1").arg(ui->cB_BlobJType_Get->currentText());

    QJsonDocument blobJList = getBlobJFromDB(url);

    displayResponse(&blobJList);
}

void Dialog::getBlobForUpdate()
{
    QString url = "";

    if(!ui->lE_BlobId_UpdateForm->text().isEmpty()) {
        url = QString("http://localhost:8080/blobj/byId?id=%1").arg(ui->lE_BlobId_UpdateForm->text().toInt());
    } else if (!ui->lE_BlobName_UpdateForm->text().isEmpty()) {
        url = QString("http://localhost:8080/blobj/byName?name=%1").arg(ui->lE_BlobName_UpdateForm->text());
    } else if (!ui->lE_BlobName_UpdateForm->text().isEmpty() && !ui->lE_BlobId_UpdateForm->text().isEmpty()) {
        url = QString("http://localhost:8080/blobj/byId?id=%1").arg(ui->lE_BlobId_UpdateForm->text().toInt());
    }

    QJsonDocument blobJsonDoc = getBlobJFromDB(url);

    qDebug() << "JSON DOC" << blobJsonDoc;

    QJsonObject blob;

    if(blobJsonDoc.isObject()) {
        blob = blobJsonDoc.object();
    } else if (blobJsonDoc.isArray()) {
        QJsonArray blobArray = blobJsonDoc.array();
        blob = blobArray[0].toObject();
    }


    QString idAsString = QString::number(blob["id"].toDouble());

    // Display in form
    ui->lE_BlobId_UpdateForm->setText(idAsString);
    ui->lE_BlobName_UpdateForm->setText(blob["name"].toString());
    ui->lE_BlobSign_UpdateForm->setText(blob["sign"].toString());
    ui->sB_BlobCount_UpdateForm->setValue(blob["count"].toInt());
    ui->sB_BlobRank_UpdateForm->setValue(blob["rank"].toInt());
    ui->cB_BlobType_UpdateForm->setCurrentText(blob["type"].toString());

    // Display in main view
    displayResponse(&blobJsonDoc);
}

void Dialog::saveBlob()
{
    QString BlobJAsText = ui->pTE_View->toPlainText();

    QJsonDocument BlobJAsJson = QJsonDocument::fromJson(BlobJAsText.toUtf8());

    saveBlobInDB(BlobJAsJson);
}

void Dialog::deleteBlob()
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

    ui->pTE_View->document()->setPlainText(response_data);

    reply->deleteLater();
}

void Dialog::displayResponse(QJsonDocument *json)
{
    QString strJson(json->toJson());

    ui->pTE_View->document()->setPlainText(strJson);
}

void Dialog::saveBlobFromForm()
{
    QJsonObject blobJToSave
    {
        {"name", ui->lE_BlobName_SaveForm->text()},
        {"sign", ui->lE_BlobSign_SaveForm->text()},
        {"count", ui->sB_BlobCount_SaveForm->value()},
        {"rank", ui->sB_BlobRank_SaveForm->value()},
        {"type", ui->cB_BlobType_SaveForm->currentText()},
    };

    saveBlobInDB(QJsonDocument(blobJToSave));
}

void Dialog::saveBlobInDB(QJsonDocument blobJToSave)
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

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    QString strJson(json.toJson());

    ui->pTE_View->document()->setPlainText(strJson);

    reply->deleteLater();
}

void Dialog::addTagToBlob()
{

}

QJsonDocument Dialog::getBlobJFromDB(QString url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nam.get(request);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    reply->deleteLater();

    return json;
}

QJsonArray Dialog::getAllBlobJTypes()
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

QJsonArray Dialog::getAllTags()
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

    for(const QJsonValue &tag : tagsArray) {
        QJsonObject obj = tag.toObject();
    }

    reply->deleteLater();

    return tagsArray;
}

void Dialog::checkCountRadioButton()
{
    if(ui->rB_MinMaxCount->isChecked())
    {
        ui->sB_BlobJCount2->setEnabled(true);
    } else {
        ui->sB_BlobJCount2->setEnabled(false);
    }
}

void Dialog::getSender()
{
    QPushButton *btn = static_cast<QPushButton*>(sender());
    if(!btn) return;

    getBlobById(btn->text());
}
