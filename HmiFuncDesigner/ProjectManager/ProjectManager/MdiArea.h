#ifndef MDIAREA_H
#define MDIAREA_H

#include <QWidget>
#include <QMdiArea>

class MdiArea : public QMdiArea
{
    Q_OBJECT
public:
    MdiArea(QWidget* parent);
    virtual ~MdiArea();

public:
    void setupMdiArea();

Q_SIGNALS:
    void tabCloseRequested(int index);

};


#endif // MDIAREA_H
