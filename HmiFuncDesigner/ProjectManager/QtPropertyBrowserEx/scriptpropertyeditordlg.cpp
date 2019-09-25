#include "scriptpropertyeditordlg.h"
#include "ui_scriptpropertyeditordlg.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include "insertfunctiondlg.h"
#include "inserttagdlg.h"
#include "Qsci/qsciapis.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qscilexerjavascript.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qsciscintilla.h"

#include <QMessageBox>
#include <QScriptEngine>
#include <QSize>
#include <QSplitter>
#include <QVBoxLayout>

#include <QDebug>

ScriptPropertyEditorDlg::ScriptPropertyEditorDlg(QWidget *parent, QStringList events)
    : QDialog(parent),
      ui(new Ui::ScriptPropertyEditorDlg) {
    ui->setupUi(this);
    this->setWindowTitle(tr("JavaScript脚本编辑器"));

    mapNameToShowName_.clear();
    mapShowNameToName_.clear();
    supportEvents_.clear();

    for(int i=0; i<events.size(); i++) {
        QString szNameToShowName = events.at(i);
        QStringList listNameToShowName = szNameToShowName.split('-');
        if ( listNameToShowName.size() == 2 ) {
            supportEvents_ << listNameToShowName.at(1);
            mapNameToShowName_[listNameToShowName.at(0)] = listNameToShowName.at(1);
            mapShowNameToName_[listNameToShowName.at(1)] = listNameToShowName.at(0);
        }
    }

    ///////////////////////////////>>////////////////

    scriptEdit = new QsciScintilla(ui->widgetEditor);
    QsciLexerJavaScript *scriptLexer =
            new QsciLexerJavaScript();      //创建一个词法分析器
    scriptEdit->setLexer(scriptLexer);  //设置词法分析器

    scriptEdit->setMarginType(0, QsciScintilla::NumberMargin);  //设置编号为0的页边显示行号。
    scriptEdit->setMarginLineNumbers(0, true);  //启用行号
    scriptEdit->setMarginWidth(0, 26);

    QsciAPIs *apis = new QsciAPIs(scriptLexer);  //代码提示
    apis->add(QString("local"));
    apis->prepare();

    scriptEdit->setFont(QFont("Courier New"));  //设置字体
    scriptEdit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE, QsciScintilla::SC_CP_UTF8);  //设置编码为UTF-8
    connect(scriptEdit, SIGNAL(textChanged()), this, SLOT(documentWasModified()));

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(1, 1, 1, 1);
    vLayout->setObjectName(QStringLiteral("vLayout"));
    vLayout->addWidget(scriptEdit);
    ui->widgetEditor->setLayout(vLayout);

    this->resize(700, 500);
}

ScriptPropertyEditorDlg::~ScriptPropertyEditorDlg() {
    delete scriptEdit;
    delete ui;
}

QString ScriptPropertyEditorDlg::getScript()
{
    script_ = scriptEdit->text();
    return script_;
}


void ScriptPropertyEditorDlg::setScript(const QString &script)
{
    script_ = script;
    scriptEdit->setText(script_);
}

void ScriptPropertyEditorDlg::documentWasModified() {}

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
void ScriptPropertyEditorDlg::on_btnTypeInteger_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString("%1\n%2\n").arg("// type Integer").arg("var tInt = 0;");
    scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型Float
*/
void ScriptPropertyEditorDlg::on_btnTypeFloat_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText +=
            QString("%1\n%2\n").arg("// type Float").arg("var tFloat = 0.0;");
    scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型String
*/
void ScriptPropertyEditorDlg::on_btnTypeString_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText +=
            QString("%1\n%2\n").arg("// type String").arg("var tString = \"\";");
    scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型Bool
*/
void ScriptPropertyEditorDlg::on_btnTypeBool_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText +=
            QString("%1\n%2\n").arg("// type Bool").arg("var tBool = false;");
    scriptEdit->setText(scriptText);
}

/*
*插槽：单击确定
*/
void ScriptPropertyEditorDlg::on_btnOk_clicked() { this->accept(); }

/*
*插槽：单击取消
*/
void ScriptPropertyEditorDlg::on_btnCancel_clicked() { this->reject(); }

/*
*插槽：运算符加
*/
void ScriptPropertyEditorDlg::on_btnOptAdd_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("+");
    scriptEdit->setText(scriptText);
}

/*
*插槽：运算符减
*/
void ScriptPropertyEditorDlg::on_btnOptSub_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("-");
    scriptEdit->setText(scriptText);
}

/*
*插槽：运算符乘
*/
void ScriptPropertyEditorDlg::on_btnOptMul_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("*");
    scriptEdit->setText(scriptText);
}

/*
*插槽：运算符除
*/
void ScriptPropertyEditorDlg::on_btnOptDiv_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("/");
    scriptEdit->setText(scriptText);
}

/*
*插槽：运算符赋值
*/
void ScriptPropertyEditorDlg::on_btnOptAssign_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("=");
    scriptEdit->setText(scriptText);
}

