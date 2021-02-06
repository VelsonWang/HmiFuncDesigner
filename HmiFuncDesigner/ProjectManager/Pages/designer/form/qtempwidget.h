#ifndef QTEMPWIDGET_H
#define QTEMPWIDGET_H

#include <QWidget>
#include <QMap>

namespace Ui {
class QTempWidget;
}

class QNameEdit;
struct tagTemplateInfo;
class QAbstractHost;

class QTempWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QTempWidget(QWidget *parent = 0);
    ~QTempWidget();

    void set_name_list(const QStringList &list);

protected:
    bool eventFilter(QObject *o, QEvent *e);

signals:
    void add_file(const QString &path);
    void cancel();
    void ok(QAbstractHost* host);

public slots:
    void set_current(tagTemplateInfo* info);

private:
    Ui::QTempWidget *ui;

protected:
    QMap<tagTemplateInfo*,QAbstractHost*> m_info_to_host;
    tagTemplateInfo *m_current;
    QNameEdit *m_name_edit;

private slots:
    void on_exists_button_clicked();
    void on_okbtn_clicked();
    void on_cancelbtn_clicked();
};

#endif // QTEMPWIDGET_H
