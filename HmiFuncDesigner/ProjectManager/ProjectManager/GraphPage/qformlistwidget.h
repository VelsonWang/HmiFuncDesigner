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
    explicit QFormListWidget(QWidget *parent = Q_NULLPTR);
    ~QFormListWidget();

    void insertForm(QAbstractHost *host,int index=-1);
    void removeForm(QAbstractHost* host);
    void showForm(QAbstractHost* host);
    void clear();
    void setSelectObj(QAbstractHost* host);

public slots:
    void sameLeft();
    void sameTop();
    void sameRight();
    void sameBottom();
    void sameWidth();
    void sameHeight();
    void sameGeometry();
    void sameVCenter();
    void sameHCenter();

protected slots:
    void insertPageSlot(QAbstractHost *host);
    void removePageSlot(QAbstractHost *host);

signals:
    void selectObj(QAbstractHost* host);

protected:
    QList<QAction*> m_form_action_list;
    QList<QDesignerFormHost*> m_forms;
    QMap<QAbstractHost*,QDesignerFormHost*> m_host_to_form;
    QDesignerFormHost* m_current_form;
};

#endif // QFORMLISTWIDGET_H
