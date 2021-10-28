#ifndef QWIDGETVIEWTOOLBAR_H
#define QWIDGETVIEWTOOLBAR_H

#include <QWidget>

namespace Ui
{
class QWidgetViewToolBar;
}

class QFancyLineEdit;

class QWidgetViewToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit QWidgetViewToolBar(QWidget *parent = 0);
    ~QWidgetViewToolBar();
    void setLabelText(const QString &text);

signals:
    void findNext(const QString &text);
    void findPrev(const QString &text);

private:
    Ui::QWidgetViewToolBar *ui;

protected:
    QFancyLineEdit *m_lineEdit;

private slots:
    void on_prevBtn_clicked();
    void on_nextBtn_clicked();
};

#endif // QWIDGETVIEWTOOLBAR_H
