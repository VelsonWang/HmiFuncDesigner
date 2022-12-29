#ifndef QFANCYLINEEDIT_H
#define QFANCYLINEEDIT_H

#include <QLineEdit>
#include <QAbstractButton>

class QFancyLineEditPrivate;

class IconButton: public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(float iconOpacity READ iconOpacity WRITE setIconOpacity)
    Q_PROPERTY(bool autoHide READ hasAutoHide WRITE setAutoHide)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
public:
    explicit IconButton(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void setPixmap(const QPixmap &pixmap)
    {
        m_pixmap = pixmap;
        update();
    }
    QPixmap pixmap() const
    {
        return m_pixmap;
    }
    float iconOpacity()
    {
        return m_iconOpacity;
    }
    void setIconOpacity(float value)
    {
        m_iconOpacity = value;
        update();
    }
    void animateShow(bool visible);

    void setAutoHide(bool hide)
    {
        m_autoHide = hide;
    }
    bool hasAutoHide() const
    {
        return m_autoHide;
    }

private:
    float m_iconOpacity;
    bool m_autoHide;
    QPixmap m_pixmap;
};

class QFancyLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_ENUMS(Side)

public:
    enum Side {Left = 0, Right = 1};

    explicit QFancyLineEdit(QWidget *parent = 0);
    ~QFancyLineEdit();

    QPixmap buttonPixmap(Side side) const;
    void setButtonPixmap(Side side, const QPixmap &pixmap);

    QMenu *buttonMenu(Side side) const;
    void setButtonMenu(Side side, QMenu *menu);

    void setButtonVisible(Side side, bool visible);
    bool isButtonVisible(Side side) const;

    void setButtonToolTip(Side side, const QString &);
    void setButtonFocusPolicy(Side side, Qt::FocusPolicy policy);

    void setMenuTabFocusTrigger(Side side, bool v);
    bool hasMenuTabFocusTrigger(Side side) const;

    void setAutoHideButton(Side side, bool h);
    bool hasAutoHideButton(Side side) const;

signals:
    void buttonClicked(QFancyLineEdit::Side side);
    void leftButtonClicked();
    void rightButtonClicked();

private slots:
    void checkButtons(const QString &);
    void iconClicked();

protected:
    void resizeEvent(QResizeEvent *e);
    bool event(QEvent *e);

private:
    void setCompleter(QCompleter *);

    void updateMargins();
    void updateButtonPositions();
    friend class FancyLineEditPrivate;

    QFancyLineEditPrivate *d;
    QString m_oldText;
};

#endif // QFANCYLINEEDIT_H
