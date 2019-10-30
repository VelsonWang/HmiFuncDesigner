#include "TagFuncEditDialog.h"
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QRect>
#include "ConfigUtils.h"
#include "Helper.h"
#include "ProjectData.h"

static const int MaxTableColumns = 2;

FuncModel::FuncModel(QObject *parent) : QAbstractTableModel(parent) {}

FuncModel::~FuncModel()
{
    while(m_FuncList.size()) {
        PFuncObjectItem pObj = m_FuncList.takeFirst();
        if(pObj != Q_NULLPTR) {
            delete pObj;
            pObj = Q_NULLPTR;
        }
    }
}

Qt::ItemFlags FuncModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid()) theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    return theFlags;
}

QVariant FuncModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 ||
            index.row() >= m_FuncList.count() || index.column() < 0 ||
            index.column() >= MaxTableColumns)
        return QVariant();
    PFuncObjectItem pfunc = m_FuncList.at(index.row());
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case FUNC_NAME:
            return pfunc->GetFuncString();
        case EVENT_TYPE:
            return pfunc->event;
        default:
            Q_ASSERT(false);
        }
    }
    if (role == Qt::TextAlignmentRole) {
        QVariant value = (Qt::AlignLeft);
        return value;
    }
    return QVariant();
}

QVariant FuncModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case FUNC_NAME:
            return tr("功能函数");
        case EVENT_TYPE:
            return tr("事件类型");
        default:
            Q_ASSERT(false);
        }
    }
    return section + 1;
}

int FuncModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : m_FuncList.count();
}

int FuncModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : MaxTableColumns;
}

bool FuncModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    Q_UNUSED(value)
    if (!index.isValid() || role != Qt::EditRole || index.row() < 0 ||
            index.row() >= m_FuncList.count() || index.column() < 0 ||
            index.column() >= MaxTableColumns)
        return false;

    switch (index.column()) {
    default:
        Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}

bool FuncModel::insertRows(int row, int count, const QModelIndex &)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) m_FuncList.insert(row, new FuncObjectItem());
    endInsertRows();
    return true;
}

bool FuncModel::removeRows(int row, int count, const QModelIndex &)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) m_FuncList.removeAt(row);
    endRemoveRows();
    return true;
}

void FuncModel::save(QString &data)
{
    data = "";
    int count = m_FuncList.count();
    for (int i = 0; i < count; i++) {
        PFuncObjectItem pObj = m_FuncList.at(i);
        data += pObj->GetFuncString();
        data += "-";
        data += pObj->event;
        if (count > 1 && i < count - 1) data += ";";
    }
}

void FuncModel::load(QString data)
{
    m_FuncList.clear();
    if (data == "") return;
    QStringList strObjList = data.split(';');
    for (int i = 0; i < strObjList.size(); i++) {
        QStringList strObjTmpList = strObjList.at(i).split('-');
        if (strObjTmpList.size() != 2) continue;
        PFuncObjectItem pfuncItem = new FuncObjectItem();
        QString funcString = strObjTmpList.at(0);
        pfuncItem->name = funcString.left(funcString.indexOf("("));
        QString strArgs =
                (funcString.mid(funcString.indexOf("(") + 1,
                                funcString.indexOf(")") - funcString.indexOf("(") - 1))
                .trimmed();

        if (strArgs.length() > 0) {
            QStringList tmpList;
            tmpList = strArgs.split(',');
            for (int i = 0; i < tmpList.count(); i++) {
                TArgItem item;
                item.arg = tmpList.at(i);
                if (item.arg.indexOf("变量") > -1 || item.arg.indexOf("var") > -1)
                    item.type = "V";
                else
                    item.type = "C";
                pfuncItem->argList.append(item);
            }
        }

        pfuncItem->event = strObjTmpList.at(1);
        this->AppendRow(pfuncItem);
    }
}

void FuncModel::AppendRow(PFuncObjectItem item)
{
    int last = m_FuncList.size();
    beginInsertRows(QModelIndex(), last, last);
    m_FuncList.append(item);
    endInsertRows();
}

void FuncModel::InsertRow(int i, PFuncObjectItem item)
{
    beginInsertRows(QModelIndex(), i, i);
    m_FuncList.insert(i, item);
    endInsertRows();
}

PFuncObjectItem FuncModel::GetRow(int i)
{
    PFuncObjectItem it = NULL;
    if (i < m_FuncList.size()) return m_FuncList.at(i);
    return it;
}

