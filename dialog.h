#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include <QPixmap>
#include <QIcon>

#include "http_service.h"
#include "blob_store.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;

    void init();
    void initDataBindUi();
    void getAllBlobsByPage(QString btnName);
    void getBlobById(QString btnName);
    void displayResponse(QJsonDocument *json);
    void displayResponse(QString string);

private slots:
    void getAllBlobs();
    void getBlobByCount();
    void getBlobByName();
    void getBlobByType();
    void getBlobForUpdate();
    void saveBlob();
    void updateBlob();
    void deleteBlob();
    void saveBlobFromForm();
    void addTagToBlob();

    void checkCountRadioButton();
    void getSentder_getAll();
    void getSender_getById();
};
#endif // DIALOG_H
