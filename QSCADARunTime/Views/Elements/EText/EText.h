#ifndef ETEXT_H
#define ETEXT_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class EText : public QObject, IDrawGraphPage
{
public:
    EText();
    static IDrawGraphPage* creatObjFunc()
    {
        return new EText;
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
    Element* createElement() Q_DECL_OVERRIDE;

};

#endif // ETEXT_H
