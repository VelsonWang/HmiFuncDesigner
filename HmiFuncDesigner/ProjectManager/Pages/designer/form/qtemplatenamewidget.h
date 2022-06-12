#ifndef QTEMPLATENAMEWIDGET_H
#define QTEMPLATENAMEWIDGET_H

#include <QWidget>

namespace Ui {
class QTemplateNameWidget;
}

struct tagTemplateInfo;
class QToolBarButton;

class QTemplateNameWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QTemplateNameWidget(QWidget *parent = 0);
    ~QTemplateNameWidget();
    
    void set_temp_info(tagTemplateInfo* info);
    void set_icon(const QString &icon);
protected:
    void    enterEvent(QEvent *);
    void    leaveEvent(QEvent *);
signals:
    void    up();
    void    remove();
private:
    Ui::QTemplateNameWidget *ui;
protected:
    QToolBarButton     *m_up_button;
    QToolBarButton     *m_remove_button;
    tagTemplateInfo    *m_temp_info;

};

#endif // QTEMPLATENAMEWIDGET_H
