#ifndef QBYTEARRAYEDITOR_H
#define QBYTEARRAYEDITOR_H

#include <QLineEdit>
#include <QUndoStack>

class QAbstractProperty;

class QByteArrayEditor : public QLineEdit
{
    Q_OBJECT
public:
    Q_INVOKABLE QByteArrayEditor(QAbstractProperty* pro, QUndoStack* stack, QWidget *parent = 0);

signals:

protected slots:
    void onValueChanged();
    void onPropertyChanged();

protected:
    QAbstractProperty *m_property;
};

#endif // QBYTEARRAYEDITOR_H
