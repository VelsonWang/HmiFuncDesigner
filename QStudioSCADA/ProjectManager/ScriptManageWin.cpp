
#include <QListWidgetItem>
#include <QMenu>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include "ScriptManageWin.h"
#include "configutils.h"
#include "ScriptConditionConfigForm.h"
#include "ScriptEditorDlg.h"

QList<ScriptObject *> ScriptFileManage::m_listScriptInfo = QList<ScriptObject *>();


ScriptObject::ScriptObject()
{

}

ScriptObject::~ScriptObject()
{

}


void ScriptObject::load(QJsonObject &json)
{
    m_strName = json["Name"].toString();
    m_bInUse = json["InUse"].toBool();
    m_strDescription = json["Description"].toString();
    m_strRunMode = json["RunMode"].toString();
    m_strRunModeArgs = json["RunModeArgs"].toString();
}

void ScriptObject::save(QJsonObject &json)
{
    json["Name"] = m_strName;
    json["InUse"] = m_bInUse;
    json["Description"] = m_strDescription;
    json["RunMode"] = m_strRunMode;
    json["RunModeArgs"] = m_strRunModeArgs;
}


void ScriptFileManage::AddScriptInfo(ScriptObject * obj)
{
    int pos = m_listScriptInfo.indexOf(obj);
    if(pos == -1)
        m_listScriptInfo.append(obj);
}


void ScriptFileManage::ModifyScriptInfo(ScriptObject * oldobj, ScriptObject * newobj)
{
    int pos = m_listScriptInfo.indexOf(oldobj);
    if(pos == -1)
        return;
    m_listScriptInfo.replace(pos, newobj);
}


void ScriptFileManage::DeleteScriptInfo(ScriptObject * obj)
{
    m_listScriptInfo.removeOne(obj);
}

ScriptObject *ScriptFileManage::GetScriptObject(const QString &name)
{
    foreach (ScriptObject *pobj, m_listScriptInfo)
    {
        if(pobj->m_strName == name)
            return pobj;
    }
    return NULL;
}

void ScriptFileManage::load(const QString &filename, SaveFormat saveFormat)
{
    QFile loadFile(filename);

    if(!loadFile.exists())
        return;

    if (!loadFile.open(QIODevice::ReadOnly))
        return;

    m_listScriptInfo.clear();
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
    const QJsonObject json = loadDoc.object();

    QJsonArray scriptInfoArray = json["ScriptInfos"].toArray();
    for (int i = 0; i < scriptInfoArray.size(); ++i)
    {
        QJsonObject jsonObj = scriptInfoArray[i].toObject();
        ScriptObject *pObj = new ScriptObject();
        pObj->load(jsonObj);
        m_listScriptInfo.append(pObj);
    }

    loadFile.close();
}


void ScriptFileManage::save(const QString &filename, SaveFormat saveFormat)
{
    QString strPath = filename.left(filename.lastIndexOf("/"));
    QDir dir(strPath);
    if(!dir.exists())
    {
        dir.mkpath(strPath);
    }

    QFile saveFile(filename);
    QJsonObject obj;
    QJsonArray scriptInfosArray;

    saveFile.open(QFile::WriteOnly);

    for(int i = 0; i < m_listScriptInfo.size(); i++)
    {
        QJsonObject jsonObj;
        ScriptObject *pObj = m_listScriptInfo.at(i);
        pObj->save(jsonObj);
        scriptInfosArray.append(jsonObj);
    }

    obj["ScriptInfos"] = scriptInfosArray;

    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
}



/////////////////////////////////////////////////////////////////////////////////////

