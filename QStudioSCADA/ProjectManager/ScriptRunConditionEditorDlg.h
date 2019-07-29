#ifndef SCRIPTRUNCONDITIONEDITORDLG_H
#define SCRIPTRUNCONDITIONEDITORDLG_H

#include <QDialog>

namespace Ui {
class ScriptRunConditionEditorDlg;
}

class ScriptRunConditionEditorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptRunConditionEditorDlg(QString projectPath, QWidget *parent = 0);
    ~ScriptRunConditionEditorDlg();
    QString getConditionString() const;
    void setConditionString(QString condition);

private slots:
    void on_btnVarConst_clicked();
    void on_btnVarLeft_clicked();
    void on_btnVarRight_clicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_radioBtnVarConst_clicked(bool checked);
    void on_radioBtnVarVar_clicked(bool checked);

private:
    Ui::ScriptRunConditionEditorDlg *ui;
    QString m_strProjectPath;
    QString m_strCondition;
};

#endif // SCRIPTRUNCONDITIONEDITORDLG_H
