#ifndef TAGTEXTLISTEDIT_H
#define TAGTEXTLISTEDIT_H

#include <QLabel>

class TagTextListEdit : public QWidget
{
    Q_OBJECT
public:
    TagTextListEdit(QWidget *parent = Q_NULLPTR);
    QStringList getValueTextList() const;

signals:
    void valueTextListChanged(const QString &szVal);

public slots:
    void setValueTextList(const QStringList &list);

private slots:
    void buttonClicked();

private:
    QLabel *theLabel_;
    QStringList valueTextList_;
};


#endif // TAGTEXTLISTEDIT_H
