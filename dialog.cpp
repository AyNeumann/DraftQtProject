#include "dialog.h"
#include "ui_dialog.h"

/**
 * for configuration variables
 * @see: https://forum.qt.io/topic/114664/how-to-set-environment-variables-properly/11
 */
/**
 * @see: https://amin-ahmadi.com/2016/01/17/how-to-send-and-receive-json-requests-in-qt/
 * @see: https://makina-corpus.com/blog/metier/archives/access-json-webservice-qt-c
 * @see: http://erickveil.github.io/2016/04/06/How-To-Manipulate-JSON-With-C++-and-Qt.html
 * @see: Qt official doc: https://doc.qt.io/qt-5/qnetworkaccessmanager.html
 *
 * @see: https://lucidar.me/fr/dev-c-cpp/reading-xml-files-with-qt/
 * @see: https://qt.developpez.com/tutoriels/qtxml/
 *
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

    connect(ui->pB_GetPage, &QPushButton::clicked, this, &Dialog::getSender_getAll);
    connect(ui->pB_GetPageAndSave, &QPushButton::clicked, this, &Dialog::getSender_getAll);
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
    connect(ui->pB_resetSaveForm, &QPushButton::clicked, this, &Dialog::getSender_resetForm);
    connect(ui->pB_resetUpdateForm, &QPushButton::clicked, this, &Dialog::getSender_resetForm);

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
    QString url_types = "type/all";
    QJsonArray types = httpService.getAll_JsonArray(url_types);

    for(int i=0; i< types.count(); ++i){
        ui->cB_BlobType_SaveForm->addItem(types.at(i).toString());
        ui->cB_BlobJType_Get->addItem(types.at(i).toString());
        ui->cB_BlobType_UpdateForm->addItem(types.at(i).toString());
    }

    QString url_tags = "tag/all?pageNumber=0";
    QJsonArray tags = httpService.getPage_JsonArray(url_tags);

    for(int i=0; i< tags.count(); ++i){
        ui->cB_TagName_AddTag->addItem(tags.at(i)["name"].toString());
    }
}

void Dialog::getAllBlobsByPage(QString btnName)
{
    QString url = QString("blobj/all?pageNumber=%1").arg(QString::number(ui->sB_PageNumber->value()));

    QJsonDocument blobList = httpService.get(url);

    displayResponse(blobList);

    if(btnName == "pB_GetPageAndSave")
    {
        QString storeStatus = blobStore.storeBlobsInTempStore(blobList);

        if(storeStatus != "OK") {
            QMessageBox::critical(this, "Error", storeStatus);
        }
    }
}

void Dialog::getAllBlobs()
{

    QString url = "blobj/all?pageNumber=";
    QJsonDocument allPagesJsonDoc = httpService.getAll(url);

    displayResponse(allPagesJsonDoc);

    QString storeStatus = blobStore.storeBlobs(allPagesJsonDoc);
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

    QString url = QString("blobj/byId?id=%1").arg(arg);

    QJsonDocument blobJ = httpService.get(url);

    displayResponse(blobJ);
}

void Dialog::getBlobByCount()
{
    QString url = "";

    if(ui->rB_ExactCount->isChecked())
    {
        url = QString("blobj/byCount?count=%1")
                .arg(ui->sB_BlobJCount1->value());
    }
    if(ui->rB_MinCount->isChecked())
    {
        url = QString("blobj/byCountMin?minCount=%1")
                .arg(ui->sB_BlobJCount1->value());
    }
    if(ui->rB_MaxCount->isChecked())
    {
        url = QString("blobj/byCountMax?maxCount=%1")
                .arg(ui->sB_BlobJCount1->value());
    }
    if(ui->rB_MinMaxCount->isChecked())
    {
        url = QString("blobj/byCountTranche?minCount=%1&maxCount=%2")
                .arg(ui->sB_BlobJCount1->value()).arg(ui->sB_BlobJCount2->value());
    }

    QJsonDocument blobJList = httpService.get(url);

    displayResponse(blobJList);
}

void Dialog::getBlobByName()
{
    QString url = QString("blobj/byName?name=%1").arg(ui->lE_BlobJName_Get->text());

    QJsonDocument blobJList = httpService.get(url);

    displayResponse(blobJList);
}

void Dialog::getBlobByType()
{
    QString url = QString("blobj/byType?type=%1").arg(ui->cB_BlobJType_Get->currentText());

    QJsonDocument blobJList = httpService.get(url);

    displayResponse(blobJList);
}

void Dialog::saveBlob()
{
    QString BlobJAsText = ui->pTE_View->toPlainText();

    QJsonDocument BlobJAsJson = QJsonDocument::fromJson(BlobJAsText.toUtf8());

    QString url = "blobj/save";
    QJsonDocument savedBlob = httpService.save(url, BlobJAsJson);

    displayResponse(savedBlob);
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

    QString url = "blobj/update";
    QJsonDocument updatedBlob = httpService.update(url, bloJToUpdateDoc);

    displayResponse(updatedBlob);
}

void Dialog::getBlobForUpdate()
{
    QString url = "";

    if(!ui->lE_BlobId_UpdateForm->text().isEmpty()) {
        url = QString("blobj/byId?id=%1").arg(ui->lE_BlobId_UpdateForm->text().toInt());
    } else if (!ui->lE_BlobName_UpdateForm->text().isEmpty()) {
        url = QString("blobj/byName?name=%1").arg(ui->lE_BlobName_UpdateForm->text());
    } else if (!ui->lE_BlobName_UpdateForm->text().isEmpty() && !ui->lE_BlobId_UpdateForm->text().isEmpty()) {
        url = QString("blobj/byId?id=%1").arg(ui->lE_BlobId_UpdateForm->text().toInt());
    } else {
        return;
    }

    QJsonDocument blobJsonDoc = httpService.get(url);

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
    displayResponse(blobJsonDoc);
}

void Dialog::deleteBlob()
{    
    int id = ui->sb_DeleteIdNumber->value();

    QString url = "blobj/delete";
    QByteArray deletedBlob = httpService.deleteObj(url, id);

    QString deletedBlobAsString = QString(deletedBlob);

    displayResponse(deletedBlobAsString);
}

void Dialog::saveBlobFromForm()
{
    QJsonObject blobToSaveObj
    {
        {"name", ui->lE_BlobName_SaveForm->text()},
        {"sign", ui->lE_BlobSign_SaveForm->text()},
        {"count", ui->sB_BlobCount_SaveForm->value()},
        {"rank", ui->sB_BlobRank_SaveForm->value()},
        {"type", ui->cB_BlobType_SaveForm->currentText()},
    };

    QJsonDocument blobToSaveDoc = QJsonDocument(blobToSaveObj);
    QString url = "blobj/save";
    QJsonDocument savedBlob = httpService.save(url, blobToSaveDoc);

    displayResponse(savedBlob);
}

void Dialog::addTagToBlob()
{

}

void Dialog::displayResponse(QJsonDocument &json)
{
    QString strJson(json.toJson());

    ui->pTE_View->document()->setPlainText(strJson);
}

void Dialog::displayResponse(QString &string)
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

void Dialog::resetForm(QString btnName)
{
    QList<QLineEdit*> lineEditList;
    QList<QSpinBox*> spinBoxList;
    QList<QComboBox*> comboBoxList;

    if(btnName == "pB_resetSaveForm")
    {
        lineEditList = ui->grpB_SaveForm->findChildren<QLineEdit*>();
        spinBoxList = ui->grpB_SaveForm->findChildren<QSpinBox*>();
        comboBoxList = ui->grpB_SaveForm->findChildren<QComboBox*>();
    }

    if(btnName == "pB_resetUpdateForm")
    {
        lineEditList = ui->grpB_UpdateForm->findChildren<QLineEdit*>();
        spinBoxList = ui->grpB_UpdateForm->findChildren<QSpinBox*>();
        comboBoxList = ui->grpB_UpdateForm->findChildren<QComboBox*>();
    }

    foreach(auto obj, lineEditList)
    {
        obj->clear();
    }

    foreach(auto obj, spinBoxList)
    {
        obj->clear();
    }

    foreach(auto obj, comboBoxList)
    {
        obj->setCurrentIndex(0);
    }

    /*if(btnName == "pB_resetSaveForm")
    {
        ui->lE_BlobName_SaveForm->clear();
        ui->lE_BlobSign_SaveForm->clear();
        ui->sB_BlobCount_SaveForm->setValue(0);
        ui->sB_BlobRank_SaveForm->setValue(0);
        ui->cB_BlobType_SaveForm->setCurrentIndex(0);
    }

    if(btnName == "pB_resetUpdateForm")
    {
        ui->lE_BlobId_UpdateForm->clear();
        ui->lE_BlobName_UpdateForm->clear();
        ui->lE_BlobSign_UpdateForm->clear();
        ui->sB_BlobCount_UpdateForm->setValue(0);
        ui->sB_BlobRank_UpdateForm->setValue(0);
        ui->cB_BlobType_UpdateForm->setCurrentIndex(0);
    }*/
}

void Dialog::getSender_getAll()
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

void Dialog::getSender_resetForm()
{
    QPushButton *btn = static_cast<QPushButton*>(sender());
    if(!btn) return;

    resetForm(btn->objectName());
}
