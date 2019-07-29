#include "ScriptEditorDlg.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include "InsertFunctionDialog.h"
#include "InsertTagDialog.h"
#include "Qsci/qsciapis.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qscilexerjavascript.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qsciscintilla.h"
#include "ui_ScriptEditorDlg.h"

#include <QFile>
#include <QMessageBox>
#include <QScriptEngine>
#include <QSize>
#include <QSplitter>
#include <QTextCodec>
#include <QTextStream>
#include <QVBoxLayout>

#include <QDebug>

ScriptEditorDlg::ScriptEditorDlg(QString projectPath, QWidget *parent)
    : QDialog(parent),
      m_strProjectPath(projectPath),
      ui(new Ui::ScriptEditorDlg) {
  ui->setupUi(this);
  this->setWindowTitle(tr("JavaScript脚本编辑器"));

  ///////////////////////////////>>////////////////

  scriptEdit = new QsciScintilla(ui->widgetEditor);
  QsciLexerJavaScript *scriptLexer =
      new QsciLexerJavaScript();      //创建一个词法分析器
  scriptEdit->setLexer(scriptLexer);  //设置词法分析器

  scriptEdit->setMarginType(
      0, QsciScintilla::NumberMargin);  //设置编号为0的页边显示行号。
  scriptEdit->setMarginLineNumbers(0, true);  //启用行号
  scriptEdit->setMarginWidth(0, 26);

  QsciAPIs *apis = new QsciAPIs(scriptLexer);  //代码提示
  apis->add(QString("local"));
  apis->prepare();

  scriptEdit->setFont(QFont("Courier New"));  //设置字体
  scriptEdit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,
                            QsciScintilla::SC_CP_UTF8);  //设置编码为UTF-8
  connect(scriptEdit, SIGNAL(textChanged()), this, SLOT(documentWasModified()));

  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(1, 1, 1, 1);
  vLayout->setObjectName(QStringLiteral("vLayout"));
  vLayout->addWidget(scriptEdit);
  ui->widgetEditor->setLayout(vLayout);

  ////////////////////////////////<<///////////////
  /*
      scriptEdit->setMinimumSize(600, 260);

      ui->btnTypeInteger->setMaximumSize(60, 26);
      ui->btnTypeFloat->setMaximumSize(60, 26);
      ui->btnTypeBool->setMaximumSize(60, 26);
      ui->btnTypeString->setMaximumSize(60, 26);

      ui->btnOptAdd->setMaximumSize(40, 26);
      ui->btnOptSub->setMaximumSize(40, 26);
      ui->btnOptMul->setMaximumSize(40, 26);
      ui->btnOptDiv->setMaximumSize(40, 26);
      ui->btnOptAssign->setMaximumSize(40, 26);
      ui->btnOptMod->setMaximumSize(40, 26);

      ui->btnCmpAnd->setMaximumSize(32, 26);
      ui->btnCmpEq->setMaximumSize(32, 26);
      ui->btnCmpGE->setMaximumSize(32, 26);
      ui->btnCmpGt->setMaximumSize(32, 26);
      ui->btnCmpLe->setMaximumSize(32, 26);
      ui->btnCmpLt->setMaximumSize(32, 26);
      ui->btnCmpNeq->setMaximumSize(32, 26);
      ui->btnCmpNot->setMaximumSize(32, 26);
      ui->btnCmpOr->setMaximumSize(32, 26);
      ui->btnCmpXor->setMaximumSize(32, 26);

      ui->btnBitoptAnd->setMaximumSize(32, 26);
      ui->btnBitoptInv->setMaximumSize(32, 26);
      ui->btnBitoptLShift->setMaximumSize(32, 26);
      ui->btnBitoptOr->setMaximumSize(32, 26);
      ui->btnBitoptRShift->setMaximumSize(32, 26);
      ui->btnBitoptXor->setMaximumSize(32, 26);

      ui->btnStatementBreak->setMaximumSize(70, 26);
      ui->btnStatementFor->setMaximumSize(70, 26);
      ui->btnStatementIf->setMaximumSize(70, 26);
      ui->btnStatementSwitch->setMaximumSize(70, 26);
      ui->btnStatementWhile->setMaximumSize(70, 26);

      ui->btnToolComment->setMaximumSize(96, 26);
      ui->btnToolFunc->setMaximumSize(96, 26);
      ui->btnToolRtdb->setMaximumSize(96, 26);
      ui->btnToolSyntaxcheck->setMaximumSize(96, 26);

      ui->btnOk->setMaximumSize(52, 26);
      ui->btnCancel->setMaximumSize(52, 26);
  */
  this->resize(700, 500);
}

ScriptEditorDlg::~ScriptEditorDlg() {
  delete scriptEdit;
  delete ui;
}

void ScriptEditorDlg::documentWasModified() {}

/*
void ScriptEditorDlg::resizeEvent(QResizeEvent *event)
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
void ScriptEditorDlg::on_btnTypeInteger_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString("%1\n%2\n").arg("// type Integer").arg("var tInt = 0;");
  scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型Float
*/
void ScriptEditorDlg::on_btnTypeFloat_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText +=
      QString("%1\n%2\n").arg("// type Float").arg("var tFloat = 0.0;");
  scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型String
*/
void ScriptEditorDlg::on_btnTypeString_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText +=
      QString("%1\n%2\n").arg("// type String").arg("var tString = \"\";");
  scriptEdit->setText(scriptText);
}

