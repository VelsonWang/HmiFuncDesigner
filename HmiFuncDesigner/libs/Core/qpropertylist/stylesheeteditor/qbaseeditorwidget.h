#ifndef QBASEEDITORWIDGET_H
#define QBASEEDITORWIDGET_H


#include "../../../shared/property/stylesheetitem/stylesheetstruct.h"

#include <QWidget>
#include <QUndoCommand>
#include <QIcon>

class QAbstractStylesheetItem;

class QBaseEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QBaseEditorWidget(QWidget *parent = 0);
    
    virtual void    set_item(QAbstractStylesheetItem* item);
    virtual void    init(QAbstractStylesheetItem *item)=0;
    tagStylesheetItem   get_value();
    virtual void        take_resource(QUndoCommand *cmd)=0;
    virtual void        add_resource(QUndoCommand *cmd)=0;
protected:
    QGradient   get_color(const QColor &color);
    QGradient   get_gradient(const QGradient &g);

    QIcon       get_icon(const QGradient &gradient);
signals:
    void    changed();
public slots:
protected:
    tagStylesheetItem       m_item;
};

extern QBaseEditorWidget* create_editor_widget(const QString &name);

#endif // QBASEEDITORWIDGET_H
