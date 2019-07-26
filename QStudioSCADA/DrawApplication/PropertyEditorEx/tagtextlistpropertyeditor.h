#ifndef TAGTEXTLISTPROPERTYEDITOR_H
#define TAGTEXTLISTPROPERTYEDITOR_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QStringList>
#include <QSpacerItem>

class TagTextListPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TagTextListPropertyEditor(QWidget *parent = nullptr);
    QStringList getValueTextList() const;
    bool eventFilter(QObject *obj, QEvent *ev);

signals:
    void dataChangedByUser(const QStringList &list, TagTextListPropertyEditor* editor);

public slots:
    void setValueTextList(const QStringList &list);

private slots:
    void onToolButtonClicked();

private:
    QStringList valueTextList_;
    QToolButton* toolButton_;
    QSpacerItem* spacer_;
};

#endif // TAGTEXTLISTPROPERTYEDITOR_H
