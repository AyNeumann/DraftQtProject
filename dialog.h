#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QDebug>
#include <QJsonDocument>

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

private slots:
    void getAllBlobJs();
    void getBlobById(QString btnName);
    void getBlobByCount();
    void getBlobByName();
    void getBlobByType();
    void getBlobForUpdate();
    void saveBlob();
    void deleteBlob();
    void displayResponse(QJsonDocument *json);
    void saveBlobFromForm();
    void saveBlobInDB(QJsonDocument blobJToSave);
    void addTagToBlob();

    QJsonDocument getBlobJFromDB(QString url);
    QJsonArray getAllBlobJTypes();
    QJsonArray getAllTags();

    void checkCountRadioButton();
    void getSender();
};
#endif // DIALOG_H
