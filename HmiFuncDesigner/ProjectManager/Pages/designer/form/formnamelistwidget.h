#ifndef FORMNAMELISTWIDGET_H
#define FORMNAMELISTWIDGET_H

#include <QListWidget>
#include <QContextMenuEvent>

class QAbstractHost;
class QAbstractProperty;

class FormNameListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit FormNameListWidget(QWidget *parent = nullptr);
    void updateUI();

signals:
    // 创建指定名称的画面
    void notifyCreateFormPageUseName(const QString &szName);
    // 选中画面画面
    void notifySelectPage(QAbstractHost* pHostObj);
    // 属性变化
    void notifyPropertyChange(QAbstractProperty* pro, const QVariant& value);

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    // 新建画面
    void onSlotNewFormPage();
    // 重命名画面
    void onRenameFormPage();

    void onItemClicked(QListWidgetItem *item);

private:


};

#endif // FORMNAMELISTWIDGET_H
