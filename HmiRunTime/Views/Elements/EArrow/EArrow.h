#ifndef EARROW_H
#define EARROW_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class EArrow : public QObject, IDrawGraphPage
{
public:
    EArrow();

    static IDrawGraphPage* creatObjFunc()
    {
        return new EArrow;
    }

    // 初始化
    void initialize() Q_DECL_OVERRIDE;
    // 获取元素名称
    QString getElementName() Q_DECL_OVERRIDE;
    // 获取元素ID
    int getElementID() Q_DECL_OVERRIDE;
    // 获取元素ID String
    QString getElementIDString() Q_DECL_OVERRIDE;
    // 创建元素
    Element* createElement(QWidget *owner=0) Q_DECL_OVERRIDE;

signals:

private slots:

};

#endif // EARROW_H
