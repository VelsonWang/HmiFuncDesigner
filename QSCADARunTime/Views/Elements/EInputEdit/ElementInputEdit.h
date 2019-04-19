#ifndef INPUTEDITITEM_H
#define INPUTEDITITEM_H

#include "PublicDefine.h"
#include "Element.h"
#include "InputLineEdit.h"
#include <QPainter>
#include <QDataStream>
#include <QTimer>

class ElementInputEdit : public Element
{
    Q_OBJECT

public:
    explicit ElementInputEdit();
    ~ElementInputEdit();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {Type = InputEditItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementInputEdit &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawInputEdit(QPainter *painter);

private slots:
    void refreshTagValue();
    void enterPressed();
    void closePressed();

private:
    QRectF elementRect;
    // 允许编辑输入
    bool enableEdit_;
    // 关联的变量
    QString szTagSelected_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparentBackground_;
    // 字体
    QFont font_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 密码输入
    bool inputPassword_;
    // 初始有效性
    bool enableOnInitial_;
    // 初始可见性
    bool showOnInitial_;
    // 输入编辑控件，用于触发键盘输入
    InputLineEdit *inputLineEdit_;
    // 变量刷新定时器
    QTimer refreshTmr;
};

#endif // INPUTEDITITEM_H
