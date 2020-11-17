#include "SysTag.h"
#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QTextCodec>

SysTag::SysTag(QWidget *parent) : QWidget(parent)
{
    setWindowIcon(QIcon(":/images/pj_var.png"));
    setContextMenuPolicy(Qt::DefaultContextMenu);

    setObjectName(QString::fromUtf8("SysTag"));
    resize(832, 409);

    QFont font;
    font.setPointSize(12);
    setFont(font);

    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/pm_var.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    m_pVLayoutObj = new QVBoxLayout(this);
    m_pVLayoutObj->setSpacing(1);
    m_pVLayoutObj->setObjectName(QString::fromUtf8("verticalLayout"));
    m_pVLayoutObj->setContentsMargins(1, 1, 1, 1);
    m_pSysTagTableViewObj = new QTableWidgetEx(this);
    m_pSysTagTableViewObj->setObjectName(QString::fromUtf8("SysTagManagerTableView"));
    m_pSysTagTableViewObj->setFont(font);

    m_pVLayoutObj->addWidget(m_pSysTagTableViewObj);


    QString szTagFile = QCoreApplication::applicationDirPath() + "/SysVarList.odb";
    QString szTags = "";
    QFile readFile(szTagFile);
    if (readFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&readFile);
        in.setCodec("utf-8");
        szTags = in.readAll();
        readFile.close();

        XMLObject xml;
        if(xml.load(szTags, Q_NULLPTR)) {
            m_pSysTagTableViewObj->m_tagMgr.openFromXml(&xml);
            m_pSysTagTableViewObj->updateTable();
        }
    }

    if(m_pSysTagTableViewObj->model()->rowCount() > 0)
        m_pSysTagTableViewObj->selectRow(0);
}

SysTag::~SysTag()
{
    if(m_pSysTagTableViewObj != Q_NULLPTR) {
        delete m_pSysTagTableViewObj;
        m_pSysTagTableViewObj = Q_NULLPTR;
    }

    if(m_pVLayoutObj != Q_NULLPTR) {
        delete m_pVLayoutObj;
        m_pVLayoutObj = Q_NULLPTR;
    }
}

void SysTag::closeEvent(QCloseEvent *event)
{
    QString szTagFile = QCoreApplication::applicationDirPath() + "/SysVarList.odb";

    XMLObject tagObjs;
    tagObjs.setTagName("tags");
    tagObjs.setProperty("application_version", "V1.0.0");
    m_pSysTagTableViewObj->m_tagMgr.saveToXml(&tagObjs);

    QFile writeFile(szTagFile);
    if (!writeFile.open(QIODevice::WriteOnly)) return;
    QTextStream out(&writeFile);
    out.setCodec("utf-8");
    out << tagObjs.write();
    writeFile.close();

    QWidget::closeEvent(event);
}
