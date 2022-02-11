#include "scriptpropertyeditordlg.h"
#include "ui_scriptpropertyeditordlg.h"
#include "../../shared/confighelper.h"
#include "insertfunctiondlg.h"
#include "inserttagdlg.h"
#include "Qsci/qsciapis.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qscilexerjavascript.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qsciscintilla.h"
#include <QTextStream>
#include <QMessageBox>
#include <QScriptEngine>
#include <QSize>
#include <QSplitter>
#include <QVBoxLayout>
#include <QApplication>

ScriptPropertyEditorDlg::ScriptPropertyEditorDlg(QWidget *parent, QStringList events)
    : QDialog(parent),
      ui(new Ui::ScriptPropertyEditorDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("JavaScript脚本编辑器"));
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    m_nameToShowName.clear();
    m_showNameToName.clear();
    m_supportEvents.clear();
    ui->cboEventTrigger->clear();

    for(int i = 0; i < events.size(); i++) {
        QString szNameToShowName = events.at(i);
        QStringList listNameToShowName = szNameToShowName.split('-');
        if ( listNameToShowName.size() == 2 ) {
            m_supportEvents << listNameToShowName.at(1);
            m_nameToShowName[listNameToShowName.at(0)] = listNameToShowName.at(1);
            m_showNameToName[listNameToShowName.at(1)] = listNameToShowName.at(0);
        }
    }
    ui->cboEventTrigger->addItems(m_supportEvents);

    ///////////////////////////////>>////////////////

    m_scriptEdit = new QsciScintilla(ui->widgetEditor);
    QsciLexerJavaScript *scriptLexer = new QsciLexerJavaScript();      //创建一个词法分析器
    m_scriptEdit->setLexer(scriptLexer);  //设置词法分析器

    m_scriptEdit->setMarginType(0, QsciScintilla::NumberMargin);  //设置编号为0的页边显示行号。
    m_scriptEdit->setMarginLineNumbers(0, true);  //启用行号
    m_scriptEdit->setMarginWidth(0, 26);

    QsciAPIs *apis = new QsciAPIs(scriptLexer);  //代码提示
    apis->add(QString("local"));
    apis->prepare();

    m_scriptEdit->setFont(QFont("Courier New"));  //设置字体
    m_scriptEdit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE, QsciScintilla::SC_CP_UTF8);  //设置编码为UTF-8
    connect(m_scriptEdit, SIGNAL(textChanged()), this, SLOT(documentWasModified()));

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(1, 1, 1, 1);
    vLayout->setObjectName(QStringLiteral("vLayout"));
    vLayout->addWidget(m_scriptEdit);
    ui->widgetEditor->setLayout(vLayout);

    this->resize(700, 500);
}

ScriptPropertyEditorDlg::~ScriptPropertyEditorDlg()
{
    delete m_scriptEdit;
    delete ui;
}

QString ScriptPropertyEditorDlg::getScript()
{
    QStringList listEvScript;

    m_script = m_scriptEdit->text();
    QString szScriptTmp = m_script;
    szScriptTmp = szScriptTmp.replace("\r", "_R");
    szScriptTmp = szScriptTmp.replace("\n", "_N");

    m_event = ui->cboEventTrigger->currentText();
    QString szScriptEvent = m_showNameToName[m_event];

    listEvScript << szScriptEvent << szScriptTmp;
    return listEvScript.join("][");
}


void ScriptPropertyEditorDlg::setScript(const QString &szScript)
{
    QString szScriptTmp = szScript;
    szScriptTmp = szScriptTmp.replace("_R", "\r");
    szScriptTmp = szScriptTmp.replace("_N", "\n");

    if (m_script != szScriptTmp) {
        QStringList listEvScript = szScriptTmp.split("][");
        if(listEvScript.size() == 2) {
            m_event = listEvScript.at(0);
            m_script = listEvScript.at(1);
            m_scriptEdit->setText(m_script);
            QString szScriptEvent = m_nameToShowName[m_event];
            ui->cboEventTrigger->setCurrentText(szScriptEvent);
        }
    }
}


