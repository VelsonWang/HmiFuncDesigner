#ifndef INPUTEDITITEM_H
#define INPUTEDITITEM_H

#include "../../Public/Element.h"
#include "InputLineEdit.h"
#include <QPainter>
#include <QDataStream>


class ElementInputEdit : public Element
{
    Q_OBJECT

public:
    explicit ElementInputEdit();
    ~ElementInputEdit() override;
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int type() const {
        return InputEditItemType;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementInputEdit &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawInputEdit(QPainter *painter);
    void refreshTagValue();

private slots:
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
    // 处于编辑模式
    bool bInEditMode_;
};

#endif // INPUTEDITITEM_H
