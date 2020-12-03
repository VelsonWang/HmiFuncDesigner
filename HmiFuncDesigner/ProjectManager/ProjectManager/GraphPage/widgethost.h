#ifndef WIDGETHOST_H
#define WIDGETHOST_H

#include <QScrollArea>

class FormResizer;

class WidgetHost : public QScrollArea
{
    Q_OBJECT
public:
    explicit WidgetHost(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetHost();

    void    showHandle();
    void    hideHandle();

    bool    isHandleVisible();

    void    setFormWidget(QWidget* widget);
    QWidget *formWidget();

    void    updateFormGeometry();

signals:
    void    sizeChanged(const QRect &,const QRect &);

protected:
    FormResizer     *m_formResizer;
    QWidget         *m_formwidget;
};

#endif // WIDGETHOST_H
