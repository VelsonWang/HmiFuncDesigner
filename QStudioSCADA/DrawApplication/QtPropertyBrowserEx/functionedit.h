
#ifndef FUNCTIONEDIT_H
#define FUNCTIONEDIT_H

#include <QLineEdit>

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
    void filePathChanged(const QString &filePath);
protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
private slots:
    void buttonClicked();
private:
    QLineEdit *theLineEdit;
    QStringList funcs_;
    QStringList supportEvents_;

};

#endif
