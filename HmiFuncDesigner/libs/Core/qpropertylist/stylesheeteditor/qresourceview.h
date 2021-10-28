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

    void setFile(const QByteArray &data);
    tagFileInfo* getFile();
    int getRet();

protected slots:
    void select(tagFileInfo* file);
    void ok();

protected:
    QFileListView *m_fileView;
    QResourceShowWidget *m_showWidget;
    tagFileInfo *m_fileInfo;
    int m_ret;
};

#endif // QRESOURCEVIEW_H
