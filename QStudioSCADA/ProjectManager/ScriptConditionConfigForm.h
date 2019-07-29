#ifndef SCRIPTCONDITIONCONFIGFORM_H
#define SCRIPTCONDITIONCONFIGFORM_H

#include <QDialog>

namespace Ui {
class ScriptConditionConfigForm;
}

class ScriptConditionConfigForm : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptConditionConfigForm(QString projectPath, QWidget *parent = 0);
    ~ScriptConditionConfigForm();

    QString GetName();
    void SetName(const QString &name);

    bool isInUse();
    void SetInUse(bool b);

    QString GetDescription();
    void SetDescription(const QString &des);

    QString GetRunMode();
    void SetRunMode(const QString &run);

    QString GetRunModeArgs();
    void SetRunModeArgs(const QString &args);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_btnEdit_clicked();

private:
    Ui::ScriptConditionConfigForm *ui;
    QString m_strProjectPath;
};

#endif // SCRIPTCONDITIONCONFIGFORM_H
