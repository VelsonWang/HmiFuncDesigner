#include "qstatecheckwidget.h"
#include "qitemcheckbox.h"
#include "../../../shared/host/qabstracthost.h"
#include "../../../shared/qstylesheetinfo.h"
#include "../../../shared/property/qabstractproperty.h"
#include <QGridLayout>
#include <QPainter>

QStateCheckWidget::QStateCheckWidget(QAbstractProperty *property, const QString &title, QWidget *parent):
    QWidget(parent)
{
    SheetItems items = property->get_host()->property("sheet_state").value<SheetItems>();
    QGridLayout *g = new QGridLayout;

    QStringList list = title.split(":");
    list.removeAll("");

    foreach(tagSheetItem item, items.values()) {
        if(item.m_un_used.contains(property->getObjectProperty("name").toString())) {
            continue;
        }
        sheet s = StyleSheetInfo::get_info(item.m_item_id);
        if(s.m_name != "") {
            QItemCheckBox *c = new QItemCheckBox(s.m_show_name, s.m_not);
            m_widget_to_name.insert(c,s.m_name);
            if(list.contains(s.m_name)) {
                c->set_checked(true);
                c->set_yes(true);
            } else if(list.contains("!" + s.m_name)) {
                c->set_checked(true);
                c->set_yes(false);
            }

            connect(c, SIGNAL(changed()), this, SLOT(check_changed()));
            m_widgets.append(c);
            int index = m_widgets.size()-1;
            g->addWidget(c, index/4, index%4);
        }
    }

    this->setLayout(g);
}

void QStateCheckWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(QColor(0, 0, 0));
    p.setBrush(Qt::transparent);
    p.drawRect(this->rect().adjusted(0, 0, -1, -1));
}

void QStateCheckWidget::check_changed()
{
    emit changed();
}

QString QStateCheckWidget::getTagName()
{
    QString str;
    foreach(QItemCheckBox* c, m_widgets) {
        if(c->get_check()) {
            str += ":";
            if(!c->get_yes()) {
                str += "!";
            }
            str += m_widget_to_name.value(c);
        }
    }
    return str;
}
