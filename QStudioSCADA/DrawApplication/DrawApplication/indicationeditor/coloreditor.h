#ifndef COLOREDITOR_H
#define COLOREDITOR_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QSpacerItem>

class ColorEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ColorEditor(QWidget *parent = 0);
    QColor getColor() const;
    static QString getColorString(const QColor &);
    void setColor(const QColor &);
    static QPixmap getColorPixmap(const QColor &color, uint size = 16);

private:
    QToolButton *toolBtn;
    QLabel *lblColorInfo;
    QLabel *lblPic;
    QColor color;
    QSpacerItem* spacer;

private slots:
    void showColorDialog();
};

#endif // COLOREDITOR_H
