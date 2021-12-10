#include "TableviewDelegate.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QPainter>
#include <QListView>
#include <QDebug>

TableViewLineEditDelegate::TableViewLineEditDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

//返回一个编辑控件，用来编辑指定项的数据
QWidget *TableViewLineEditDelegate::createEditor(QWidget *parent,
	const QStyleOptionViewItem &/* option */,
	const QModelIndex &/* index */) const
{
	//返回该控件
	QLineEdit *editor = new QLineEdit(parent);
	return editor;
}

//将Model中数据赋值到控件上
void TableViewLineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	//返回该索引的模型，继而返回该模型中此索引的编辑角色数据
	QString value = index.model()->data(index, Qt::EditRole).toString();
	//给控件赋值
	QLineEdit *lineedit = static_cast<QLineEdit*>(editor);

    QRegExp regx(m_szLineEditConditions);
    QValidator *validator = new QRegExpValidator(regx, lineedit);
    if(m_szLineEditConditions != "")
        lineedit->setValidator(validator);
	lineedit->setText(value);
}

//设定模型数据，根据指定项中对应编辑控件的数据
void TableViewLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
	QString value = lineedit->text();
	//设置模型的数据
	model->setData(index, value, Qt::EditRole);
}

//更新编辑框几何形状
void TableViewLineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	//根据option,设置编辑框位置
	editor->setGeometry(option.rect);
}

void TableViewLineEditDelegate::setLineEditConditions(QString conditions)
{
    m_szLineEditConditions = conditions;
}

///////////////////////////////////////////////////////////////////////////////



TableViewComboBoxDelegate::TableViewComboBoxDelegate(const QStringList &texts,
                                                     bool editable,
                                                     QObject *parent) :
    QItemDelegate(parent),
    m_bEditable(editable),
    m_szItemTexts(texts)
{
}


QWidget *TableViewComboBoxDelegate::createEditor(QWidget *parent,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QComboBox *pEditor = new QComboBox(parent);
    pEditor->setView(new QListView());
    pEditor->setEditable(m_bEditable);
    pEditor->addItems(m_szItemTexts);

    return pEditor;
}


void TableViewComboBoxDelegate::setEditorData(QWidget *editor,
                                              const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *pEditor = static_cast<QComboBox *>(editor);
    pEditor->setCurrentText(value);
}


void TableViewComboBoxDelegate::setModelData(QWidget *editor,
                                             QAbstractItemModel *model,
                                             const QModelIndex &index) const
{
    QComboBox *pEditor = static_cast<QComboBox *>(editor);
    QString szValue = pEditor->currentText();
    model->setData(index, szValue, Qt::EditRole);
}


void TableViewComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                     const QStyleOptionViewItem &option,
                                                     const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

void TableViewComboBoxDelegate::setItems(QStringList szList)
{
    m_szItemTexts.clear();
    m_szItemTexts.append(szList);
}

///////////////////////////////////////////////////////////////////////////////



TableViewSpinBoxDelegate::TableViewSpinBoxDelegate(int iMin, int iMax, QObject *parent)
    : QItemDelegate(parent),
      m_iMin(iMin),
      m_iMax(iMax)
{
}


QWidget *TableViewSpinBoxDelegate::createEditor(QWidget *parent,
                                                const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QSpinBox *pEditor = new QSpinBox(parent);
    pEditor->setRange(m_iMin, m_iMax);
    return pEditor;
}


void TableViewSpinBoxDelegate::setEditorData(QWidget *editor,
                                              const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QSpinBox *pEditor = static_cast<QSpinBox *>(editor);
    pEditor->setValue(value.toInt());
}


void TableViewSpinBoxDelegate::setModelData(QWidget *editor,
                                            QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    QSpinBox *pEditor = static_cast<QSpinBox *>(editor);
    int iValue = pEditor->value();
    model->setData(index, iValue, Qt::EditRole);
}


void TableViewSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

