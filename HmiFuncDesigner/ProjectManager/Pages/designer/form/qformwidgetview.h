#ifndef QFORMWIDGETVIEW_H
#define QFORMWIDGETVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QUndoStack>

class QFormListWidget;

class StyledBar;
class QFormHost;
class QAbstractHost;
class QAbstractProperty;

class QFormWidgetView : public QWidget
{
    Q_OBJECT
public:
    explicit QFormWidgetView(QWidget *parent = 0);
    ~QFormWidgetView();
    void setUndoStack(QUndoStack* stack);

protected slots:
    void    project_opened();
    void    project_closed();
    void    add_page();

public slots:
    void    set_select(QAbstractHost* host);

signals:
    void select(QAbstractHost* host);
    void notifyPropertyEdit(QAbstractProperty* pro, const QVariant& value);

protected:
    QFormListWidget* m_formWidget;
    StyledBar* m_styledBar;
    QUndoStack* m_undo_stack;
    QStringList m_language_id;
};

#endif // QFORMWIDGETVIEW_H
