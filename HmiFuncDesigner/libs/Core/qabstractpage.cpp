#include "qabstractpage.h"


void QAbstractPage::init_plugin()//初始化插件
{
    m_undo_stack=new QUndoStack;
    m_widget=Q_NULLPTR;
    create_widget();//创建页面插件
    if(m_widget!=Q_NULLPTR)
    {
        m_widget->setVisible(false);
    }
}

QWidget *QAbstractPage::get_widget()
{//获取页面控件
    return m_widget;
}

QString QAbstractPage::get_plugin_type()
{
    return PAGE_PLUGIN_NAME;
}

QUndoStack* QAbstractPage::get_undo_stack()
{
    return m_undo_stack;
}
