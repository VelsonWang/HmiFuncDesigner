#include "tagcolorlisteditordialog.h"
#include "ui_TagColorListEditorDialog.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QColorDialog>
#include <QDebug>


ColorEditor::ColorEditor(QWidget *parent) :
    QWidget(parent),
    color_(Qt::white)
{
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    setFocusProxy(toolButton_);
    setFocusPolicy(toolButton_->focusPolicy());
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    colorLabel_ = new QLabel(this);
    QString styleSheet = QString("background:%1;").arg(color_.name());
    colorLabel_->setStyleSheet(styleSheet);
    colorLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(colorLabel_);
    layout->addWidget(toolButton_);
}

ColorEditor::~ColorEditor() {

}


void ColorEditor::setColor(const QColor& color) {
    if (color_ != color) {
        color_ = color;
        QString styleSheet = QString("background:%1;").arg(color_.name());
        colorLabel_->setStyleSheet(styleSheet);
    }
}


void ColorEditor::onToolButtonClicked() {
    bool ok = false;
    QRgb oldRgba = color_.rgba();
    QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
    if (ok && newRgba != oldRgba) {
        setColor(QColor::fromRgba(newRgba));
    }
}

bool ColorEditor::eventFilter(QObject *obj, QEvent *ev) {
    if(obj == toolButton_ && (ev->type() == QEvent::KeyPress ||
                              ev->type() == QEvent::KeyPress)) {
        ev->ignore();
        return true;
    }
    return QWidget::eventFilter(obj, ev);
}

QColor ColorEditor::getColor() const {
    return color_;
}



/////////////////////////////////////////////////////


TagColorListEditorDialog::TagColorListEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagColorListEditorDialog) {
    ui->setupUi(this);
    initUi();
}

TagColorListEditorDialog::~TagColorListEditorDialog() {
    delete ui;
}

void TagColorListEditorDialog::initUi() {
    this->setWindowTitle(tr("设置颜色"));
    initTableWidget();
}

/**
 * @brief TagColorListEditorDialog::initTableWidget
 * @details 初始化颜色设置表控件
 */
void TagColorListEditorDialog::initTableWidget() {
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


void TagColorListEditorDialog::on_btnAdd_clicked() {
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
void TagColorListEditorDialog::on_btnDel_clicked() {
    ui->tableTagColor->removeRow(ui->tableTagColor->currentRow());
}


/**
 * @brief TagColorListEditorDialog::on_btnOk_clicked
 * @details 单击确定
 */
void TagColorListEditorDialog::on_btnOk_clicked() {
    getValueColorList();
    this->accept();
}


/**
 * @brief TagColorListEditorDialog::on_btnCancel_clicked
 * @details 单击取消
 */
void TagColorListEditorDialog::on_btnCancel_clicked() {
    this->reject();
}

QStringList TagColorListEditorDialog::getValueColorList() {
    valueColorList_.clear();
    int iRowCount = ui->tableTagColor->rowCount();
    for(int i=0; i<iRowCount; i++) {
        QStringList listValueColor;
        QLineEdit *valueEditor = qobject_cast<QLineEdit* >(ui->tableTagColor->cellWidget(i, 0));
        ColorEditor *colorEditor = qobject_cast<ColorEditor* >(ui->tableTagColor->cellWidget(i, 1));
        QString szValue = valueEditor->text();
        QString szColor = colorEditor->getColor().name();
        listValueColor.append(szValue);
        listValueColor.append(szColor);
        QString szValueColor = listValueColor.join(":");
        valueColorList_.append(szValueColor);
    }
    return valueColorList_;
}

void TagColorListEditorDialog::setValueColorList(const QStringList &list) {
    valueColorList_.clear();
    valueColorList_ = list;

    while(ui->tableTagColor->rowCount())
        ui->tableTagColor->removeRow(0);

    for(int i=0; i<list.size(); i++) {
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
