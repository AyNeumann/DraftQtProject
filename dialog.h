#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QDebug>

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
    void getBlobJById();
    void saveBlobJ();
    void deleteBlobJ();
    void displayResponse(QJsonDocument *json);
    void saveBlobJFromForm();
    void saveBlobJInDB(QJsonDocument blobJToSave);
};
#endif // DIALOG_H
