#ifndef CHILDBASE_H
#define CHILDBASE_H

#include <QWidget>

class ChildBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChildBase(QWidget *parent = Q_NULLPTR,
                       const QString &itemName = "",
                       const QString &projName = "");
    virtual ~ChildBase();
    QString getProjectName() const;
    void setProjectName(const QString &s);
    QString getItemName() const;
    void setItemName(const QString &s);
    // 设置文件修改标志
    void setModifiedFlag(bool b);
    // 获取文件修改标志
    bool getModifiedFlag();

    // 打开文件
    virtual void open() = 0;
    // 保存文件
    virtual void save() = 0;
    // 显示大图标
    virtual void showLargeIcon() = 0;
    // 显示小图标
    virtual void showSmallIcon() = 0;

public:
    QString m_strProjectName;
    QString m_strItemName;

private:
    bool m_bModifiedFlag;

};

#endif // CHILDBASE_H
