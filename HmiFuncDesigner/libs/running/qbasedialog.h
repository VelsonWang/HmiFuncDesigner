#ifndef QBASEDIALOG_H
#define QBASEDIALOG_H

#include <QDialog>

class QBaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QBaseDialog(QWidget *parent = 0);
    
    void    set_widget(QWidget* wid);
signals:
    
public slots:
protected:
    QWidget*        m_widget;
};

#endif // QBASEDIALOG_H
