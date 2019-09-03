
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
    explicit ColorEditor(QWidget *parent = Q_NULLPTR);
    ~ColorEditor();

    void setColor(const QColor &color);
    QColor getColor() const;

    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
    void onToolButtonClicked();

private:
    QColor color_;
    QToolButton* toolButton_;
    QLabel* colorLabel_;
};

#endif
