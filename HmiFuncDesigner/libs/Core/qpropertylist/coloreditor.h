
#ifndef COLOREDITOR_H
#define COLOREDITOR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

class ColorEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ColorEditor(QWidget *parent = NULL);
    ~ColorEditor();

    void setColor(const QColor &c);
    QColor getColor() const;

    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
    void onToolButtonClicked();

private:
    QColor color;
    QToolButton* toolButton;
    QLabel* colorLabel;
};

#endif
