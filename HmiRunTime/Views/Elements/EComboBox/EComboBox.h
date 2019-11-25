#ifndef EComboBox_H
#define EComboBox_H

#include <QObject>
#include "Public/IDrawGraphPage.h"


class EComboBox : public QObject, IDrawGraphPage
{
public:
    EComboBox();
    static IDrawGraphPage* creatObjFunc()
    {
        return new EComboBox;
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
    Element* createElement(QWidget *owner=nullptr) Q_DECL_OVERRIDE;

};

#endif // EComboBox_H
