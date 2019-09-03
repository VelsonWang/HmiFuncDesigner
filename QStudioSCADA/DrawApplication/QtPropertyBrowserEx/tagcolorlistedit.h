
#ifndef TAGCOLORLISTEDIT_H
#define TAGCOLORLISTEDIT_H

#include <QLabel>

class TagColorListEdit : public QWidget
{
    Q_OBJECT
public:
    TagColorListEdit(QWidget *parent = Q_NULLPTR);

    QStringList getValueColorList() const;

signals:
    void valueColorListChanged(const QString &szVal);

public slots:
    void setValueColorList(const QStringList &list);

private slots:
    void buttonClicked();

private:
    QLabel *theLabel_;
    QStringList valueColorList_;
};

#endif
