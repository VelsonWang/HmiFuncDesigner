#ifndef GRAPHPAGELISTWIDGET_H
#define GRAPHPAGELISTWIDGET_H

#include <QListWidget>
#include <QContextMenuEvent>

class GraphPageListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit GraphPageListWidget(QWidget *parent = nullptr);
    // 设置工程目录
    //void setProjectPath(const QString &szPath);

signals:

private:
    // 创建空的画面页
    //void createEmptyGraphpage(const QString &szGraphPageName, int iWidth, int Hheight);


protected:
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    // 新建画面
    void onSlotNewDrawPage();

private:
    //QString m_szProjectPath; // 工程目录

};

#endif // GRAPHPAGELISTWIDGET_H
