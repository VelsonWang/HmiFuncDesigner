
#ifndef SCRIPTEDIT_H
#define SCRIPTEDIT_H

#include <QLabel>

class ScriptEdit : public QWidget
{
    Q_OBJECT
public:
    ScriptEdit(QWidget *parent = Q_NULLPTR);

    QString getScript() const;
    void setScript(const QString &script);

    /**
     * @brief setSupportEvents
     * @details 设置支持的事件列表
     * @param events 事件列表
     */
    void setSupportEvents(QStringList events);

signals:
    void scriptChanged(const QString &szVal);

private slots:
    void buttonClicked();

private:
    QLabel *theLabel_;
    QString script_;
    QStringList supportEvents_;

};

#endif