void ScriptPropertyEditorDlg::documentWasModified()
{

}

/*
void ScriptPropertyEditorDlg::resizeEvent(QResizeEvent *event)
{

    QSize size = event->size();
    int h1 = size.height();
    int h2 = ui->widgetTool->size().height();
    QList<int> list;
    list << (h1 - h2) << h2;
    splitter->setSizes(list);
}
*/

/*
*插槽：变量类型Integer
*/
void ScriptPropertyEditorDlg::on_btnTypeInteger_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString("%1\n%2\n").arg("// type Integer").arg("var tInt = 0;");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型Float
*/
void ScriptPropertyEditorDlg::on_btnTypeFloat_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText +=
        QString("%1\n%2\n").arg("// type Float").arg("var tFloat = 0.0;");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型String
*/
void ScriptPropertyEditorDlg::on_btnTypeString_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText +=
        QString("%1\n%2\n").arg("// type String").arg("var tString = \"\";");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型Bool
*/
void ScriptPropertyEditorDlg::on_btnTypeBool_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText +=
        QString("%1\n%2\n").arg("// type Bool").arg("var tBool = false;");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：单击确定
*/
void ScriptPropertyEditorDlg::on_btnOk_clicked()
{
    this->accept();
}

/*
*插槽：单击取消
*/
void ScriptPropertyEditorDlg::on_btnCancel_clicked()
{
    this->reject();
}

/*
*插槽：运算符加
*/
void ScriptPropertyEditorDlg::on_btnOptAdd_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("+");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：运算符减
*/
void ScriptPropertyEditorDlg::on_btnOptSub_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("-");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：运算符乘
*/
void ScriptPropertyEditorDlg::on_btnOptMul_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("*");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：运算符除
*/
void ScriptPropertyEditorDlg::on_btnOptDiv_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("/");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：运算符赋值
*/
void ScriptPropertyEditorDlg::on_btnOptAssign_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("=");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：运算符取余
*/
void ScriptPropertyEditorDlg::on_btnOptMod_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("%");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 >
*/
void ScriptPropertyEditorDlg::on_btnCmpGt_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg(">");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 >=
*/
void ScriptPropertyEditorDlg::on_btnCmpGE_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg(">=");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 <
*/
void ScriptPropertyEditorDlg::on_btnCmpLt_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("<");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 <=
*/
void ScriptPropertyEditorDlg::on_btnCmpLe_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("<=");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 !=
*/
void ScriptPropertyEditorDlg::on_btnCmpNeq_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("!=");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ==
*/
void ScriptPropertyEditorDlg::on_btnCmpEq_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("==");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 &&
*/
void ScriptPropertyEditorDlg::on_btnCmpAnd_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("&&");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 !
*/
void ScriptPropertyEditorDlg::on_btnCmpNot_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("!");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ||
*/
void ScriptPropertyEditorDlg::on_btnCmpOr_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("||");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ^
*/
void ScriptPropertyEditorDlg::on_btnCmpXor_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("^");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 |
*/
void ScriptPropertyEditorDlg::on_btnBitoptOr_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("|");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 ~
*/
void ScriptPropertyEditorDlg::on_btnBitoptInv_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("~");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 &
*/
void ScriptPropertyEditorDlg::on_btnBitoptAnd_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("&");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 ^
*/
void ScriptPropertyEditorDlg::on_btnBitoptXor_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("^");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 <<
*/
void ScriptPropertyEditorDlg::on_btnBitoptLShift_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg("<<");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 >>
*/
void ScriptPropertyEditorDlg::on_btnBitoptRShift_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString(" %1 ").arg(">>");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：语句 if
*/
void ScriptPropertyEditorDlg::on_btnStatementIf_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString("\n%1%2%3\n")
                  .arg("if(condition1)\n{\n\tstatement1;\n}\n")
                  .arg("else if(condition2)\n{\n\tstatement2;\n}\n")
                  .arg("else\n{\n\tstatement3;\n}\n");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：语句 for
