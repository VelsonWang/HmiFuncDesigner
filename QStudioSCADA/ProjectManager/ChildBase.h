#ifndef CHILDBASE_H
#define CHILDBASE_H

#include <QWidget>

class ChildBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChildBase(QWidget *parent = 0, QString itemName = "", QString projName = "");
    virtual ~ChildBase();
    QString GetItemProjectName();
    void SetItemProjectName(QString s);
    QString GetItemName();
    void SetItemName(QString s);
    // 设置文件修改标志
    void SetModifiedFlag(bool b);
    // 获取文件修改标志
    bool GetModifiedFlag();

    // 打开文件
    virtual void open() = 0;
    // 保存文件
    virtual void save() = 0;
    // 显示大图标
    virtual void ShowLargeIcon() = 0;
    // 显示小图标
    virtual void ShowSmallIcon() = 0;

public:
    QString m_strProjectName;
    QString m_strItemName;

private:
    bool m_bModifiedFlag;

};

#endif // CHILDBASE_H
