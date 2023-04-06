#ifndef QPROGRESSBAREX_H
#define QPROGRESSBAREX_H

#include <QProgressBar>
#include <QPaintEvent>

class QProgressBarEx : public QProgressBar
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ getTagSelected WRITE setTagSelected)
    Q_ENUMS(Qt::Alignment)

public:
    explicit QProgressBarEx(QWidget *parent = nullptr);

    QString getTagSelected() const;
    void setTagSelected(const QString &value);

private:
    // 关联的变量
    QString tagSelected;
};

#endif // QPROGRESSBAREX_H