void FuncModel::UpdateRow(int i, PFuncObjectItem item)
{
    if (i < m_FuncList.size()) {
        m_FuncList.replace(i, item);
        for (int c = 0; c < MaxTableColumns; c++)
            emit dataChanged(this->index(i, c), this->index(i, c));
    }
}

/*
* 当前行上移一行
*/
void FuncModel::UpRow(int row)
{
    if (row > 0 && row < m_FuncList.size()) {
        m_FuncList.move(row, row - 1);
        for (int c = 0; c < MaxTableColumns; c++) {
            emit dataChanged(this->index(row - 1, c), this->index(row - 1, c));
            emit dataChanged(this->index(row, c), this->index(row, c));
        }
    }
}

/*
* 当前行下移一行
*/
void FuncModel::DownRow(int row)
{
    if (row >= 0 && row < m_FuncList.size() - 1) {
        m_FuncList.move(row, row + 1);
        for (int c = 0; c < MaxTableColumns; c++) {
            emit dataChanged(this->index(row, c), this->index(row, c));
            emit dataChanged(this->index(row + 1, c), this->index(row + 1, c));
        }
    }
}

//////////////////////////////////////////////////////////////////

FuncTableView::FuncTableView(QWidget *parent) : QTableView(parent) {}

FuncTableView::~FuncTableView() {}

void FuncTableView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        QModelIndex idx = indexAt(event->pos());
        if (!idx.isValid()) {
            selectionModel()->clearSelection();
        }
    }
    QTableView::mousePressEvent(event);
}

///////////////////////////////////////////////////////////////////

