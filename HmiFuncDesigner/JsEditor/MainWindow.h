#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QAction>
#include <QWidget>
#include <QMenu>
#include <QTreeWidget>
#include <QTextEdit>

#include "ScriptGeneratorForm.h"

class QsciScintilla;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString projPath, QString jsFile, QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();

private:
    void createUI();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockObjectOpt();
    void createDockObjectMsg();
    void createDockExecCondition();
    void createDockScriptGenerator();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void showScriptPropertyConfigDlg();

    QsciScintilla *textEdit;
    QString curFile;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QTreeWidget *treeWidgetObjectOpt;
    QDockWidget *dockObjectOpt;
    QTextEdit *textMsg;
    QDockWidget *dockObjectMsg;
    QDockWidget *dockExecCondition;
    QDockWidget *dockScriptGenerator;
    ScriptGeneratorForm *pScriptGeneratorForm;

private:
    QString m_strProjPath;
    QString m_strJsFile;

};

#endif // MAINWINDOW_H


