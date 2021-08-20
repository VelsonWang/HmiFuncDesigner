#ifndef QSWITCHBUTTON_H
#define QSWITCHBUTTON_H

#include <QWidget>
#include <QPaintEvent>

class QSwitchButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ getTagSelected WRITE setTagSelected)
    Q_PROPERTY(QStringList funcs READ getFuncs WRITE setFuncs)
    Q_PROPERTY(bool stateOnInitial READ getStateOnInitial WRITE setStateOnInitial)
    Q_PROPERTY(QString showContent READ getShowContent WRITE setShowContent)
    Q_PROPERTY(QString resetPictureFile READ getResetPictureFile WRITE setResetPictureFile)
    Q_PROPERTY(QString setPictureFile READ getSetPictureFile WRITE setSetPictureFile)
    Q_PROPERTY(bool showNoScale READ getShowNoScale WRITE setShowNoScale)
    Q_PROPERTY(QString resetText READ getResetText WRITE setResetText)
    Q_PROPERTY(QString setText READ getSetText WRITE setSetText)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QString szHAlign READ getSzHAlign WRITE setSzHAlign)
    Q_PROPERTY(QString szVAlign READ getSzVAlign WRITE setSzVAlign)
    Q_PROPERTY(QColor resetBackgroundColor READ getResetBackgroundColor WRITE setResetBackgroundColor)
    Q_PROPERTY(QColor setBackgroundColor READ getSetBackgroundColor WRITE setSetBackgroundColor)
    Q_PROPERTY(int borderWidth READ getBorderWidth WRITE setBorderWidth)
    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)
    Q_PROPERTY(bool transparent READ getTransparent WRITE setTransparent)
    Q_PROPERTY(QFont font READ getFont WRITE setFont)
    Q_PROPERTY(bool enableOnInitial READ getEnableOnInitial WRITE setEnableOnInitial)
    Q_PROPERTY(bool showOnInitial READ getShowOnInitial WRITE setShowOnInitial)

public:
    explicit QSwitchButton(QWidget *parent = nullptr);

    QString getTagSelected() const;
    void setTagSelected(const QString &value);

    QStringList getFuncs() const;
    void setFuncs(const QStringList &value);

    bool getStateOnInitial() const;
    void setStateOnInitial(bool value);

    QString getShowContent() const;
    void setShowContent(const QString &value);

    QString getResetPictureFile() const;
    void setResetPictureFile(const QString &value);

    QString getSetPictureFile() const;
    void setSetPictureFile(const QString &value);

    bool getShowNoScale() const;
    void setShowNoScale(bool value);

    QString getResetText() const;
    void setResetText(const QString &value);

    QString getSetText() const;
    void setSetText(const QString &value);

    QString getSzHAlign() const;
    void setSzHAlign(const QString &value);

    QString getSzVAlign() const;
    void setSzVAlign(const QString &value);

    QColor getResetBackgroundColor() const;
    void setResetBackgroundColor(const QColor &value);

    QColor getSetBackgroundColor() const;
    void setSetBackgroundColor(const QColor &value);

    bool getTransparent() const;
    void setTransparent(bool value);

    QFont getFont() const;
    void setFont(const QFont &value);

    bool getEnableOnInitial() const;
    void setEnableOnInitial(bool value);

    bool getShowOnInitial() const;
    void setShowOnInitial(bool value);

    int getBorderWidth() const;
    void setBorderWidth(int value);

    QColor getBorderColor() const;
    void setBorderColor(const QColor &value);

    QColor getTextColor() const;
    void setTextColor(const QColor &value);

private:
    void setPropertyInner();
    void drawSwitchButton(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 关联的变量
    QString tagSelected;
    // 功能操作
    QStringList funcs;
    // 初始状态
    bool stateOnInitial;
    // 显示内容
    QString showContent;
    // false-显示图片，true-显示文本
    bool showContentText;
    // 复位图片
    QString resetPictureFile;
    // 置位图片
    QString setPictureFile;
    // 原尺寸显示
    bool showNoScale;

    // 复位显示文本
    QString resetText;
    // 置位显示文本
    QString setText;
    // 文本颜色
    QColor textColor;
    // 水平对齐
    QString szHAlign;
    // 垂直对齐
    QString szVAlign;
    // 复位按钮背景颜色
    QColor resetBackgroundColor;
    // 置位按钮背景颜色
    QColor setBackgroundColor;
    // 透明背景颜色
    bool transparent;
    // 边框宽度
    int borderWidth;
    // 边框颜色
    QColor borderColor;
    // 字体
    QFont font;
    // 初始有效
    bool enableOnInitial;
    // 初始可见
    bool showOnInitial;

    //-------------------------------------------------------------------------

    QImage resetImageObj;
    QImage setImageObj;
    bool selected;
};

#endif // QSWITCHBUTTON_H
