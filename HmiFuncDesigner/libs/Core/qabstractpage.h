#ifndef QABSTRACTPAGE_H
#define QABSTRACTPAGE_H

#include "corelibglobal.h"
#include "../shared/qabstractplugin.h"
#include <QWidget>
#include <QUndoStack>

#define PAGE_PLUGIN_NAME "Plugin.Page"


class CORELIB_EXPORT QAbstractPage : public QAbstractPlugin
{
    Q_OBJECT
public:
    void initPlugin();
    QString getPluginType();
    QWidget* getWidget(); // 获取页面控件
    virtual QIcon getPageIcon() = 0; // 获取页面的图标
    virtual QString getPageName() = 0;
    virtual int getIndex() = 0; // 获取页面索引，用于在标签中的位置
    virtual bool getEnabled() = 0; // 工程关闭的时候是否启用这个页面
    virtual QUndoStack* getUndoStack();

protected:
    virtual void createWidget() = 0; // 创建插件对应的页面控件

protected:
    QWidget* m_widget; // 页面控件
    QUndoStack* m_undo_stack;
};


#endif // QABSTRACTPAGE_H
