#ifndef QDESIGNERMIMEDATA_H
#define QDESIGNERMIMEDATA_H

#include <QMimeData>
#include <QDrag>
#include <QWidget>

class QAbstractHost;

class QDesignerDnDItemInterface
{
public:
    enum DropType { MoveDrop, CopyDrop };

    QDesignerDnDItemInterface() {}
    virtual ~QDesignerDnDItemInterface() {}
    virtual QAbstractHost *host() const = 0;
    virtual QPoint hotSpot() const = 0;
    virtual DropType type() const = 0;
    virtual QString name()const =0;
};

class QDesignerDnDItem: public QDesignerDnDItemInterface
{
public:
    explicit QDesignerDnDItem(DropType type);
    virtual ~QDesignerDnDItem();
    virtual QAbstractHost *host() const;
    virtual QPoint hotSpot() const;
    virtual DropType type() const;
    virtual QString name() const;

protected:
    void init(QAbstractHost *host,const QPoint &global_mouse_pos);

protected:
    const DropType m_type;
    const QPoint m_globalStartPos;
    QAbstractHost *m_host;
    QPoint m_hot_spot;
    QString m_name;

    Q_DISABLE_COPY(QDesignerDnDItem)
};

class WidgetBoxDnDItem : public QDesignerDnDItem
{
public:
    WidgetBoxDnDItem(const QString &name,const QPoint &global_mouse_pos);
    ~WidgetBoxDnDItem();
};

class FormDnItem: public QDesignerDnDItem
{
public:
    FormDnItem(QAbstractHost *host,const QPoint &global_mouse_pos);
};

class QDesignerMimeData: public QMimeData
{
    Q_OBJECT

public:
    typedef QList<QDesignerDnDItemInterface *> QDesignerDnDItems;

    virtual ~QDesignerMimeData();

    const QDesignerDnDItems &items() const { return m_items; }
    static Qt::DropAction execDrag(const QDesignerDnDItems &items, QWidget * dragSource);
    QPoint hotSpot() const { return m_hotSpot; }
    void acceptEvent(QDropEvent *e) const;
    static void acceptEventWithAction(Qt::DropAction desiredAction, QDropEvent *e);

    void moveDecoration(const QPoint &globalPos) const;
private:
    QDesignerMimeData(const QDesignerDnDItems &items, QDrag *drag);
    Qt::DropAction proposedDropAction() const;
    void setImageTransparency(QImage &image, int alpha);
    const QDesignerDnDItems m_items;
    QPoint m_globalStartPos;
    QPoint m_hotSpot;
};

#endif // QDESIGNERMIMEDATA_H
