#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QString>
#include <QAction>
#include "Core_global.h"

class QActionArray
{
public:
    QActionArray() : m_type(""), m_pActionObj(Q_NULLPTR){}
    ~QActionArray() {
        QMap<QString, QActionArray*>::iterator iter;
        iter = m_children.begin();
        while (iter != m_children.end()) {
            QActionArray* pObj = iter.value();
            ++iter;
            delete pObj;
        }

        m_children.clear();
        //if(m_pActionObj != Q_NULLPTR) delete m_pActionObj;
    }
    QString m_type;
    QMap<QString, QActionArray*> m_children;
    QAction* m_pActionObj;
};

class CORESHARED_EXPORT Core
{
public:
    explicit Core();
    ~Core();
    static Core* getInstance();

public:
    // 添加全局action
    void insertAction(const QString& szKey, QAction *pActObj);
    // 获取全局action
    QAction* getAction(const QString &szKey);

protected:
    QActionArray m_actions; // 全局action


private:  

    Q_DISABLE_COPY(Core)
};


#endif // CORE_H