/*
*插槽：变量类型Bool
*/
void ScriptEditorDlg::on_btnTypeBool_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText +=
      QString("%1\n%2\n").arg("// type Bool").arg("var tBool = false;");
  scriptEdit->setText(scriptText);
}

/*
*插槽：单击确定
*/
void ScriptEditorDlg::on_btnOk_clicked() { this->accept(); }

/*
*插槽：单击取消
*/
void ScriptEditorDlg::on_btnCancel_clicked() { this->reject(); }

/*
*插槽：运算符加
*/
void ScriptEditorDlg::on_btnOptAdd_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("+");
  scriptEdit->setText(scriptText);
}

/*
*插槽：运算符减
*/
void ScriptEditorDlg::on_btnOptSub_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("-");
  scriptEdit->setText(scriptText);
}

/*
*插槽：运算符乘
*/
void ScriptEditorDlg::on_btnOptMul_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("*");
  scriptEdit->setText(scriptText);
}

/*
*插槽：运算符除
*/
void ScriptEditorDlg::on_btnOptDiv_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("/");
  scriptEdit->setText(scriptText);
}

/*
*插槽：运算符赋值
*/
void ScriptEditorDlg::on_btnOptAssign_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("=");
  scriptEdit->setText(scriptText);
}

/*
*插槽：运算符取余
*/
void ScriptEditorDlg::on_btnOptMod_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("%");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 >
*/
void ScriptEditorDlg::on_btnCmpGt_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg(">");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 >=
*/
void ScriptEditorDlg::on_btnCmpGE_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg(">=");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 <
*/
void ScriptEditorDlg::on_btnCmpLt_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("<");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 <=
*/
void ScriptEditorDlg::on_btnCmpLe_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("<=");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 !=
*/
void ScriptEditorDlg::on_btnCmpNeq_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("!=");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ==
*/
void ScriptEditorDlg::on_btnCmpEq_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("==");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 &&
*/
void ScriptEditorDlg::on_btnCmpAnd_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("&&");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 !
*/
void ScriptEditorDlg::on_btnCmpNot_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("!");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ||
*/
void ScriptEditorDlg::on_btnCmpOr_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("||");
  scriptEdit->setText(scriptText);
}

/*
*插槽：比较操作 ^
*/
void ScriptEditorDlg::on_btnCmpXor_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("^");
  scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 |
*/
void ScriptEditorDlg::on_btnBitoptOr_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("|");
  scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 ~
*/
void ScriptEditorDlg::on_btnBitoptInv_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("~");
  scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 &
*/
void ScriptEditorDlg::on_btnBitoptAnd_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("&");
  scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 ^
*/
void ScriptEditorDlg::on_btnBitoptXor_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("^");
  scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 <<
*/
void ScriptEditorDlg::on_btnBitoptLShift_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg("<<");
  scriptEdit->setText(scriptText);
}

/*
*插槽：位操作 >>
*/
void ScriptEditorDlg::on_btnBitoptRShift_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString(" %1 ").arg(">>");
  scriptEdit->setText(scriptText);
}

/*
*插槽：语句 if
*/
void ScriptEditorDlg::on_btnStatementIf_clicked() {
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
void ScriptEditorDlg::on_btnStatementFor_clicked() {
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
void ScriptEditorDlg::on_btnStatementWhile_clicked() {
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
void ScriptEditorDlg::on_btnStatementSwitch_clicked() {
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
void ScriptEditorDlg::on_btnStatementBreak_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString("%1;\n").arg("break");
  scriptEdit->setText(scriptText);
}

/*
*插槽：注释
*/
void ScriptEditorDlg::on_btnToolComment_clicked() {
  QString scriptText = scriptEdit->text();
  scriptText += QString("%1").arg("//");
  scriptEdit->setText(scriptText);
}

/*
*插槽：语法检查
*/
void ScriptEditorDlg::on_btnToolSyntaxcheck_clicked() {
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
    QMessageBox::critical(0, "语法错误",
                          QString::fromLatin1("%1 %2 %3")
                              .arg(syntaxCheckResult.errorLineNumber())
                              .arg(syntaxCheckResult.errorColumnNumber())
                              .arg(syntaxCheckResult.errorMessage()));
    return;
  }

  QScriptValue result = engine.evaluate(scriptContents);
  // qDebug() << "result = " << result.toNumber();
  if (result.isError()) {
    QMessageBox::critical(0, "错误",
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
void ScriptEditorDlg::on_btnToolFunc_clicked() {
  InsertFunctionDialog *pDlg = new InsertFunctionDialog(this);
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
void ScriptEditorDlg::on_btnToolRtdb_clicked() {
  InsertTagDialog *pDlg = new InsertTagDialog(m_strProjectPath, this);
  if (pDlg->exec() == QDialog::Accepted) {
    QString scriptText = scriptEdit->text();
    scriptText += QString("%1").arg(pDlg->getSelectedTagName());
    scriptEdit->setText(scriptText);
  }
  delete pDlg;
}

void ScriptEditorDlg::load(const QString scriptName) {
  scriptEdit->setText(Helper::readString(scriptName));
}

void ScriptEditorDlg::save(const QString scriptName) {
  Helper::writeString(scriptName, scriptEdit->text());
}