TagFuncEditDialog::TagFuncEditDialog(QString projectPath, QWidget *parent)
    : QDialog(parent), m_strProjectPath(projectPath) {
    this->setWindowTitle(tr("工程转换"));

    m_pSplitterMain = new SplitterForm(Qt::Vertical, this);
    SplitterForm *splitterUp = new SplitterForm(Qt::Horizontal, m_pSplitterMain);

    SplitterForm *splitterLeft = new SplitterForm(Qt::Vertical, splitterUp);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(1, 1, 1, 1);
    leftLayout->setObjectName(QStringLiteral("leftLayout"));
    QLabel *labelFunc = new QLabel(splitterLeft);
    labelFunc->setText(tr("功能函数："));
    labelFunc->setObjectName(QStringLiteral("labelFunc"));
    leftLayout->addWidget(labelFunc);

    m_pTreeViewFunc = new QTreeView(splitterLeft);
    m_pTreeViewFunc->setSizePolicy(QSizePolicy::Preferred,
                                   QSizePolicy::Expanding);
    m_pTreeViewFunc->setObjectName(QStringLiteral("treeViewFunc"));
    connect(m_pTreeViewFunc, SIGNAL(clicked(QModelIndex)), this,
            SLOT(on_treeViewFunc_clicked(const QModelIndex &)));
    connect(m_pTreeViewFunc, SIGNAL(doubleClicked(QModelIndex)), this,
            SLOT(on_treeViewFunc_doubleClicked(const QModelIndex &)));
    TreeViewInit();
    leftLayout->addWidget(m_pTreeViewFunc);

    QLabel *labelFuncDesc = new QLabel(splitterLeft);
    labelFuncDesc->setText(tr("功能描述："));
    labelFuncDesc->setObjectName(QStringLiteral("labelFuncDesc"));
    leftLayout->addWidget(labelFuncDesc);

    m_pTextDesc = new QTextEdit(splitterLeft);
    m_pTextDesc->setObjectName(QStringLiteral("textDesc"));
    m_pTextDesc->setFrameShape(QFrame::StyledPanel);
    m_pTextDesc->setReadOnly(true);
    leftLayout->addWidget(m_pTextDesc);

    splitterLeft->setLayout(leftLayout);

    /////////////////////////////////////////////////////////////////

    SplitterForm *splitterMiddle = new SplitterForm(Qt::Vertical, splitterUp);
    QVBoxLayout *widgetMiddleLayout = new QVBoxLayout();
    widgetMiddleLayout->setContentsMargins(1, 1, 1, 1);
    widgetMiddleLayout->setObjectName(QStringLiteral("widgetMiddleLayout"));
    QLabel *labelFuncList = new QLabel(splitterMiddle);
    labelFuncList->setText(tr("事件功能函数列表："));
    labelFuncList->setObjectName(QStringLiteral("labelFuncList"));
    widgetMiddleLayout->addWidget(labelFuncList);

    m_pfuncItemModel = new FuncModel();
    m_funcTableView = new FuncTableView(splitterMiddle);
    m_funcTableView->setObjectName(QStringLiteral("m_funcTableView"));
    m_funcTableView->setModel(m_pfuncItemModel);
    m_funcTableView->setColumnWidth(0, 160);
    m_funcTableView->setColumnWidth(1, 120);
    m_funcTableView->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
    QHeaderView *pVHeaderView = m_funcTableView->verticalHeader();
    pVHeaderView->setSectionResizeMode(QHeaderView::ResizeToContents);
    // QHeaderView *pHHeaderView = m_funcTableView->horizontalHeader();
    // pHHeaderView->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_funcTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_funcTableView->setWordWrap(false);
    m_funcTableView->horizontalHeader()->setStretchLastSection(true);
    m_funcTableView->horizontalHeader()->setHighlightSections(false);
    connect(m_funcTableView, SIGNAL(clicked(QModelIndex)), this,
            SLOT(FuncTableViewClicked(QModelIndex)));
    widgetMiddleLayout->addWidget(m_funcTableView);

    splitterMiddle->setLayout(widgetMiddleLayout);

    /////////////////////////////////////////////////////////////////

    SplitterForm *splitterRight = new SplitterForm(Qt::Vertical, splitterUp);
    QVBoxLayout *widgetRightLayout = new QVBoxLayout();
    widgetRightLayout->setContentsMargins(1, 1, 1, 1);
    widgetRightLayout->setObjectName(QStringLiteral("widgetRightLayout"));

    m_varsList.clear();
    m_eventTypes << "变量值被修改"
                 << "变量被读取前";
    m_pVariantManager = new QtVariantPropertyManager(splitterRight);
    connect(m_pVariantManager,
            SIGNAL(valueChanged(QtProperty *, const QVariant &)), this,
            SLOT(FuncPropertyValueChanged(QtProperty *, const QVariant &)));
    m_pVariantFactory = new QtVariantEditorFactory(splitterRight);
    m_pPropertyEditor = new QtTreePropertyBrowser(splitterRight);
    m_pPropertyEditor->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Expanding);
    m_pPropertyEditor->setHeaderLabels(QStringList() << tr("参数名称")
                                       << tr("参数值"));
    m_pPropertyEditor->setColumnWidth(0, 60);
    m_pPropertyEditor->setColumnWidth(1, 300);
    m_pPropertyEditor->setFactoryForManager(m_pVariantManager, m_pVariantFactory);

    widgetRightLayout->addWidget(m_pPropertyEditor);
    splitterRight->setLayout(widgetRightLayout);

    /////////////////////////////////////////////////////////////////

    QWidget *widgetBottom = new QWidget(m_pSplitterMain);

    QHBoxLayout *hLayoutBtn = new QHBoxLayout();
    hLayoutBtn->setContentsMargins(1, 1, 1, 1);
    hLayoutBtn->setObjectName(QStringLiteral("hLayoutBtn"));
    QSpacerItem *horizontalSpacer_2 =
            new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hLayoutBtn->addItem(horizontalSpacer_2);

    QToolButton *btnAdd = new QToolButton(widgetBottom);
    btnAdd->setObjectName(QStringLiteral("btnAdd"));
    btnAdd->setText(tr("添加"));
    QIcon icon;
    icon.addFile(QStringLiteral(":/images1/add_light.png"), QSize(),
                 QIcon::Normal, QIcon::Off);
    btnAdd->setIcon(icon);
    btnAdd->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(on_btnAdd_clicked()));
    hLayoutBtn->addWidget(btnAdd);

    QToolButton *btnDel = new QToolButton(widgetBottom);
    btnDel->setObjectName(QStringLiteral("btnDel"));
    btnDel->setText(tr("删除"));
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/images1/del_light.png"), QSize(),
                  QIcon::Normal, QIcon::Off);
    btnDel->setIcon(icon1);
    btnDel->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnDel, SIGNAL(clicked(bool)), this, SLOT(on_btnDel_clicked()));
    hLayoutBtn->addWidget(btnDel);

    QSpacerItem *horizontalSpacer_3 =
            new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hLayoutBtn->addItem(horizontalSpacer_3);

    QToolButton *btnUp = new QToolButton(widgetBottom);
    btnUp->setObjectName(QStringLiteral("btnUp"));
    btnUp->setText(tr("上移"));
    QIcon icon2;
    icon2.addFile(QStringLiteral(":/images1/up.png"), QSize(), QIcon::Normal,
                  QIcon::Off);
    btnUp->setIcon(icon2);
    btnUp->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnUp, SIGNAL(clicked(bool)), this, SLOT(on_btnUp_clicked()));
    hLayoutBtn->addWidget(btnUp);

    QToolButton *btnDown = new QToolButton(widgetBottom);
    btnDown->setObjectName(QStringLiteral("btnDown"));
    btnDown->setText(tr("下移"));
    QIcon icon3;
    icon3.addFile(QStringLiteral(":/images1/down.png"), QSize(), QIcon::Normal,
                  QIcon::Off);
    btnDown->setIcon(icon3);
    btnDown->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnDown, SIGNAL(clicked(bool)), this, SLOT(on_btnDown_clicked()));
    hLayoutBtn->addWidget(btnDown);

    QSpacerItem *horizontalSpacer_5 =
            new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hLayoutBtn->addItem(horizontalSpacer_5);

    QToolButton *btnOk = new QToolButton(widgetBottom);
    btnOk->setObjectName(QStringLiteral("btnOk"));
    btnOk->setText(tr("确定"));
    QIcon icon4;
    icon4.addFile(QStringLiteral(":/images1/Finish.png"), QSize(), QIcon::Normal,
                  QIcon::Off);
    btnOk->setIcon(icon4);
    btnOk->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(on_btnOk_clicked()));
    hLayoutBtn->addWidget(btnOk);

    QToolButton *btnCancel = new QToolButton(widgetBottom);
    btnCancel->setObjectName(QStringLiteral("btnCancel"));
    btnCancel->setText(tr("取消"));
    QIcon icon5;
    icon5.addFile(QStringLiteral(":/images1/Cancel.png"), QSize(), QIcon::Normal,
                  QIcon::Off);
    btnCancel->setIcon(icon5);
    btnCancel->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(on_btnCancel_clicked()));
    hLayoutBtn->addWidget(btnCancel);

    QSpacerItem *horizontalSpacer_6 =
            new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hLayoutBtn->addItem(horizontalSpacer_6);

    widgetBottom->setLayout(hLayoutBtn);

    /////////////////////////////////////////////////////////////////

    QDesktopWidget *pDesktopWidget = QApplication::desktop();
    QRect rect = pDesktopWidget->screenGeometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();
    this->resize(screenWidth / 2, screenHeight / 2);
}

