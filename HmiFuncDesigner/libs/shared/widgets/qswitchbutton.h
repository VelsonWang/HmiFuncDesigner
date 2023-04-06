#ifndef QSWITCHBUTTON_H
#define QSWITCHBUTTON_H

#include <QWidget>
#include <QPaintEvent>

class QSwitchButton : public QWidget
{
    Q_OBJECT
    Q_ENUMS(ShowContent HAlign VAlign)
    Q_PROPERTY(QString tag READ getTagSelected WRITE setTagSelected)
    Q_PROPERTY(QStringList onFuncs READ getOnFuncs WRITE setOnFuncs)
    Q_PROPERTY(QStringList offFuncs READ getOffFuncs WRITE setOffFuncs)
    Q_PROPERTY(bool stateOnInitial READ getStateOnInitial WRITE setStateOnInitial)
    Q_PROPERTY(ShowContent showContent READ getShowContent WRITE setShowContent)
    Q_PROPERTY(QString resetPictureFile READ getResetPictureFile WRITE setResetPictureFile)
    Q_PROPERTY(QString setPictureFile READ getSetPictureFile WRITE setSetPictureFile)
    Q_PROPERTY(bool showNoScale READ getShowNoScale WRITE setShowNoScale)
    Q_PROPERTY(QString resetText READ getResetText WRITE setResetText)
    Q_PROPERTY(QString setText READ getSetText WRITE setSetText)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(HAlign szHAlign READ getSzHAlign WRITE setSzHAlign)
    Q_PROPERTY(VAlign szVAlign READ getSzVAlign WRITE setSzVAlign)
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

    enum ShowContent {
        Text,
        Image,
    };

    enum HAlign {
        Left,
        HCenter,
        Right,
    };

    enum VAlign {
        Top,
        VCenter,
        Bottom,
    };

    QString getTagSelected() const;
    void setTagSelected(const QString &value);

    QStringList getOnFuncs() const;
    void setOnFuncs(const QStringList &value);

    QStringList getOffFuncs() const;
    void setOffFuncs(const QStringList &value);

    bool getStateOnInitial() const;
    void setStateOnInitial(bool value);

    ShowContent getShowContent() const;
    void setShowContent(ShowContent value);

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

    HAlign getSzHAlign() const;
    void setSzHAlign(HAlign value);

    VAlign getSzVAlign() const;
    void setSzVAlign(VAlign value);

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
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    // 关联的变量
    QString tagSelected;
    // 切换到开功能操作
    QStringList onFuncs;
    // 切换到关功能操作
    QStringList offFuncs;
    // 初始状态
    bool stateOnInitial;
    // 显示内容
    ShowContent showContent;
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
    HAlign szHAlign;
    // 垂直对齐
    VAlign szVAlign;
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
};

#endif // QSWITCHBUTTON_H
