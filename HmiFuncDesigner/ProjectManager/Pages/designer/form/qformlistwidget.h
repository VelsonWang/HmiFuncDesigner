#ifndef QFORMLISTWIDGET_H
#define QFORMLISTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QUndoStack>

class QDesignerFormHost;
class QAbstractHost;
class QAbstractProperty;

class QFormListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QFormListWidget(QWidget *parent = 0);
    ~QFormListWidget();

    void insertForm(QAbstractHost *host,int index=-1);
    void removeForm(QAbstractHost* host);
    void showForm(QAbstractHost* host);
    void setUndoStack(QUndoStack* stack);
    void clear();
    void selectHost(QAbstractHost* host);
    void onZoom();

public slots:
    void slotPropertyEdit(QAbstractProperty* pro, const QVariant& value);
    void sameLeft();
    void sameTop();
    void sameRight();
    void sameBottom();
    void sameWidth();
    void sameHeight();
    void sameGeometry();
    void sameVCenter();
    void sameHCenter();
    void horizontalUniformDistribution();
    void verticalUniformDistribution();
    void redo();
    void undo();

protected slots:
    void slotInsertPage(QAbstractHost *host);
    void slotRemovePage(QAbstractHost *host);

private slots:
    void onSelect(QAbstractHost* host);

signals:
    void select(QAbstractHost* host);
    void remove(QList<QAbstractHost*> hosts);

protected:
    QList<QAction*> m_form_action_list;
    QList<QDesignerFormHost*> m_forms;
    QMap<QAbstractHost*, QDesignerFormHost*> m_host_to_form;
    QDesignerFormHost *m_current_form;
    QUndoStack *m_undo_stack;
};

#endif // QFORMLISTWIDGET_H
