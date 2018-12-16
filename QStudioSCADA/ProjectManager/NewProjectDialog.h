#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "../Public/Public.h"
#include <QDialog>
#include <QMap>
#include <QJsonObject>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();

public:
    bool loadFromFile(SaveFormat saveFormat, QString file);
    bool saveToFile(SaveFormat saveFormat);
    QString GetProjectName();

private:
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
    bool check_data();

private slots:
    void on_btnFileDialog_clicked();
    void on_btnHelp_clicked();
    void on_btnCheck_clicked();
    void on_btnOk_clicked();
    void on_btnExit_clicked();

private:
    Ui::NewProjectDialog *ui;
    QMap<QString, int>  m_DeviceMap;
    QString m_strProjectName;
};

#endif // NEWPROJECTDIALOG_H