TagFuncEditDialog::~TagFuncEditDialog() {
    m_varsList.clear();
    delete m_pTreeViewFunc;
    delete m_pTextDesc;
    delete m_pVariantFactory;
    delete m_pVariantManager;
    delete m_pPropertyEditor;
    delete m_pfuncItemModel;
    delete m_funcTableView;
    qDeleteAll(m_supportFuncList);
    m_supportFuncList.clear();
}

void TagFuncEditDialog::resizeEvent(QResizeEvent *e) {
    m_pSplitterMain->resize(e->size());
}

void TagFuncEditDialog::TreeViewInit() {
    m_groupFuncNameList.clear();
    m_pTreeViewFunc->setHeaderHidden(true);
    m_pTreeViewItemModel = new QStandardItemModel();

    QString iniFileName = Helper::AppDir() + "/Config/RtdbFun.ini";
    QFile fileCfg(iniFileName);
    if (!fileCfg.exists()) {
        QMessageBox::critical(this, tr("提示"), tr("功能定义文件不存在！"));
        return;
    }

    m_supportFuncList.clear();
    // device type
    QStringList sFuncTypeList;
    ConfigUtils::getCfgList(iniFileName, "FunSupportList", "list", sFuncTypeList);
    for (int i = 0; i < sFuncTypeList.count(); i++) {
        QString sFuncTypeOne = sFuncTypeList.at(i).trimmed();
        QString key = sFuncTypeOne.left(sFuncTypeOne.indexOf("-"));
        QString nameTmp = sFuncTypeOne.right(sFuncTypeOne.length() -
                                             sFuncTypeOne.indexOf("-") - 1);
        QString name = QString::fromLocal8Bit(nameTmp.toLatin1());
        // qDebug()<< key << "  " << name;
        m_groupFuncNameList.append(name);
        QStandardItem *itemFuncTypeOne = new QStandardItem(name);
        itemFuncTypeOne->setEditable(false);

        QString strSecName = key;
        int funcCount =
                ConfigUtils::getCfgStr(iniFileName, strSecName, "Func-count", "0")
                .toInt();
        int descCount =
                ConfigUtils::getCfgStr(iniFileName, strSecName, "Desc-count", "0")
                .toInt();
        int argsCount =
                ConfigUtils::getCfgStr(iniFileName, strSecName, "Args-count", "0")
                .toInt();
        if (funcCount != (descCount + descCount + argsCount) / 3) {
            QMessageBox::critical(this, tr("提示"), tr("功能定义文件配置错误！"));
            return;
        }
        for (int i = 0; i < funcCount; i++) {
            TFuncObject *pFuncObj = new TFuncObject();
            m_supportFuncList.append(pFuncObj);
        }
        QString strIdent = "";
        QString strName = "";
        for (int i = 0; i < funcCount; i++) {
            TFuncObject *pFuncObj = m_supportFuncList.at(i);
            strIdent = QString("Func-%1").arg(i + 1);
            strName = ConfigUtils::getCfgStr(iniFileName, strSecName, strIdent, "");
            if (strName.indexOf("|") != -1) strName.replace("|", ",");
            pFuncObj->name = strName;
        }
        for (int i = 0; i < descCount; i++) {
            TFuncObject *pFuncObj = m_supportFuncList.at(i);
            strIdent = QString("Desc-%1").arg(i + 1);
            QString strDesc =
                    ConfigUtils::getCfgStr(iniFileName, strSecName, strIdent, "");
            pFuncObj->desc = QString::fromLocal8Bit(strDesc.toLatin1());
        }
        for (int i = 0; i < argsCount; i++) {
            TFuncObject *pFuncObj = m_supportFuncList.at(i);
            strIdent = QString("Args-%1").arg(i + 1);
            pFuncObj->args_type =
                    ConfigUtils::getCfgStr(iniFileName, strSecName, strIdent, "");
        }
        for (int i = 0; i < funcCount; i++) {
            TFuncObject *pFuncObj = m_supportFuncList.at(i);
            // qDebug()<< pFuncObj->name << "  " << pFuncObj->desc << "  " <<
            // pFuncObj->args_type;
            QString sFuncOne = pFuncObj->name.trimmed();
            QStandardItem *itemsFuncOne = new QStandardItem(sFuncOne);
            itemsFuncOne->setEditable(false);
            itemFuncTypeOne->appendRow(itemsFuncOne);
        }
        m_pTreeViewItemModel->appendRow(itemFuncTypeOne);
    }
    m_pTreeViewFunc->setModel(m_pTreeViewItemModel);
    m_pTreeViewFunc->expandAll();
}

