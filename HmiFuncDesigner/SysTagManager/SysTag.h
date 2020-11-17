#ifndef SYSVARIABLE_H
#define SYSVARIABLE_H

#include "../Public/Public.h"
#include "qtablewidgetex.h"
#include <QVector>
#include <QWidget>
#include <QVBoxLayout>


class SysTag : public QWidget
{
    Q_OBJECT

public:
    explicit SysTag(QWidget *parent = Q_NULLPTR);
    ~SysTag();

protected:
    void closeEvent(QCloseEvent *event);

private:
    QVBoxLayout *m_pVLayoutObj;
    QTableWidgetEx *m_pSysTagTableViewObj;


};

#endif // SYSVARIABLE_H
