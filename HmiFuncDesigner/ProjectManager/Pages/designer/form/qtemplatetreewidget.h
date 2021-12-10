#ifndef QTEMPLATETREEWIDGET_H
#define QTEMPLATETREEWIDGET_H

#include <QTreeWidget>
#include <QMap>

class QFormWidget;

struct tagTemplateInfo
{
    QString m_file_name;
    QString m_name;
    int m_type;
};

class QTemplateTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QTemplateTreeWidget(const QString &type, QWidget *parent = 0);
    ~QTemplateTreeWidget();
    void init();
    void add_item(tagTemplateInfo *info);
    void save();
    void set_select(tagTemplateInfo* info);
    QStringList get_all_name();

protected:
    void mousePressEvent(QMouseEvent *event);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;

signals:
    void select(tagTemplateInfo* info);

public slots:

protected slots:
    void remove();
    void up();
    void double_clicked(QTreeWidgetItem* item);

protected:
    friend class QTemplateDeletgate;

protected:
    QList<tagTemplateInfo*> m_infos;
    QMap<QTreeWidgetItem*, tagTemplateInfo*> m_item_to_info;
    QMap<tagTemplateInfo*, QTreeWidgetItem*> m_info_to_item;
    QMap<QString, QTreeWidgetItem*> m_name_to_item;
    QTreeWidgetItem* m_default;
    QTreeWidgetItem* m_more;
    QString m_type;
    QTreeWidgetItem* m_current_item;
};

#endif // QTEMPLATETREEWIDGET_H
