
#ifndef VARIANTFACTORY_H
#define VARIANTFACTORY_H

#include "qtvariantproperty.h"

class FunctionEdit;

class VariantFactory : public QtVariantEditorFactory
{
    Q_OBJECT
public:
    VariantFactory(QObject *parent = Q_NULLPTR) : QtVariantEditorFactory(parent) { }
    virtual ~VariantFactory();

protected:
    virtual void connectPropertyManager(QtVariantPropertyManager *manager);
    virtual QWidget *createEditor(QtVariantPropertyManager *manager,
                                  QtProperty *property,
                                  QWidget *parent);
    virtual void disconnectPropertyManager(QtVariantPropertyManager *manager);

private slots:
    void slotPropertyChanged(QtProperty *property, const QVariant &value);
    void slotPropertyAttributeChanged(QtProperty *property,
                                      const QString &attribute,
                                      const QVariant &value);
    void slotSetValue(const QString &value);
    void slotEditorDestroyed(QObject *object);

private:
    QMap<QtProperty *, QList<QWidget *> > theCreatedEditors;
    QMap<QWidget *, QtProperty *> theEditorToProperty;
};



#endif
