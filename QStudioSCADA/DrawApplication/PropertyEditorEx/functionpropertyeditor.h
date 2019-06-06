#ifndef FUNCTIONPROPERTYEDITOR_H
#define FUNCTIONPROPERTYEDITOR_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QStringList>
#include <QSpacerItem>

class FunctionPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionPropertyEditor(QWidget *parent = 0);
    QStringList getFunctions() const;
    bool eventFilter(QObject *obj, QEvent *ev);
    void setSupportEvents(QStringList events);

public slots:
    void setFunctions(const QStringList &funcs);

private slots:
    void onToolButtonClicked();

private:
    QStringList funcs_;
    QToolButton* toolButton_;
    QSpacerItem* spacer_;
    QStringList supportEvents_;
};

#endif // FUNCTIONPROPERTYEDITOR_H
