#ifndef SCRIPTEDITORDLG_H
#define SCRIPTEDITORDLG_H

#include <QDialog>
#include <QResizeEvent>
#include <QStringList>
#include <QSplitter>

namespace Ui {
class ScriptEditorDlg;
}

class QsciScintilla;

class ScriptEditorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptEditorDlg(QString projectPath, QWidget *parent = 0);
    ~ScriptEditorDlg();
    void load(const QString scriptName);
    void save(const QString scriptName);

protected:
    //void resizeEvent(QResizeEvent *event);

private slots:
    void documentWasModified();
    void on_btnTypeInteger_clicked();
    void on_btnTypeFloat_clicked();
    void on_btnTypeString_clicked();
    void on_btnTypeBool_clicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_btnOptAdd_clicked();
    void on_btnOptSub_clicked();
    void on_btnOptMul_clicked();
    void on_btnOptDiv_clicked();
    void on_btnOptAssign_clicked();
    void on_btnOptMod_clicked();
    void on_btnCmpGt_clicked();
    void on_btnCmpGE_clicked();
    void on_btnCmpLt_clicked();
    void on_btnCmpLe_clicked();
    void on_btnCmpNeq_clicked();
    void on_btnCmpEq_clicked();
    void on_btnCmpAnd_clicked();
    void on_btnCmpNot_clicked();
    void on_btnCmpOr_clicked();
    void on_btnCmpXor_clicked();
    void on_btnBitoptOr_clicked();
    void on_btnBitoptInv_clicked();
    void on_btnBitoptAnd_clicked();
    void on_btnBitoptXor_clicked();
    void on_btnBitoptLShift_clicked();
    void on_btnBitoptRShift_clicked();
    void on_btnStatementIf_clicked();
    void on_btnStatementFor_clicked();
    void on_btnStatementWhile_clicked();
    void on_btnStatementSwitch_clicked();
    void on_btnStatementBreak_clicked();
    void on_btnToolComment_clicked();
    void on_btnToolSyntaxcheck_clicked();
    void on_btnToolFunc_clicked();
    void on_btnToolRtdb_clicked();

private:
    Ui::ScriptEditorDlg *ui;
    QsciScintilla *scriptEdit;
    QString m_strProjectPath;
};

#endif // SCRIPTEDITORDLG_H
