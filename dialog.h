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
    void initDataBindUi();
    void getBlobById(QString btnName);
    void displayResponse(QJsonDocument *json);
    void saveBlobInDB(QJsonDocument blobJToSave);
    void updateBlobInDB(QJsonDocument blobJToUpdate);
    QJsonDocument getBlobFromDB(QString url);
    QJsonArray getAllBlobTypes();
    QJsonArray getAllTags();

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
    void getSender();

    QJsonDocument handleHTTPErrors(QByteArray response_data, QNetworkReply *reply);
};
#endif // DIALOG_H
