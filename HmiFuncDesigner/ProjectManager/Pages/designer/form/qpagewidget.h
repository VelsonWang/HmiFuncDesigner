#ifndef QPAGEWIDGET_H
#define QPAGEWIDGET_H

#include <QWidget>

class QTemplateTreeWidget;
class QTempWidget;
class QAbstractHost;

class QPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPageWidget(const QString &type, QWidget *parent = 0);
    
protected:
signals:
    void cancel();
    void ok(QAbstractHost* host);

public slots:
    void add_file(const  QString &file);

protected:
    QTemplateTreeWidget *m_tree_widget;
    QTempWidget *m_temp_widget;
    QString m_type;
};

#endif // QPAGEWIDGET_H
