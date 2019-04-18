#ifndef EPOLYGON_H
#define EPOLYGON_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class EPolygon : public QObject, IDrawGraphPage
{
public:
    EPolygon();
    static IDrawGraphPage* creatObjFunc()
    {
        return new EPolygon;
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


};

#endif // EPOLYGON_H
