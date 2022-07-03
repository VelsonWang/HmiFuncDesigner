#ifndef QLABELEX_H
#define QLABELEX_H

#include <QLabel>
#include "realtimedb.h"
#include "iloader.h"

class QLabelEx : public QLabel, public ILoader
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ tag WRITE setTag)

public:
    Q_INVOKABLE QLabelEx(QWidget *parent = nullptr);

    QString tag();
    void setTag(const QString &szName);

public:
    void fromObject(XMLObject* xml) override;

signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_tagId;
    RunTimeTag *m_tag;
};

#endif // QLABELEX_H
