#ifndef QLINEEDITEX_H
#define QLINEEDITEX_H

#include <QLineEdit>
#include "realtimedb.h"
#include "iloader.h"

class QLineEditEx : public QLineEdit, public ILoader
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ tag WRITE setTag)
public:
    Q_INVOKABLE QLineEditEx(QWidget *parent = nullptr);

    QString tag();
    void setTag(const QString &szName);

public:
    void fromObject(XMLObject* xml) override;

signals:

private slots:
    void enterPressed();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_tagId;
    RunTimeTag *m_tag;
    bool m_editing; //处于编辑模式
};

#endif // QLINEEDITEX_H