void TagFuncEditDialog::UpdateExpandState() {
    QList<QtBrowserItem *> list = m_pPropertyEditor->topLevelItems();
    QListIterator<QtBrowserItem *> it(list);
    while (it.hasNext()) {
        QtBrowserItem *item = it.next();
        QtProperty *prop = item->property();
        m_mapIdToExpanded[m_mapPropertyToId[prop]] =
                m_pPropertyEditor->isExpanded(item);
    }
}

void TagFuncEditDialog::AddProperty(QtVariantProperty *property,
                                    const QString &id) {
    m_mapPropertyToId[property] = id;
    m_mapIdToProperty[id] = property;
    QtBrowserItem *item = m_pPropertyEditor->addProperty(property);
    if (m_mapIdToExpanded.contains(id))
        m_pPropertyEditor->setExpanded(item, m_mapIdToExpanded[id]);
}

/*
* 获取元素属性
*/
void TagFuncEditDialog::GetElementProperty(PFuncObjectItem pFuncObj) {
    UpdateExpandState();

    QMap<QtProperty *, QString>::ConstIterator itProp =
            m_mapPropertyToId.constBegin();
    while (itProp != m_mapPropertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    m_mapPropertyToId.clear();
    m_mapIdToProperty.clear();
    m_mapIdToExpanded.clear();
    m_pPropertyEditor->clear();

    m_pCurFuncObj = pFuncObj;
    if (!m_pCurFuncObj) {
        return;
    }

    QtVariantProperty *nameItem = m_pVariantManager->addProperty(QVariant::String, tr("name"));
    nameItem->setAttribute(QLatin1String("readOnly"), true);
    nameItem->setValue(m_pCurFuncObj->name);
    AddProperty(nameItem, tr("name"));
    // qDebug() << m_pCurFuncObj->name << m_pCurFuncObj->argList.count() <<
    // m_pCurFuncObj->event;
    int argCount = m_pCurFuncObj->argList.count();
    QtVariantProperty *argItem = NULL;
    QString argName = "";
    for (int i = 0; i < argCount; i++) {
        if (m_pCurFuncObj->argList.at(i).type == "V") {
            argName = QString("var%1").arg(i + 1);
            argItem = m_pVariantManager->addProperty(
                        QtVariantPropertyManager::enumTypeId(), argName);
            ProjectData::getInstance()->getAllTagName(m_varsList, "ALL");
            argItem->setAttribute(QLatin1String("enumNames"), m_varsList);
            if (m_varsList.indexOf(m_pCurFuncObj->argList.at(i).arg) != -1) {
                argItem->setValue(m_varsList.indexOf(m_pCurFuncObj->argList.at(i).arg));
            } else {
                argItem->setValue("");
            }
        } else if (m_pCurFuncObj->argList.at(i).type == "C") {
            argName = QString("var%1").arg(i + 1);
            argItem = m_pVariantManager->addProperty(QVariant::String, argName);
            if (m_pCurFuncObj->argList.at(i).arg != "") {
                argItem->setValue(m_pCurFuncObj->argList.at(i).arg);
            } else {
                argItem->setValue("0");
            }
        }
        AddProperty(argItem, argName);
    }

    QtVariantProperty *eventItem = m_pVariantManager->addProperty(
                QtVariantPropertyManager::enumTypeId(), tr("事件类型"));
    eventItem->setAttribute(QLatin1String("enumNames"), m_eventTypes);
    eventItem->setValue(m_eventTypes.indexOf(m_pCurFuncObj->event));
    AddProperty(eventItem, QLatin1String("eventType"));

    m_pPropertyEditor->setPropertiesWithoutValueMarked(true);
    m_pPropertyEditor->setRootIsDecorated(false);
}

/*
* 插槽：功能函数属性值改变
*/
void TagFuncEditDialog::FuncPropertyValueChanged(QtProperty *property,
                                                 const QVariant &value) {
    if (!m_pCurFuncObj) return;

    if (!m_mapPropertyToId.contains(property)) return;

    QString id = m_mapPropertyToId[property];
    if (id.indexOf("var") != -1 && id.length() > 3) {
        QString strArg = id.replace("var", "");
        bool ok = false;
        int idx = strArg.toInt(&ok, 10);
        if (ok && idx > 0) {
            TArgItem item = m_pCurFuncObj->argList.at(idx - 1);
            if (item.type == "V") {
                item.arg = m_varsList.at(value.toInt());
            } else if (item.type == "C") {
                item.arg = value.toString();
            }
            m_pCurFuncObj->argList.replace(idx - 1, item);
        }
    }

    if (id == QLatin1String("eventType")) {
        m_pCurFuncObj->event = m_eventTypes.at(value.toInt());
    }

    QModelIndex index = m_funcTableView->currentIndex();
    if (!index.isValid() || index.row() < 0) return;

    m_pfuncItemModel->UpdateRow(index.row(), m_pCurFuncObj);
}

/*
* 功能事件被单击
*/
void TagFuncEditDialog::FuncTableViewClicked(const QModelIndex &index) {
    if (!index.isValid() || index.row() < 0) return;
    PFuncObjectItem pFuncObj = m_pfuncItemModel->GetRow(index.row());
    GetElementProperty(pFuncObj);
}

/*
* 功能函数被单击
*/
void TagFuncEditDialog::on_treeViewFunc_clicked(const QModelIndex &index) {
    QStandardItem *item = m_pTreeViewItemModel->itemFromIndex(index);
    foreach (TFuncObject *pFuncObj, m_supportFuncList) {
        if (pFuncObj->name.trimmed() == item->text()) {
            m_pTextDesc->setText(pFuncObj->desc);
            return;
        }
    }
    m_pTextDesc->setText("");
}

/*
* 获取函数名和参数
*/
bool TagFuncEditDialog::GetFuncNameArgs(QString funcString, QString &funcName,
                                        QList<TArgItem> &argList) {
    if (funcString == "") return false;
    funcName = funcString.left(funcString.indexOf("("));
    QString strArgs =
            funcString.mid(funcString.indexOf("(") + 1,
                           funcString.indexOf(")") - funcString.indexOf("(") - 1);

    QString args_type = "";
    for (int i = 0; i < m_supportFuncList.count(); i++) {
        TFuncObject *pObj = m_supportFuncList.at(i);
        if (pObj->name.indexOf(funcName) > -1) {
            args_type = pObj->args_type;
        }
    }
    // qDebug() << funcName << args_type;
    QStringList tmpArgTypeList;
    tmpArgTypeList = args_type.split('-');

    if (strArgs.length() > 0 && tmpArgTypeList.count() > 0) {
        QStringList tmpList;
        tmpList = strArgs.split(',');
        if (tmpArgTypeList.count() == tmpList.count()) {
            for (int i = 0; i < tmpList.count(); i++) {
                TArgItem item;
                item.arg = tmpList.at(i);
                QString argTypeString = tmpArgTypeList.at(i);
                argTypeString = argTypeString.trimmed();
                item.type = argTypeString.right(1);
                argList.append(item);
                // qDebug() << item.arg << "    " << item.type;
            }
        }
    }
    return true;
}

/*
* 选择功能
*/
void TagFuncEditDialog::on_treeViewFunc_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid() || index.row() < 0) return;
    QStandardItem *item = m_pTreeViewItemModel->itemFromIndex(index);
    if (m_groupFuncNameList.indexOf(item->text()) == -1) {
        PFuncObjectItem pfuncItem = new FuncObjectItem();
        if (GetFuncNameArgs(item->text(), pfuncItem->name, pfuncItem->argList)) {
            pfuncItem->event = "变量值被修改";
            m_pfuncItemModel->AppendRow(pfuncItem);
        }
    }
}