*/
void ScriptPropertyEditorDlg::on_btnStatementFor_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText +=
        QString("\n%1%2%3%4\n")
        .arg("for (initialization; expression; post-loop-expression)")
        .arg("\n{")
        .arg("\n\tstatement;")
        .arg("\n}");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：语句 while
*/
void ScriptPropertyEditorDlg::on_btnStatementWhile_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString("\n%1%2%3%4\n")
                  .arg("while (expression)")
                  .arg("\n{")
                  .arg("\n\tstatement;")
                  .arg("\n}");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：语句 switch
*/
void ScriptPropertyEditorDlg::on_btnStatementSwitch_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString("\n%1%2%3%4%5%6%7%8%9\n")
                  .arg("switch (expression)")
                  .arg("\n{")
                  .arg("\n\tcase value: statement;")
                  .arg("\n\t\tbreak;")
                  .arg("\n\tcase value: statement;")
                  .arg("\n\t\tbreak;")
                  .arg("\n\tdefault: statement;")
                  .arg("\n\t\tbreak;")
                  .arg("\n}");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：语句 break
*/
void ScriptPropertyEditorDlg::on_btnStatementBreak_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString("%1;\n").arg("break");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：注释
*/
void ScriptPropertyEditorDlg::on_btnToolComment_clicked()
{
    QString scriptText = m_scriptEdit->text();
    scriptText += QString("%1").arg("//");
    m_scriptEdit->setText(scriptText);
}

/*
*插槽：语法检查
*/
void ScriptPropertyEditorDlg::on_btnToolSyntaxcheck_clicked()
{
    QScriptEngine engine;

    QString scriptFileName(QApplication::applicationDirPath() + "/Config/ScriptFunc.js");
    QFile scriptFile(scriptFileName);
    scriptFile.open(QIODevice::ReadOnly);
    QTextStream stream(&scriptFile);
    QString scriptContentsTmp = stream.readAll();
    scriptFile.close();

    QString scriptContents = m_scriptEdit->text() + "\n" + scriptContentsTmp;
    // qDebug() << "scriptContents:\n" << scriptContents;

    QScriptSyntaxCheckResult syntaxCheckResult =
        engine.checkSyntax(scriptContents);
    if (syntaxCheckResult.state() != QScriptSyntaxCheckResult::Valid) {
        QMessageBox::critical(NULL, "语法错误",
                              QString::fromLatin1("%1 %2 %3")
                              .arg(syntaxCheckResult.errorLineNumber())
                              .arg(syntaxCheckResult.errorColumnNumber())
                              .arg(syntaxCheckResult.errorMessage()));
        return;
    }

    QScriptValue result = engine.evaluate(scriptContents);
    // qDebug() << "result = " << result.toNumber();
    if (result.isError()) {
        QMessageBox::critical(NULL, "错误",
                              QString::fromLatin1("%1: %2")
                              .arg(result.property("lineNumber").toInt32())
                              .arg(result.toString()));
        return;
    }

    QMessageBox::information(
        0, "提示", QString::fromLatin1("%0").arg(tr("不存在语法问题！")));
}

/*
*插槽：插入函数
*/
void ScriptPropertyEditorDlg::on_btnToolFunc_clicked()
{
    InsertFunctionDlg *pDlg = new InsertFunctionDlg(this);
    if (pDlg->exec() == QDialog::Accepted) {
        QString scriptText = m_scriptEdit->text();
        scriptText += QString("%1").arg(pDlg->getSelectedFuncName());
        m_scriptEdit->setText(scriptText);
    }
    delete pDlg;
}

/*
*插槽：插入RTDB变量
*/
void ScriptPropertyEditorDlg::on_btnToolRtdb_clicked()
{
    InsertTagDlg *pDlg = new InsertTagDlg(this);
    if (pDlg->exec() == QDialog::Accepted) {
        QString scriptText = m_scriptEdit->text();
        scriptText += QString("%1").arg(pDlg->getSelectedTagName());
        m_scriptEdit->setText(scriptText);
    }
    delete pDlg;
}



