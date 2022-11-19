#ifndef QFINDWIDGET_H
#define QFINDWIDGET_H

#include "../../styledbar.h"

namespace Ui {
class QFindWidget;
}

class QFancyLineEdit;

class QFindWidget : public StyledBar
{
    Q_OBJECT

public:
    explicit QFindWidget(QWidget *parent = 0);
    ~QFindWidget();
    void setFindText(const QString &text);

public slots:
    void hideAndResetFocus();
    void showFindWidget();

    void clearFindEdit();
    void clearReplaceEdit();

    void checkFind();

    void replace_signal();
    void replaceAndNext_signal();
    void replaceAll_signal();

signals:
    void find(const QString &text);
    void findNext();
    void findPrev();
    void replace(const QString &text);
    void replaceAndNext(const QString &text);
    void replaceAll(const QString &text);
private:
    Ui::QFindWidget *ui;
    QFancyLineEdit *m_findEdit;
    QFancyLineEdit *m_replaceEdit;
};

#endif // QFINDWIDGET_H
