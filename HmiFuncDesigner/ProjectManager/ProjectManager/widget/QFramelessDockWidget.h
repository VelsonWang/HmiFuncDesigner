#ifndef QFRAMELESSDOCKWIDGET_H
#define QFRAMELESSDOCKWIDGET_H

#include <QDockWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>

class QFramelessDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit QFramelessDockWidget(QWidget *parent = 0);
    ~QFramelessDockWidget();

private slots:
    void onBtnHideOrShowClick();

private:
    QWidget *m_titleWidget;
    QHBoxLayout *m_titleWidgetHLayout;
    QLabel *m_titleLabel;
    QSpacerItem *m_hSpacer;
    QPushButton *m_btnHideOrShow;
};

#endif // QFRAMELESSDOCKWIDGET_H