/*
*插槽：运算符取余
*/
void ScriptPropertyEditorDlg::on_btnOptMod_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("%");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 >
*/
void ScriptPropertyEditorDlg::on_btnCmpGt_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg(">");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 >=
*/
void ScriptPropertyEditorDlg::on_btnCmpGE_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg(">=");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 <
*/
void ScriptPropertyEditorDlg::on_btnCmpLt_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("<");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 <=
*/
void ScriptPropertyEditorDlg::on_btnCmpLe_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("<=");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 !=
*/
void ScriptPropertyEditorDlg::on_btnCmpNeq_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("!=");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ==
*/
void ScriptPropertyEditorDlg::on_btnCmpEq_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("==");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 &&
*/
void ScriptPropertyEditorDlg::on_btnCmpAnd_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("&&");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 !
*/
void ScriptPropertyEditorDlg::on_btnCmpNot_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("!");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ||
*/
void ScriptPropertyEditorDlg::on_btnCmpOr_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("||");
    scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ^
*/
void ScriptPropertyEditorDlg::on_btnCmpXor_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("^");
    scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 |
*/
void ScriptPropertyEditorDlg::on_btnBitoptOr_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("|");
    scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 ~
*/
void ScriptPropertyEditorDlg::on_btnBitoptInv_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("~");
    scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 &
*/
void ScriptPropertyEditorDlg::on_btnBitoptAnd_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("&");
    scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 ^
*/
void ScriptPropertyEditorDlg::on_btnBitoptXor_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("^");
    scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 <<
*/
void ScriptPropertyEditorDlg::on_btnBitoptLShift_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg("<<");
    scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 >>
*/
void ScriptPropertyEditorDlg::on_btnBitoptRShift_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString(" %1 ").arg(">>");
    scriptEdit->setText(scriptText);
}

/*
*插槽：语句 if
*/
void ScriptPropertyEditorDlg::on_btnStatementIf_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString("\n%1%2%3\n")
            .arg("if(condition1)\n{\n\tstatement1;\n}\n")
            .arg("else if(condition2)\n{\n\tstatement2;\n}\n")
            .arg("else\n{\n\tstatement3;\n}\n");
    scriptEdit->setText(scriptText);
}

/*
*插槽：语句 for
*/
void ScriptPropertyEditorDlg::on_btnStatementFor_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText +=
            QString("\n%1%2%3%4\n")
            .arg("for (initialization; expression; post-loop-expression)")
            .arg("\n{")
            .arg("\n\tstatement;")
            .arg("\n}");
    scriptEdit->setText(scriptText);
}

/*
*插槽：语句 while
*/
void ScriptPropertyEditorDlg::on_btnStatementWhile_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString("\n%1%2%3%4\n")
            .arg("while (expression)")
            .arg("\n{")
            .arg("\n\tstatement;")
            .arg("\n}");
    scriptEdit->setText(scriptText);
}

/*
*插槽：语句 switch
*/
void ScriptPropertyEditorDlg::on_btnStatementSwitch_clicked() {
    QString scriptText = scriptEdit->text();
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
    scriptEdit->setText(scriptText);
}

/*
*插槽：语句 break
*/
void ScriptPropertyEditorDlg::on_btnStatementBreak_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString("%1;\n").arg("break");
    scriptEdit->setText(scriptText);
}

/*
*插槽：注释
*/
void ScriptPropertyEditorDlg::on_btnToolComment_clicked() {
    QString scriptText = scriptEdit->text();
    scriptText += QString("%1").arg("//");
    scriptEdit->setText(scriptText);
}

/*
*插槽：语法检查
*/
void ScriptPropertyEditorDlg::on_btnToolSyntaxcheck_clicked() {
    QScriptEngine engine;

    QString scriptFileName(Helper::AppDir() + "/Config/ScriptFunc.js");
    QFile scriptFile(scriptFileName);
    scriptFile.open(QIODevice::ReadOnly);
    QTextStream stream(&scriptFile);
    QString scriptContentsTmp = stream.readAll();
    scriptFile.close();

    QString scriptContents = scriptEdit->text() + "\n" + scriptContentsTmp;
    // qDebug() << "scriptContents:\n" << scriptContents;

    QScriptSyntaxCheckResult syntaxCheckResult =
            engine.checkSyntax(scriptContents);
    if (syntaxCheckResult.state() != QScriptSyntaxCheckResult::Valid) {
        QMessageBox::critical(Q_NULLPTR, "语法错误",
                              QString::fromLatin1("%1 %2 %3")
                              .arg(syntaxCheckResult.errorLineNumber())
                              .arg(syntaxCheckResult.errorColumnNumber())
                              .arg(syntaxCheckResult.errorMessage()));
        return;
    }

    QScriptValue result = engine.evaluate(scriptContents);
    // qDebug() << "result = " << result.toNumber();
    if (result.isError()) {
        QMessageBox::critical(Q_NULLPTR, "错误",
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
void ScriptPropertyEditorDlg::on_btnToolFunc_clicked() {
    InsertFunctionDlg *pDlg = new InsertFunctionDlg(this);
    if (pDlg->exec() == QDialog::Accepted) {
        QString scriptText = scriptEdit->text();
        scriptText += QString("%1").arg(pDlg->getSelectedFuncName());
        scriptEdit->setText(scriptText);
    }
    delete pDlg;
}

/*
*插槽：插入RTDB变量
*/
void ScriptPropertyEditorDlg::on_btnToolRtdb_clicked() {
    InsertTagDlg *pDlg = new InsertTagDlg(this);
    if (pDlg->exec() == QDialog::Accepted) {
        QString scriptText = scriptEdit->text();
        scriptText += QString("%1").arg(pDlg->getSelectedTagName());
        scriptEdit->setText(scriptText);
    }
    delete pDlg;
}


