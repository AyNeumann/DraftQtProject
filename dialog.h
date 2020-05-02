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

#include "httpservice.h"

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
    void getBlobById(QString btnName);
    void displayResponse(QJsonDocument *json);

private slots:
    void getAllBlobs();
    void getAllBlobsAndSave();
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
    void getSender();
};
#endif // DIALOG_H
