#ifndef QABSTRACTPAGE_H
#define QABSTRACTPAGE_H

#include "corelibglobal.h"

#include "../shared/qabstractplugin.h"

#include <QWidget>
#include <QUndoStack>

#define PAGE_PLUGIN_NAME       "Plugin.Page"


class CORELIB_EXPORT QAbstractPage : public QAbstractPlugin
{
    Q_OBJECT
public:
    void                init_plugin();
    QString             get_plugin_type();
    QWidget*            get_widget();//获取页面控件
    virtual QIcon       get_page_icon()=0;//获取页面的图标
    virtual QString     get_page_name()=0;
    virtual int         get_index()=0;//获取页面索引，用于在标签中的位置
    virtual bool        get_enabled()=0;//工程关闭的时候是否启用这个页面
    virtual QUndoStack* get_undo_stack();
protected:
    virtual void        create_widget()=0;//创建插件对应的页面控件

protected:
    QWidget                     *m_widget;//页面控件
    QUndoStack                  *m_undo_stack;
};


#endif // QABSTRACTPAGE_H
