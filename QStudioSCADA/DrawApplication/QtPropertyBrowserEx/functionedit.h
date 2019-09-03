
#ifndef FUNCTIONEDIT_H
#define FUNCTIONEDIT_H

#include <QLabel>

class FunctionEdit : public QWidget
{
    Q_OBJECT
public:
    FunctionEdit(QWidget *parent = Q_NULLPTR);

    QStringList getFunctions() const { return funcs_; }
    void setFunctions(const QStringList &funcs) { funcs_ = funcs; }

    /**
     * @brief setSupportEvents
     * @details 设置支持的事件列表
     * @param events 事件列表
     */
    void setSupportEvents(QStringList events) { supportEvents_ = events; }

signals:
    void functionsChanged(const QString &szVal);

private slots:
    void buttonClicked();

private:
    QLabel *theLabel_;
    QStringList funcs_;
    QStringList supportEvents_;

};

#endif
