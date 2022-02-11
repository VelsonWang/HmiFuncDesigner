#ifndef QLABELEX_H
#define QLABELEX_H

#include <QLabel>
#include "realtimedb.h"

class QLabelEx : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ tag WRITE setTag)

public:
    explicit QLabelEx(QWidget *parent = nullptr);

    QString tag();
    void setTag(const QString &szName);

signals:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString m_tagId;
    RunTimeTag *m_tag;
};

#endif // QLABELEX_H