ScriptManageWin::ScriptManageWin(QWidget *parent, QString itemName, QString projName) :
    ChildBase(parent, itemName, projName)
{
    this->setWindowTitle(itemName);
    m_strItemName = itemName;

    ///////////////////////////////////////////////////////////

    m_pListWidget = new QListWidget(this);
    m_pListWidget->setViewMode(QListView::IconMode);
    m_pListWidget->setIconSize(QSize(32, 32));
    m_pListWidget->setGridSize(QSize(100, 100));
    m_pListWidget->setWordWrap(true);
    m_pListWidget->setSpacing(20);
    m_pListWidget->setResizeMode(QListView::Adjust);
    m_pListWidget->setMovement(QListView::Static);
    connect(m_pListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(ListWidgetClicked(QListWidgetItem*)));

    ///////////////////////////////////////////////////////////

    m_pVLayout = new QVBoxLayout();
    m_pVLayout->addWidget(m_pListWidget);
    m_pVLayout->setContentsMargins(1, 1, 1, 1);
    this->setLayout(m_pVLayout);
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

ScriptManageWin::~ScriptManageWin()
{
    if(m_pListWidget != nullptr) {
        delete m_pListWidget;
        m_pListWidget = nullptr;
    }

    if(m_pVLayout != nullptr) {
        delete m_pVLayout;
        m_pVLayout = nullptr;
    }
}


/*
* 打开文件
*/
void ScriptManageWin::open()
{
    QString fileDes = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/Scripts/Script.info";
    ScriptFileManage::load(fileDes, DATA_SAVE_FORMAT);

    m_pListWidget->clear();

    QListWidgetItem *pNewItem = new QListWidgetItem(QIcon(":/images/pm_script.png"), tr("新建脚本"));
    m_pListWidget->addItem(pNewItem);

    for(int i=0; i<ScriptFileManage::m_listScriptInfo.count(); i++) {
        ScriptObject *pObj = ScriptFileManage::m_listScriptInfo.at(i);
        QString scriptName = pObj->m_strName;
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/pm_script.png"), scriptName);
        m_pListWidget->addItem(pItem);
    }

}

/*
* 保存文件
*/
void ScriptManageWin::save()
{
    QString fileDes = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/Scripts/Script.info";
    ScriptFileManage::save(fileDes, DATA_SAVE_FORMAT);
}

/*
* 显示大图标
*/
void ScriptManageWin::ShowLargeIcon()
{
    m_pListWidget->setIconSize(QSize(32, 32));
}

/*
* 显示小图标
*/
void ScriptManageWin::ShowSmallIcon()
{
    m_pListWidget->setIconSize(QSize(24, 24));
}

/*
* 插槽：列表视图控件单击
*/
void ScriptManageWin::ListWidgetClicked(QListWidgetItem *item)
{
    if(m_strProjectName == "")
        return;

    if(item->text() == "新建脚本") {
        NewScript();
    } else {
        ModifyScript();
    }
}

/*
* 右键菜单生成
*/
void ScriptManageWin::contextMenuEvent(QContextMenuEvent * event)
{
    Q_UNUSED(event);

    QMenu *pMenu = new QMenu(this);

    QAction *pNewAct = new QAction(QIcon(":/images/icon_new.png"), tr("新建"), this);
    pNewAct->setStatusTip(tr("新建脚本"));
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(NewScript()));
    pMenu->addAction(pNewAct);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    pModifyAct->setStatusTip(tr("修改脚本"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(ModifyScript()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/icon_delete.png"), tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除脚本"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(DeleteScript()));
    pMenu->addAction(pDeleteAct);

    pMenu->move(cursor().pos());
    pMenu->show();

}

/*
* 插槽：新建
*/
void ScriptManageWin::NewScript()
{
    if(m_strProjectName == "")
        return;

    QString strProjectPath = m_strProjectName.left(m_strProjectName.lastIndexOf("/"));
    QListWidgetItem *pCurItem = m_pListWidget->currentItem();

    /////////////////////////////////////////////////////////////////////////////

    ScriptConditionConfigForm  *pDlg = new ScriptConditionConfigForm(strProjectPath, this);
    pDlg->setWindowTitle(tr("脚本属性"));
    pDlg->SetName(pCurItem->text());
    if(pDlg->exec() == QDialog::Accepted) {
        ScriptObject *pObj = new ScriptObject();
        pObj->m_strName = pDlg->GetName();
        pObj->m_bInUse = pDlg->isInUse();
        pObj->m_strDescription = pDlg->GetDescription();
        pObj->m_strRunMode = pDlg->GetRunMode();
        pObj->m_strRunModeArgs = pDlg->GetRunModeArgs();
        ScriptFileManage::AddScriptInfo(pObj);
        save();
        open();
    }
    delete pDlg;

    /////////////////////////////////////////////////////////////////////////////

}


/*
* 插槽：修改
*/
void ScriptManageWin::ModifyScript()
{
    if(m_strProjectName == "")
        return;

    QString strProjectPath = m_strProjectName.left(m_strProjectName.lastIndexOf("/"));
    QListWidgetItem *pCurItem = m_pListWidget->currentItem();
    QString scriptFileName = strProjectPath + "/Scripts/" + pCurItem->text() + ".js";

    /////////////////////////////////////////////////////////////////////////////

    ScriptConditionConfigForm  *pDlg = new ScriptConditionConfigForm(strProjectPath, this);
    pDlg->setWindowTitle(tr("脚本属性"));
    ScriptObject *pObj = ScriptFileManage::GetScriptObject(pCurItem->text());
    pDlg->SetName(pObj->m_strName);
    pDlg->SetInUse(pObj->m_bInUse);
    pDlg->SetDescription(pObj->m_strDescription);
    pDlg->SetRunMode(pObj->m_strRunMode);
    pDlg->SetRunModeArgs(pObj->m_strRunModeArgs);
    if(pDlg->exec() == QDialog::Accepted) {
        pObj->m_strName = pDlg->GetName();
        pObj->m_bInUse = pDlg->isInUse();
        pObj->m_strDescription = pDlg->GetDescription();
        pObj->m_strRunMode = pDlg->GetRunMode();
        pObj->m_strRunModeArgs = pDlg->GetRunModeArgs();
        if(pObj->m_strName != pCurItem->text()) {
            QString oldScriptFileName = strProjectPath + "/Scripts/" + pCurItem->text() + ".js";
            QString newScriptFileName = strProjectPath + "/Scripts/" + pObj->m_strName + ".js";
            pCurItem->setText(pObj->m_strName);
            QFile::rename(oldScriptFileName, newScriptFileName);
            scriptFileName = newScriptFileName;
        }

        save();
        open();

        /////////////////////////////////////////////////////////////////////////////

        ScriptEditorDlg *pScriptEditorDlg = new ScriptEditorDlg(strProjectPath, this);
        //qDebug() << "scriptFileName: " << scriptFileName;
        pScriptEditorDlg->load(scriptFileName);
        if(pScriptEditorDlg->exec() == QDialog::Accepted) {
            pScriptEditorDlg->save(scriptFileName);
        }
        delete pScriptEditorDlg;

    }
    delete pDlg;

}


/*
* 插槽：删除
*/
void ScriptManageWin::DeleteScript()
{
    QListWidgetItem *pCurItem = m_pListWidget->currentItem();
    ScriptObject *pObj = ScriptFileManage::GetScriptObject(pCurItem->text());
    ScriptFileManage::DeleteScriptInfo(pObj);

    QString scriptFileName = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/Scripts/" + pCurItem->text() + ".js";
    QFile scriptFile(scriptFileName);
    if(scriptFile.exists())
        scriptFile.remove();

    m_pListWidget->removeItemWidget(pCurItem);

    save();
    open();
}




