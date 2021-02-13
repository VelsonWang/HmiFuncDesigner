#ifndef QSTATECHECKWIDGET_H
#define QSTATECHECKWIDGET_H

#include <QWidget>
#include <QMap>

class QAbstractProperty;
class QItemCheckBox;


class QStateCheckWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QStateCheckWidget(QAbstractProperty *property, const QString &title, QWidget *parent = 0);
    QString getTagName();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void changed();

protected slots:
    void check_changed();

protected:
    QList<QItemCheckBox*> m_widgets;
    QMap<QItemCheckBox*, QString> m_widget_to_name;
};

#endif // QSTATECHECKWIDGET_H
