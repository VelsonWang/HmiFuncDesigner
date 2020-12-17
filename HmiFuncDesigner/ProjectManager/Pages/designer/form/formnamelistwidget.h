#ifndef FORMNAMELISTWIDGET_H
#define FORMNAMELISTWIDGET_H

#include <QListWidget>
#include <QContextMenuEvent>

class FormNameListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit FormNameListWidget(QWidget *parent = nullptr);

signals:
    // 创建指定名称的画面
    void notifyCreateFormPageUseName(const QString &szName);

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    // 新建画面
    void onSlotNewFormPage();

private:


};

#endif // FORMNAMELISTWIDGET_H
