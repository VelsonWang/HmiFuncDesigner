#ifndef QPROGRESSBAREX_H
#define QPROGRESSBAREX_H

#include <QProgressBar>
#include <QPaintEvent>
#include "iloader.h"
#include "realtimedb.h"

class QProgressBarEx : public QProgressBar, public ILoader
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ getTagSelected WRITE setTagSelected)

public:
    Q_INVOKABLE QProgressBarEx(QWidget *parent = nullptr);

    QString getTagSelected() const;
    void setTagSelected(const QString &value);

public:
    void fromObject(XMLObject* xml) override;

protected:
    void paintEvent(QPaintEvent *) override;

private:
    // 关联的变量
    QString tagSelected;

private:
    RunTimeTag *m_tag;
};

#endif // QPROGRESSBAREX_H
