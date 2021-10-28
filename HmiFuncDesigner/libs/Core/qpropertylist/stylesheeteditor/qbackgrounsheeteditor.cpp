#include "qbackgrounsheeteditor.h"
#include "ui_qbackgrounsheeteditor.h"
#include "qresourceview.h"
#include "../../qsoftcore.h"
#include "../../qfilemanager.h"
#include "../../../shared/qprojectcore.h"
#include "../../../shared/property/stylesheetitem/qabstractstylesheetitem.h"
#include <QColorDialog>
#include <QGradient>

QBackgrounSheetEditor::QBackgrounSheetEditor(QWidget *parent) :
    QBaseEditorWidget(parent),
    ui(new Ui::QBackgrounSheetEditor)
{
    ui->setupUi(this);

    ui->repeat_combo->addItems(QStringList() << "repeat-x" << "repeat-y" << "repeat-xy" << "no-repeat");
    ui->repeat_combo->setCurrentIndex(2);

    ui->position_combo->addItems(QStringList() << "left top" << "left center" << "left bottom" << "center top" << "center" << "center bottom"
                                 << "right top" << "right center" << "right bottom");
    ui->position_combo->setCurrentIndex(0);

    connect(ui->repeat_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(repeat_changed()));
    connect(ui->position_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(position_changed()));
}

QBackgrounSheetEditor::~QBackgrounSheetEditor()
{
    delete ui;
    if(m_tempResource != "") {

    }
}

void QBackgrounSheetEditor::on_colorBtn_clicked()
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
        QGradient g = QLinearGradient(0, 0, 1, 0);
        g.setColorAt(0, cc);
        g.setColorAt(1, cc);
        QVariant v;
        v.setValue<QGradient>(g);
        m_item.m_attributes.insert("color", v);
        emit changed();
    }
}

void QBackgrounSheetEditor::on_gradientBtn_clicked()
{
    QVariant v;
    v = m_item.m_attributes.value("color");
    QGradient g = get_gradient(v.value<QGradient>());
    if (g.type() != QGradient::NoGradient) {
        v.setValue<QGradient>(g);
        m_item.m_attributes.insert("color", v);
        emit changed();
    }
}

void QBackgrounSheetEditor::on_imageBtn_clicked()
{
    QResourceView dlg(this);
    QString name = m_item.m_attributes.value("image").toString();
    //dlg.set_file(info->m_resourceData);
    dlg.exec();

    if(dlg.getRet() == 1) {
        tagFileInfo *info = dlg.getFile();
        if(info != NULL) {
            tagFileGroupInfo *g = QSoftCore::getCore()->getFileManager()->getGroup(info->m_group_uuid);
            QString s = g->m_group_name + "/" + info->m_file_name;
            if(s != name) {
                m_item.m_attributes.insert("image", s);
                if(m_tempResource != "") {
                }
                m_tempResource = s;
                emit changed();
            }
        }
    }
}

void QBackgrounSheetEditor::setItem(QAbstractStylesheetItem *item)
{
    QBaseEditorWidget::setItem(item);

    QString str;

    str = m_item.m_attributes.value("position", "left top").toString();
    ui->position_combo->setCurrentIndex(ui->position_combo->findText(str));

    str = m_item.m_attributes.value("repeat", "repeat-xy").toString();
    ui->repeat_combo->setCurrentIndex(ui->repeat_combo->findText(str));

    if(m_tempResource != "") {

    }
}

void QBackgrounSheetEditor::repeat_changed()
{
    QString str = ui->repeat_combo->currentText();
    if(m_item.m_attributes.value("repeat").toString() != str) {
        m_item.m_attributes.insert("repeat", str);
        emit changed();
    }
}

void QBackgrounSheetEditor::position_changed()
{
    QString str = ui->position_combo->currentText();
    if(m_item.m_attributes.value("position").toString() != str) {
        m_item.m_attributes.insert("position", str);
        emit changed();
    }
}

void QBackgrounSheetEditor::takeResource(QUndoCommand *cmd)
{
    if(m_startResource != m_tempResource) {
        if(cmd != NULL) {
            if(m_startResource != "") {
            }
        }
    }
}

void QBackgrounSheetEditor::addResource(QUndoCommand *cmd)
{
    if(m_startResource != m_tempResource) {
        if(cmd != NULL) {
            if(m_tempResource != "") {
            }
        }
        if(m_tempResource != "") {
        }
        m_tempResource = "";
    }
}

void QBackgrounSheetEditor::init(QAbstractStylesheetItem *item)
{
    tagStylesheetItem temp = item->value().value<tagStylesheetItem>();

    m_startResource = temp.m_attributes.value("image").toString();
}
