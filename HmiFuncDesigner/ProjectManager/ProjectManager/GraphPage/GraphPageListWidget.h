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
    // 创建指定名称的画面
    void notifyCreateGraphPageUseName(const QString &szName);

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    // 新建画面
    void onSlotNewDrawPage();

private:
    //QString m_szProjectPath; // 工程目录

};

#endif // GRAPHPAGELISTWIDGET_H
