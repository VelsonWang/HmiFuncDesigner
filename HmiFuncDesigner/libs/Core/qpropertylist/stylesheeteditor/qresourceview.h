#ifndef QRESOURCEVIEW_H
#define QRESOURCEVIEW_H

#include <QDialog>

class QFileListView;
class QResourceShowWidget;

struct tagFileInfo;

class QResourceView : public QDialog
{
    Q_OBJECT
    
public:
    explicit QResourceView(QWidget *parent = 0);
    ~QResourceView();
    
    void set_file(const QByteArray &data);
    tagFileInfo* get_file();
    int get_ret();

protected slots:
    void select(tagFileInfo* file);
    void ok();

protected:
    QFileListView *m_file_view;
    QResourceShowWidget *m_show_widget;
    tagFileInfo *m_file_info;
    int m_ret;
};

#endif // QRESOURCEVIEW_H
