#ifndef TAGCOLORLISTPROPERTYEDITOR_H
#define TAGCOLORLISTPROPERTYEDITOR_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QStringList>
#include <QSpacerItem>

class TagColorListPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TagColorListPropertyEditor(QWidget *parent = 0);
    QStringList getValueColorList() const;
    bool eventFilter(QObject *obj, QEvent *ev);

signals:
    void dataChangedByUser(const QStringList &list, TagColorListPropertyEditor* editor);

public slots:
    void setValueColorList(const QStringList &list);

private slots:
    void onToolButtonClicked();

private:
    QStringList valueColorList_;
    QToolButton* toolButton_;
    QSpacerItem* spacer_;
};

#endif // TAGCOLORLISTPROPERTYEDITOR_H
