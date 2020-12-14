#include "formresizer.h"

#include "sizehandlerect.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

FormResizer::FormResizer(QWidget *parent) :
    QWidget(parent),
    m_widget(NULL),
    m_frame(new QFrame(this)),
    m_handleVisible(true)
{
    setWindowFlags(windowFlags()| Qt::SubWindow);
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);

    QVBoxLayout *handleLayout=new QVBoxLayout(this);
    handleLayout->setMargin(10);
    handleLayout->addWidget(m_frame);

    m_frame->setBackgroundRole(QPalette::Base);
    m_frame->setAutoFillBackground(true);

    QVBoxLayout *layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    m_frame->setLayout(layout);

    for(int i=SizeHandleRect::LeftTop;i<=SizeHandleRect::Left;i++)
    {
        m_handles[i]=new SizeHandleRect(this,(SizeHandleRect::Direction)i,this);
        connect(m_handles[i],SIGNAL(mouseButtonReleased(QRect,QRect)),this,SIGNAL(size_changed(QRect,QRect)));
    }
    updateGeometry();
}

FormResizer::~FormResizer()
{
}

void FormResizer::updateGeometry()
{
    const QRect &geom=m_frame->geometry();

    const int w=6;
    const int h=6;

    for(int i=0;i<8;i++)
    {
        switch(m_handles[i]->dir())
        {
        case SizeHandleRect::LeftTop:
            m_handles[i]->move(geom.x()-w/2,geom.y()-h/2);
            break;
        case SizeHandleRect::Top:
            m_handles[i]->move(geom.x()+geom.width()/2-w/2,geom.y()-h/2);
            break;
        case SizeHandleRect::RightTop:
            m_handles[i]->move(geom.x()+geom.width()-w/2,geom.y()-h/2);
            break;
        case SizeHandleRect::Right:
            m_handles[i]->move(geom.x()+geom.width()-w/2,geom.y()+geom.height()/2-h/2);
            break;
        case SizeHandleRect::RightBottom:
            m_handles[i]->move(geom.x()+geom.width()-w/2,geom.y()+geom.height()-h/2);
            break;
        case SizeHandleRect::Bottom:
            m_handles[i]->move(geom.x()+geom.width()/2-w/2,geom.y()+geom.height()-h/2);
            break;
        case SizeHandleRect::LeftBottom:
            m_handles[i]->move(geom.x()-w/2,geom.y()+geom.height()-h/2);
            break;
        case SizeHandleRect::Left:
            m_handles[i]->move(geom.x()-w/2,geom.y()+geom.height()/2-h/2);
            break;
        }
    }
}

void FormResizer::update()
{
    for(int i=0;i<8;i++)
    {
        m_handles[i]->update();
    }
}

void FormResizer::setWidget(QWidget *wid)
{
    m_widget=wid;
    if(m_widget!=NULL)
    {
        this->resize(m_widget->size()+QSize(20,20));
        m_widget->setParent(m_frame);
        m_widget->move(0,0);
    }
}

void FormResizer::resizeEvent(QResizeEvent *e)
{
    updateGeometry();
    if(m_widget!=NULL)
    {
        m_widget->resize(m_frame->size());
    }
    QWidget::resizeEvent(e);
}

void FormResizer::showHandle()
{
    for(int i=0;i<8;i++)
    {
        m_handles[i]->show();
    }
    m_handleVisible=true;
}

void FormResizer::hideHandle()
{
    for(int i=0;i<8;i++)
    {
        m_handles[i]->setVisible(false);
    }
    m_handleVisible=false;
}

void FormResizer::updateFormGeometry()
{
    if(m_frame->size()!=m_widget->size())
    {
        this->resize(m_widget->size()+QSize(20,20));
        updateGeometry();
    }
}

bool FormResizer::isHandleVisible()
{
    return m_handleVisible;
}
