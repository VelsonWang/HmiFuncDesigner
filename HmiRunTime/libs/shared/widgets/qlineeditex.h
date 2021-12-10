#ifndef QLINEEDITEX_H
#define QLINEEDITEX_H

#include <QLineEdit>
#include "realtimedb.h"

class QLineEditEx : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ tag WRITE setTag)
public:
    explicit QLineEditEx(QWidget *parent = nullptr);

    QString tag();
    void setTag(const QString &szName);

signals:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString m_tagId;
    RunTimeTag *m_tag;
};

#endif // QLINEEDITEX_H
