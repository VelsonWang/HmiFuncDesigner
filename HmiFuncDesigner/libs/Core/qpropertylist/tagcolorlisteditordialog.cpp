#include "tagcolorlisteditordialog.h"
#include "ui_tagcolorlisteditordialog.h"
#include "coloreditor.h"
#include <QApplication>
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>


TagColorListEditorDialog::TagColorListEditorDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::TagColorListEditorDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    initUi();
}

TagColorListEditorDialog::~TagColorListEditorDialog()
{
    delete ui;
}

void TagColorListEditorDialog::initUi()
{
    this->setWindowTitle(tr("设置颜色"));
    initTableWidget();
}

/**
 * @brief TagColorListEditorDialog::initTableWidget
 * @details 初始化颜色设置表控件
 */
void TagColorListEditorDialog::initTableWidget()
{
    QStringList hHeaderLabels;
    hHeaderLabels << tr("变量值") << tr("填充颜色");
    ui->tableTagColor->setColumnCount(hHeaderLabels.count());
    ui->tableTagColor->setHorizontalHeaderLabels(hHeaderLabels);
    ui->tableTagColor->horizontalHeader()->setHighlightSections(false);
    ui->tableTagColor->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableTagColor->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTagColor->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableTagColor->setColumnWidth(0, 160);
    ui->tableTagColor->setColumnWidth(1, 200);
}


void TagColorListEditorDialog::on_btnAdd_clicked()
{
    int iRowCount = ui->tableTagColor->rowCount();
    ui->tableTagColor->insertRow(iRowCount);
    QLineEdit *valueEditor = new QLineEdit();
    valueEditor->setAlignment(Qt::AlignCenter);
    valueEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableTagColor->setCellWidget(iRowCount, 0, valueEditor);

    ColorEditor *colorEditor = new ColorEditor();
    colorEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableTagColor->setCellWidget(iRowCount, 1, colorEditor);

}

/**
 * @brief TagColorListEditorDialog::on_btnDel_clicked
 * @details 删除选中的功能函数
 */
void TagColorListEditorDialog::on_btnDel_clicked()
{
    ui->tableTagColor->removeRow(ui->tableTagColor->currentRow());
}


/**
 * @brief TagColorListEditorDialog::on_btnOk_clicked
 * @details 单击确定
 */
void TagColorListEditorDialog::on_btnOk_clicked()
{
    getValueColorList();
    this->accept();
}


/**
 * @brief TagColorListEditorDialog::on_btnCancel_clicked
 * @details 单击取消
 */
void TagColorListEditorDialog::on_btnCancel_clicked()
{
    this->reject();
}

QStringList TagColorListEditorDialog::getValueColorList()
{
    m_valueColorList.clear();
    int iRowCount = ui->tableTagColor->rowCount();
    for(int i = 0; i < iRowCount; i++) {
        QStringList listValueColor;
        QLineEdit *valueEditor = qobject_cast<QLineEdit* >(ui->tableTagColor->cellWidget(i, 0));
        ColorEditor *colorEditor = qobject_cast<ColorEditor* >(ui->tableTagColor->cellWidget(i, 1));
        QString szValue = valueEditor->text();
        QString szColor = colorEditor->getColor().name();
        listValueColor.append(szValue);
        listValueColor.append(szColor);
        QString szValueColor = listValueColor.join(":");
        m_valueColorList.append(szValueColor);
    }
    return m_valueColorList;
}

void TagColorListEditorDialog::setValueColorList(const QStringList &list)
{
    m_valueColorList.clear();
    m_valueColorList = list;

    while(ui->tableTagColor->rowCount()) {
        ui->tableTagColor->removeRow(0);
    }

    for(int i = 0; i < list.size(); i++) {
        QString szValueColor = list.at(i);
        QStringList listValueColor = szValueColor.split(":");
        if(listValueColor.size() == 2) {
            int iRowCount = ui->tableTagColor->rowCount();
            ui->tableTagColor->insertRow(iRowCount);
            QLineEdit *valueEditor = new QLineEdit();
            valueEditor->setAlignment(Qt::AlignCenter);
            valueEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            valueEditor->setText(listValueColor.at(0));
            ui->tableTagColor->setCellWidget(iRowCount, 0, valueEditor);

            ColorEditor *colorEditor = new ColorEditor();
            colorEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            colorEditor->setColor(QColor(listValueColor.at(1)));
            ui->tableTagColor->setCellWidget(iRowCount, 1, colorEditor);
        }
    }
}
