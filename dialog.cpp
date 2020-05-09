#include "dialog.h"
#include "ui_dialog.h"

/**
 * @see: https://amin-ahmadi.com/2016/01/17/how-to-send-and-receive-json-requests-in-qt/
 * @see: https://makina-corpus.com/blog/metier/archives/access-json-webservice-qt-c
 * @see: http://erickveil.github.io/2016/04/06/How-To-Manipulate-JSON-With-C++-and-Qt.html
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

void Dialog::init()
{
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    QWidget::setWindowTitle("Aapi Draft Qt Client");

    ui->splitter->setStretchFactor(3,1);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter_2->setStretchFactor(1,3);

    connect(ui->pB_GetPage, &QPushButton::clicked, this, &Dialog::getSentder_getAll);
    connect(ui->pB_GetPageAndSave, &QPushButton::clicked, this, &Dialog::getSentder_getAll);
    connect(ui->pB_GetAllPages, &QPushButton::clicked, this, &Dialog::getAllBlobs);
    connect(ui->pB_GetById, &QPushButton::clicked, this, &Dialog::getSender_getById);
    connect(ui->pB_GetByCount, &QPushButton::clicked, this, &Dialog::getBlobByCount);
    connect(ui->pB_Save, &QPushButton::clicked, this, &Dialog::saveBlob);
    connect(ui->pB_SaveBlobForm, &QPushButton::clicked, this, &Dialog::saveBlobFromForm);
    connect(ui->pB_UpdateBlobForm, &QPushButton::clicked, this, &Dialog::updateBlob);
    connect(ui->pB_Delete, &QPushButton::clicked, this, &Dialog::deleteBlob);
    connect(ui->rB_ExactCount, &QRadioButton::clicked, this, &Dialog::checkCountRadioButton);
    connect(ui->rB_MaxCount, &QRadioButton::clicked, this, &Dialog::checkCountRadioButton);
    connect(ui->rB_MinCount, &QRadioButton::clicked, this, &Dialog::checkCountRadioButton);
    connect(ui->rB_MinMaxCount, &QRadioButton::clicked, this, &Dialog::checkCountRadioButton);
    connect(ui->pB_GetByName, &QPushButton::clicked, this, &Dialog::getBlobByName);
    connect(ui->pB_GetByType, &QPushButton::clicked, this, &Dialog::getBlobByType);
    connect(ui->pB_ViewBlob_AddTag, &QPushButton::clicked, this, &Dialog::getSender_getById);
    connect(ui->pB_AddTag, &QPushButton::clicked, this, &Dialog::addTagToBlob);
    connect(ui->pB_Get_UpdateBlobForm, &QPushButton::clicked, this, &Dialog::getBlobForUpdate);

    QPixmap pixmap(":/icons/save32x32.png");
    QIcon ButtonIcon(pixmap);
    ui->pB_GetPageAndSave->setIcon(ButtonIcon);
    ui->pB_GetAllPages->setIcon(ButtonIcon);
    ui->pB_GetPageAndSave_Tag->setIcon(ButtonIcon);
    ui->pB_GetAllPages_Tag->setIcon(ButtonIcon);

    initDataBindUi();

    ui->pB_AddTag->setEnabled(false);
}

void Dialog::initDataBindUi()
{
    QJsonArray types = httpService.getAllBlobTypes();

    for(int i=0; i< types.count(); ++i){
        ui->cB_BlobType_SaveForm->addItem(types.at(i).toString());
        ui->cB_BlobJType_Get->addItem(types.at(i).toString());
        ui->cB_BlobType_UpdateForm->addItem(types.at(i).toString());
    }

    QJsonArray tags = httpService.getAllTags();

    for(int i=0; i< tags.count(); ++i){
        ui->cB_TagName_AddTag->addItem(tags.at(i)["name"].toString());
    }
}

void Dialog::getAllBlobsByPage(QString btnName)
{
    QString url = QString("http://localhost:8080/blobj/all?pageNumber=%1").arg(ui->sB_PageNumber->value());

    QJsonDocument blobList = httpService.getBlob(url);

    displayResponse(&blobList);

    if(btnName == "pB_GetPageAndSave")
    {
        QString storeStatus = blobStore.storeBlobsInTempStore(&blobList);

        if(storeStatus != "OK") {
            QMessageBox::critical(this, "Error", storeStatus);
        }
    }
}

void Dialog::getAllBlobs()
{

    QJsonDocument allPagesJsonDoc = httpService.getAllBlobs();

    displayResponse(&allPagesJsonDoc);

    QString storeStatus = blobStore.storeBlobs(&allPagesJsonDoc);
    if(storeStatus != "OK") {
        QMessageBox::critical(this, "Error", storeStatus);
    }
}

void Dialog::getBlobById(QString btnName)
{
    int arg = 0;

    if(btnName == "pB_GetById")
    {
        arg = ui->sb_GetIdNumber->value();
    }

    if(btnName == "pB_ViewBlob_AddTag")
    {
        arg = ui->sB_BlobId_AddTag->value();
    }

    if(arg == 0)
    {
        return;
    }

    QString url = QString("http://localhost:8080/blobj/byId?id=%1").arg(arg);

    QJsonDocument blobJ = httpService.getBlob(url);

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

    QJsonDocument blobJList = httpService.getBlob(url);

    displayResponse(&blobJList);
}

void Dialog::getBlobByName()
{
    QString url = QString("http://localhost:8080/blobj/byName?name=%1").arg(ui->lE_BlobJName_Get->text());

    QJsonDocument blobJList = httpService.getBlob(url);

    displayResponse(&blobJList);
}

void Dialog::getBlobByType()
{
    QString url = QString("http://localhost:8080/blobj/byType?type=%1").arg(ui->cB_BlobJType_Get->currentText());

    QJsonDocument blobJList = httpService.getBlob(url);

    displayResponse(&blobJList);
}

void Dialog::saveBlob()
{
    QString BlobJAsText = ui->pTE_View->toPlainText();

    QJsonDocument BlobJAsJson = QJsonDocument::fromJson(BlobJAsText.toUtf8());

    QJsonDocument savedBlob = httpService.saveBlob(QJsonDocument(BlobJAsJson));

    displayResponse(&savedBlob);
}

void Dialog::updateBlob()
{

    QJsonObject bloJToUpdate
    {
        {"id", ui->lE_BlobId_UpdateForm->text()},
        {"name", ui->lE_BlobName_UpdateForm->text()},
        {"sign", ui->lE_BlobSign_UpdateForm->text()},
        {"count", ui->sB_BlobCount_UpdateForm->value()},
        {"rank", ui->sB_BlobRank_UpdateForm->value()},
        {"type", ui->cB_BlobType_UpdateForm->currentText()},
    };

    QJsonDocument bloJToUpdateDoc = QJsonDocument(bloJToUpdate);

    QJsonDocument updatedBlob = httpService.updateBlob(bloJToUpdateDoc);

    displayResponse(&updatedBlob);
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

    QJsonDocument blobJsonDoc = httpService.getBlob(url);

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

void Dialog::deleteBlob()
{    
    int id = ui->sb_DeleteIdNumber->value();

    QByteArray deletedBlob = httpService.deleteBlob(id);

    QJsonDocument deletedBlobJson = QJsonDocument::fromJson(deletedBlob);

    displayResponse(&deletedBlobJson);
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

    QJsonDocument savedBlob = httpService.saveBlob(QJsonDocument(blobJToSave));

    displayResponse(&savedBlob);
}

void Dialog::addTagToBlob()
{

}

void Dialog::displayResponse(QJsonDocument *json)
{
    QString strJson(json->toJson());

    ui->pTE_View->document()->setPlainText(strJson);
}

void Dialog::displayResponse(QString string)
{
    ui->pTE_View->document()->setPlainText(string);
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

void Dialog::getSentder_getAll()
{
    QPushButton *btn = static_cast<QPushButton*>(sender());
    if(!btn) return;

    getAllBlobsByPage(btn->objectName());
}

void Dialog::getSender_getById()
{
    QPushButton *btn = static_cast<QPushButton*>(sender());
    if(!btn) return;

    getBlobById(btn->objectName());
}
