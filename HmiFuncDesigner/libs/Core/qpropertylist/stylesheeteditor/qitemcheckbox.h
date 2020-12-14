#ifndef QITEMCHECKBOX_H
#define QITEMCHECKBOX_H

#include <QWidget>

namespace Ui {
class QItemCheckBox;
}

class QItemCheckBox : public QWidget
{
    Q_OBJECT
    
public:
    explicit QItemCheckBox(const QString &text,bool show,QWidget *parent = 0);
    ~QItemCheckBox();
    bool     get_check();
    bool     get_yes();

    void    set_checked(bool checked);
    void    set_yes(bool yes);
protected:
    void     paintEvent(QPaintEvent *);
signals:
    void    changed();
private:
    Ui::QItemCheckBox *ui;
};

#endif // QITEMCHECKBOX_H
