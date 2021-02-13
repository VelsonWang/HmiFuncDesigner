#ifndef QCOMMONEDITOR_H
#define QCOMMONEDITOR_H

#include <QWidget>
#include <QLabel>
#include <QUndoStack>

class QAbstractProperty;

class QCommonEditor : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE QCommonEditor(QAbstractProperty* property, QUndoStack* stack, QWidget *parent = 0);
    
signals:
    
public slots:
    void onPropertyChanged();

protected:
    QLabel *m_icon;
    QLabel *m_text;
    QAbstractProperty *m_property;
};

#endif // QCOMMONEDITOR_H