/*
* 单击取消按钮
*/
void TagFuncEditDialog::on_btnCancel_clicked()
{
    this->reject();
}

/*
* 单击确定按钮
*/
void TagFuncEditDialog::on_btnOk_clicked()
{
    this->accept();
}

/*
* 单击添加按钮
*/
void TagFuncEditDialog::on_btnAdd_clicked()
{
    QModelIndex index = m_pTreeViewFunc->currentIndex();
    if (!index.isValid() || index.row() < 0) return;
    QStandardItem *item = m_pTreeViewItemModel->itemFromIndex(index);
    if (m_groupFuncNameList.indexOf(item->text()) == -1) {
        PFuncObjectItem pfuncItem = new FuncObjectItem();
        if (GetFuncNameArgs(item->text(), pfuncItem->name, pfuncItem->argList)) {
            pfuncItem->event = "变量值被修改";
            m_pfuncItemModel->AppendRow(pfuncItem);
        }
    }
}

/*
* 单击删除按钮
*/
void TagFuncEditDialog::on_btnDel_clicked()
{
    QItemSelectionModel *pItemSelectionModel = m_funcTableView->selectionModel();
    QModelIndexList modelIndexList = pItemSelectionModel->selectedIndexes();
    QMap<int, int> rowMap;
    foreach (QModelIndex index, modelIndexList) { rowMap.insert(index.row(), 0); }
    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    while (rowMapIterator.hasPrevious()) {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        m_pfuncItemModel->removeRow(rowToDel);
    }
}

/*
* 单击上移按钮
*/
void TagFuncEditDialog::on_btnUp_clicked()
{
    QModelIndex index = m_funcTableView->currentIndex();
    if (!index.isValid() || index.row() < 0) return;
    m_pfuncItemModel->UpRow(index.row());
    if (index.row() - 1 >= 0) m_funcTableView->selectRow(index.row() - 1);
}

/*
* 单击下移按钮
*/
void TagFuncEditDialog::on_btnDown_clicked()
{
    QModelIndex index = m_funcTableView->currentIndex();
    if (!index.isValid() || index.row() < 0) return;
    m_pfuncItemModel->DownRow(index.row());
    if (index.row() + 1 <= m_pfuncItemModel->rowCount())
        m_funcTableView->selectRow(index.row() + 1);
}

/*
* 加载数据
*/
void TagFuncEditDialog::SetData(QString data)
{
    m_pfuncItemModel->load(data);
}

/*
* 保存数据
*/
QString TagFuncEditDialog::GetData()
{
    QString data = "";
    m_pfuncItemModel->save(data);
    return data;
}
