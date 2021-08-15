#ifndef QBTNLINEEDIT_H
#define QBTNLINEEDIT_H

#include "corelibglobal.h"

#include <QLineEdit>
#include <QPushButton>

class CORELIB_EXPORT QBtnLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString textValue READ value WRITE setValue USER true)
public:
    explicit QBtnLineEdit(QWidget *parent = 0);

    void setValue(const QString &value);
    QString value();
protected:
    void    resizeEvent(QResizeEvent *);
signals:

public slots:
    void    btnClicked();
    void    text_changed();
protected:
    QPushButton     *m_button;
    QString         m_value;
};


#endif // QBTNLINEEDIT_H
