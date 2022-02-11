#include "tagtextlisteditordialog.h"
#include "ui_tagtextlisteditordialog.h"
#include "coloreditor.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QColorDialog>
#include <QDebug>


TagTextListEditorDialog::TagTextListEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagTextListEditorDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    initUi();
}

TagTextListEditorDialog::~TagTextListEditorDialog()
{
    delete ui;
}

void TagTextListEditorDialog::initUi()
{
    this->setWindowTitle(tr("设置颜色"));
    initTableWidget();
}

/**
 * @brief TagTextListEditorDialog::initTableWidget
 * @details 初始化颜色设置表控件
 */
void TagTextListEditorDialog::initTableWidget()
{
    QStringList hHeaderLabels;
    hHeaderLabels << tr("变量值") << tr("文本") << tr("背景颜色") << tr("字体颜色");
    ui->tableTagText->setColumnCount(hHeaderLabels.count());
    ui->tableTagText->setHorizontalHeaderLabels(hHeaderLabels);
    ui->tableTagText->horizontalHeader()->setHighlightSections(false);
    ui->tableTagText->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableTagText->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTagText->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableTagText->setColumnWidth(0, 80);
    ui->tableTagText->setColumnWidth(1, 200);
    ui->tableTagText->setColumnWidth(2, 80);
    ui->tableTagText->setColumnWidth(3, 80);
}


void TagTextListEditorDialog::on_btnAdd_clicked()
{
    int iRowCount = ui->tableTagText->rowCount();
    ui->tableTagText->insertRow(iRowCount);
    QLineEdit *valueEditor = new QLineEdit();
    valueEditor->setAlignment(Qt::AlignCenter);
    valueEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableTagText->setCellWidget(iRowCount, 0, valueEditor);

    QLineEdit *textEditor = new QLineEdit();
    textEditor->setAlignment(Qt::AlignLeft);
    textEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableTagText->setCellWidget(iRowCount, 1, textEditor);

    ColorEditor *colorBackgroundEditor = new ColorEditor();
    colorBackgroundEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableTagText->setCellWidget(iRowCount, 2, colorBackgroundEditor);

    ColorEditor *colorTextEditor = new ColorEditor();
    colorTextEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableTagText->setCellWidget(iRowCount, 3, colorTextEditor);
}

/**
 * @brief TagTextListEditorDialog::on_btnDel_clicked
 * @details 删除选中的功能函数
 */
void TagTextListEditorDialog::on_btnDel_clicked()
{
    ui->tableTagText->removeRow(ui->tableTagText->currentRow());
}


/**
 * @brief TagTextListEditorDialog::on_btnOk_clicked
 * @details 单击确定
 */
void TagTextListEditorDialog::on_btnOk_clicked()
{
    getValueTextList();
    this->accept();
}


/**
 * @brief TagTextListEditorDialog::on_btnCancel_clicked
 * @details 单击取消
 */
void TagTextListEditorDialog::on_btnCancel_clicked()
{
    this->reject();
}

QStringList TagTextListEditorDialog::getValueTextList()
{
    m_valueTextList.clear();
    int iRowCount = ui->tableTagText->rowCount();
    for(int i = 0; i < iRowCount; i++) {
        QStringList listValueColor;
        QLineEdit *valueEditor = qobject_cast<QLineEdit* >(ui->tableTagText->cellWidget(i, 0));
        QLineEdit *textEditor = qobject_cast<QLineEdit* >(ui->tableTagText->cellWidget(i, 1));
        ColorEditor *colorBackgroundEditor = qobject_cast<ColorEditor* >(ui->tableTagText->cellWidget(i, 2));
        ColorEditor *colorTextEditor = qobject_cast<ColorEditor* >(ui->tableTagText->cellWidget(i, 3));
        QString szValue = valueEditor->text();
        QString szText = textEditor->text();
        QString szBackgroundColor = colorBackgroundEditor->getColor().name();
        QString szTextColor = colorTextEditor->getColor().name();
        listValueColor.append(szValue);
        listValueColor.append(szText);
        listValueColor.append(szBackgroundColor);
        listValueColor.append(szTextColor);
        QString szValueColor = listValueColor.join(":");
        m_valueTextList.append(szValueColor);
    }
    return m_valueTextList;
}

void TagTextListEditorDialog::setValueTextList(const QStringList &list)
{
    m_valueTextList.clear();
    m_valueTextList = list;

    while(ui->tableTagText->rowCount()) {
        ui->tableTagText->removeRow(0);
    }

    for(int i = 0; i < list.size(); i++) {
        QString szValueColor = list.at(i);
        QStringList listValueColor = szValueColor.split(":");
        if(listValueColor.size() == 4) {
            int iRowCount = ui->tableTagText->rowCount();
            ui->tableTagText->insertRow(iRowCount);

            QLineEdit *valueEditor = new QLineEdit();
            valueEditor->setAlignment(Qt::AlignCenter);
            valueEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            valueEditor->setText(listValueColor.at(0));
            ui->tableTagText->setCellWidget(iRowCount, 0, valueEditor);

            QLineEdit *textEditor = new QLineEdit();
            textEditor->setAlignment(Qt::AlignLeft);
            textEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            textEditor->setText(listValueColor.at(1));
            ui->tableTagText->setCellWidget(iRowCount, 1, textEditor);

            ColorEditor *colorBackgroundEditor = new ColorEditor();
            colorBackgroundEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            colorBackgroundEditor->setColor(QColor(listValueColor.at(2)));
            ui->tableTagText->setCellWidget(iRowCount, 2, colorBackgroundEditor);

            ColorEditor *colorTextEditor = new ColorEditor();
            colorTextEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            colorTextEditor->setColor(QColor(listValueColor.at(3)));
            ui->tableTagText->setCellWidget(iRowCount, 3, colorTextEditor);
        }
    }
}
