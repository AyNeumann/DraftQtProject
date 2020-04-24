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
    connect(ui->pB_GetById, &QPushButton::clicked, this, &Dialog::getBlobJById);
    connect(ui->pB_GetByCount, &QPushButton::clicked, this, &Dialog::getBlobByCount);
    connect(ui->pB_Save, &QPushButton::clicked, this, &Dialog::saveBlobJ);
    connect(ui->pB_SaveBlobJForm, &QPushButton::clicked, this, &Dialog::saveBlobFromForm);
    connect(ui->pB_Delete, &QPushButton::clicked, this, &Dialog::deleteBlobJ);
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

    QJsonArray types = getAllBlobJTypes();

    for(int i=0; i< types.count(); ++i){
        ui->cB_BlobJType->addItem(types.at(i).toString());
        ui->cB_BlobJType_Get->addItem(types.at(i).toString());
    }
}

void Dialog::getAllBlobJs()
{
    QString url = QString("http://localhost:8080/blobj/all?pageNumber=%1").arg(ui->sB_PageNumber->value());

    QJsonDocument blobJList = getBlobJFromDB(url);

    displayResponse(&blobJList);

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
}

void Dialog::getBlobJById()
{
    QString url = QString("http://localhost:8080/blobj/byId?id=%1").arg(ui->sb_GetIdNumber->value());

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

void Dialog::saveBlobJ()
{
    QString BlobJAsText = ui->pTE_View->toPlainText();

    QJsonDocument BlobJAsJson = QJsonDocument::fromJson(BlobJAsText.toUtf8());

    saveBlobInDB(BlobJAsJson);
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
        {"name", ui->lE_BlobJName->text()},
        {"sign", ui->lE_BlobJSign->text()},
        {"count", ui->sB_BlobJCount->value()},
        {"rank", ui->sB_BlobJRank->value()},
        {"type", ui->cB_BlobJType->currentText()},
    };

    qDebug() << blobJToSave;

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

void Dialog::checkCountRadioButton()
{
    if(ui->rB_MinMaxCount->isChecked())
    {
        ui->sB_BlobJCount2->setEnabled(true);
    } else {
        ui->sB_BlobJCount2->setEnabled(false);
    }
}
