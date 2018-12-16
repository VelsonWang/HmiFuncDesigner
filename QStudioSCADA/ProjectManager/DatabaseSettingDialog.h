#ifndef DATABASESETTINGDIALOG_H
#define DATABASESETTINGDIALOG_H

#include "../Public/Public.h"
#include <QDialog>
#include <QJsonObject>

namespace Ui {
class DatabaseSettingDialog;
}

class DatabaseSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseSettingDialog(QWidget *parent = 0, QString ProjectPath = "");
    ~DatabaseSettingDialog();

public:
    bool loadFromFile(SaveFormat saveFormat);
    bool saveToFile(SaveFormat saveFormat);

private:
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
    bool check_data();

private slots:
    void on_btnHelp_clicked();
    void on_btnCheck_clicked();
    void on_btnOk_clicked();
    void on_btnExit_clicked();

private:
    Ui::DatabaseSettingDialog *ui;
    QString m_ProjectPath;
};

#endif // DATABASESETTINGDIALOG_H
