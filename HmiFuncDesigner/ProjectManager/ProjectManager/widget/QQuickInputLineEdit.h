#ifndef QQUICKINPUTLINEEDIT_H
#define QQUICKINPUTLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QMap>


class QQuickInputLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QQuickInputLineEdit(QWidget *parent = Q_NULLPTR);
    // 设置支持快捷输入的字符串集合
    void setQuickInputWordMap(const QMap<QString, QString> &wordMap);

signals:

public slots:

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    QMap<QString, QString> wordMap_;
};

#endif // QQUICKINPUTLINEEDIT_H
