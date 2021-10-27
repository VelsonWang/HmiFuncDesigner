#include "qabstractpage.h"


void QAbstractPage::initPlugin()//初始化插件
{
    m_undo_stack = new QUndoStack;
    m_widget = NULL;
    createWidget(); // 创建页面插件
    if(m_widget != NULL) {
        m_widget->setVisible(false);
    }
}

QWidget *QAbstractPage::getWidget()
{
    //获取页面控件
    return m_widget;
}

QString QAbstractPage::getPluginType()
{
    return PAGE_PLUGIN_NAME;
}

QUndoStack* QAbstractPage::getUndoStack()
{
    return m_undo_stack;
}
