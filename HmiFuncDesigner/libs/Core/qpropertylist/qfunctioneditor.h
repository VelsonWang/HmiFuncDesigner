#ifndef QFUNCTIONEDITOR_H
#define QFUNCTIONEDITOR_H

#include "qbuttoncommoneditor.h"

class QFunctionEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QFunctionEditor(QAbstractProperty* property, QUndoStack* stack, QWidget *parent = 0);
    /**
         * @brief setSupportEvents
         * @details 设置支持的事件列表
         * @param events 事件列表
         */
    void setSupportEvents(QStringList events);

protected:
    void onBtnClicked();

private:
    QStringList supportEvents_;
};

#endif // QFONTEDITOR_H
