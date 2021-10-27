#include "qtextsheeteditor.h"
#include "ui_qtextsheeteditor.h"
#include <QColorDialog>

QTextSheetEditor::QTextSheetEditor(QWidget *parent) :
    QBaseEditorWidget(parent),
    ui(new Ui::QTextSheetEditor)
{
    ui->setupUi(this);
}

QTextSheetEditor::~QTextSheetEditor()
{
    delete ui;
}

void QTextSheetEditor::set_item(QAbstractStylesheetItem *item)
{
    QBaseEditorWidget::set_item(item);
    QGradient g = m_item.m_attributes.value("color").value<QGradient>();
    ui->pushButton->setIcon(get_icon(g));
}

void QTextSheetEditor::init(QAbstractStylesheetItem *item)
{
    Q_UNUSED(item)
}

void QTextSheetEditor::add_resource(QUndoCommand *cmd)
{
    Q_UNUSED(cmd)
}

void QTextSheetEditor::take_resource(QUndoCommand *cmd)
{
    Q_UNUSED(cmd)
}

void QTextSheetEditor::on_pushButton_clicked()
{
    QColor c;
    QVariant v = m_item.m_attributes.value("color");
    bool valid = false;
    if(v.isValid()) {
        QGradient g = v.value<QGradient>();
        QGradientStops s = g.stops();
        if(s.size() > 0) {
            c = s.first().second;
            valid = true;
        } else {
            c = QColor(255, 255, 255);
        }
    } else {
        c = QColor(255, 255, 255);
    }
    QColor cc = QColorDialog::getColor(c, this);
    if(cc.isValid()) {
        QGradient g = QLinearGradient(0,0,1,0);
        g.setColorAt(0, cc);
        g.setColorAt(1, cc);
        QVariant v;
        v.setValue<QGradient>(g);
        m_item.m_attributes.insert("color", v);
        ui->pushButton->setIcon(get_icon(g));
        emit changed();
    }
}
