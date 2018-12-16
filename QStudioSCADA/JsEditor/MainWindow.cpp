#include "MainWindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>
#include <QTreeWidgetItem>
#include <QDesktopWidget>
#include <QRect>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerjavascript.h>
#include "mainwindow.h"

MainWindow::MainWindow(QString projPath, QString jsFile, QWidget *parent) :
    QMainWindow(parent),
    m_strProjPath(projPath),
    m_strJsFile(jsFile),
    pScriptGeneratorForm(NULL)
{
    textEdit = new QsciScintilla;
    setCentralWidget(textEdit);

    ///////////////////////////////>>////////////////

    QsciLexerJavaScript *textLexer = new QsciLexerJavaScript;//创建一个词法分析器
    textEdit->setLexer(textLexer);//给QsciScintilla设置词法分析器

    textEdit->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
    textEdit->setMarginLineNumbers(0,true);//对该页边启用行号
    textEdit->setMarginWidth(0,50);//设置页边宽度

    //实现代码提示
    QsciAPIs *apis = new QsciAPIs(textLexer);
    apis->add(QString("local"));
    apis->prepare();
#if 0
但是如果你要提示的关键字很多，那就考虑把关键字存到文件，每个关键词为一行，然后调用load方法从文件加载了。
        QsciAPIs*apis = new QsciAPIs(textLexer);
           if(!apis->load(QString("D://api.txt")))
              QMessageBox::warning(this,QString("提示"),QString("读取文件失败"));
           else
              apis->prepare();


还可以对代码提示配置其他选项。
        editor->setAutoCompletionSource(QsciScintilla::AcsAll);   //设置源
        editor->setAutoCompletionCaseSensitivity(true);   //设置自动补全大小写敏感
        editor->setAutoCompletionThreshold(1);    //设置每输入一个字符就会出现自动补全的提示
#endif
    //设置字体
    textEdit->setFont(QFont("Courier New"));
    //设置编码
    textEdit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);//设置编码为UTF-8

    ////////////////////////////////<<///////////////

    createUI();
    readSettings();

    connect(textEdit, SIGNAL(textChanged()),
            this, SLOT(documentWasModified()));

    setCurrentFile("");

    showScriptPropertyConfigDlg();

}

MainWindow::~MainWindow()
{

    if(pScriptGeneratorForm != NULL)
    {
        delete pScriptGeneratorForm;
        pScriptGeneratorForm = NULL;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->isModified());
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("Trolltech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("Trolltech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSave()
{
    if (textEdit->isModified()) {
        int ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


QDockWidget *dockObjectOpt;
QDockWidget *dockObjectMsg;
QDockWidget *dockExecCondition;
QDockWidget *dockScriptGenerator;

void MainWindow::createUI()
{
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockObjectOpt();
    createDockObjectMsg();
    createDockExecCondition();
    createDockScriptGenerator();
}



void MainWindow::createDockObjectOpt()
{
    dockObjectOpt = new QDockWidget(this);
    dockObjectOpt->setObjectName(QStringLiteral("dockObjectOpt"));

    treeWidgetObjectOpt = new QTreeWidget(dockObjectOpt);
    treeWidgetObjectOpt->setObjectName(QStringLiteral("treeWidgetObjectOpt"));
    treeWidgetObjectOpt->setHeaderLabel(tr("操作对象"));
    QFont font;
    font.setPointSize(10);
    treeWidgetObjectOpt->setFont(font);

    treeWidgetObjectOpt->setColumnCount(1);
    QTreeWidgetItem *rootVar = new QTreeWidgetItem(treeWidgetObjectOpt, QStringList(QString(tr("变量"))));
    rootVar->setIcon(0, QIcon(":/images/node.png"));
    QTreeWidgetItem *devVarRoot = new QTreeWidgetItem(rootVar, QStringList(QString(tr("设备变量"))));
    devVarRoot->setIcon(0, QIcon(":/images/subnode.png"));
    rootVar->addChild(devVarRoot);
    QTreeWidgetItem *tmpVarRoot = new QTreeWidgetItem(rootVar, QStringList(QString(tr("中间变量"))));
    tmpVarRoot->setIcon(0, QIcon(":/images/subnode.png"));
    rootVar->addChild(tmpVarRoot);
    QTreeWidgetItem *sysVarRoot = new QTreeWidgetItem(rootVar, QStringList(QString(tr("系统变量"))));
    sysVarRoot->setIcon(0, QIcon(":/images/subnode.png"));
    rootVar->addChild(sysVarRoot);

    QTreeWidgetItem *rootFunc = new QTreeWidgetItem(treeWidgetObjectOpt, QStringList(QString(tr("函数"))));

    QTreeWidgetItem *rootObjects = new QTreeWidgetItem(treeWidgetObjectOpt, QStringList(QString(tr("对象"))));

    treeWidgetObjectOpt->expandAll();

    dockObjectOpt->setWidget(treeWidgetObjectOpt);
    this->addDockWidget(Qt::LeftDockWidgetArea, dockObjectOpt);
}

void MainWindow::createDockObjectMsg()
{
    dockObjectMsg = new QDockWidget(this);
    dockObjectMsg->setObjectName(QStringLiteral("dockObjectMsg"));

    textMsg = new QTextEdit(dockObjectMsg);

    dockObjectMsg->setWidget(textMsg);
    this->addDockWidget(Qt::RightDockWidgetArea, dockObjectMsg);
}

void MainWindow::createDockExecCondition()
{
    /*
    dockExecCondition = new QDockWidget(this);
    dockExecCondition->setObjectName(QStringLiteral("dockExecCondition"));

    pConditionConfigForm = new ConditionConfigForm(dockExecCondition);



    dockExecCondition->setWidget(pConditionConfigForm);
    this->addDockWidget(Qt::TopDockWidgetArea, dockExecCondition);*/
}

void MainWindow::createDockScriptGenerator()
{
    dockScriptGenerator = new QDockWidget(this);
    dockScriptGenerator->setObjectName(QStringLiteral("dockScriptGenerator"));

    pScriptGeneratorForm = new ScriptGeneratorForm(dockScriptGenerator);



    dockScriptGenerator->setWidget(pScriptGeneratorForm);
    this->addDockWidget(Qt::BottomDockWidgetArea, dockScriptGenerator);

}


void MainWindow::showScriptPropertyConfigDlg()
{

}
