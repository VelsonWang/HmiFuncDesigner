#ifndef COLORPROPERTYEDITOR_H
#define COLORPROPERTYEDITOR_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>

class ColorPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPropertyEditor(QWidget *parent = 0);

    QColor getColor() const;
    static QPixmap getColorPixmap(const QColor& color, unsigned int size = 16);
    static QString getColorString(const QColor& color);

    bool eventFilter(QObject *obj, QEvent *ev);

signals:
    void dataChangedByUser(const QColor &getColor, ColorPropertyEditor* editor);

public slots:
    void setColor(const QColor &color);

private slots:
    void onToolButtonClicked();

private:
    QColor color_;
    QToolButton* toolButton_;
    QSpacerItem* spacer_;
};

#endif // COLORPROPERTYEDITOR_H
