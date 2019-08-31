#include "variantfactory.h"
#include "variantmanager.h"
#include "functionedit.h"
#include <QDebug>

VariantFactory::~VariantFactory()
{
    QList<FunctionEdit *> editors = theEditorToProperty.keys();
    QListIterator<FunctionEdit *> it(editors);
    while (it.hasNext())
        delete it.next();
}

void VariantFactory::connectPropertyManager(QtVariantPropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));
    connect(manager, SIGNAL(attributeChanged(QtProperty *, const QString &, const QVariant &)),
                this, SLOT(slotPropertyAttributeChanged(QtProperty *, const QString &, const QVariant &)));
    QtVariantEditorFactory::connectPropertyManager(manager);
}

QWidget *VariantFactory::createEditor(QtVariantPropertyManager *manager,
        QtProperty *property, QWidget *parent)
{
    if (manager->propertyType(property) == VariantManager::functionTypeId()) {
        FunctionEdit *editor = new FunctionEdit(parent);
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__
                 << manager->value(property).toString()
                 << manager->attributeValue(property, QLatin1String("supportevents")).toString()
                 << manager->attributes(VariantManager::functionTypeId());
        editor->setFunctions(manager->value(property).toString().split('|'));
        editor->setSupportEvents(manager->attributeValue(property, QLatin1String("supportevents")).toString().split('|'));
        theCreatedEditors[property].append(editor);
        theEditorToProperty[editor] = property;

        connect(editor, SIGNAL(filePathChanged(const QString &)),
                this, SLOT(slotSetValue(const QString &)));
        connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
        return editor;
    }
    return QtVariantEditorFactory::createEditor(manager, property, parent);
}

void VariantFactory::disconnectPropertyManager(QtVariantPropertyManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));
    disconnect(manager, SIGNAL(attributeChanged(QtProperty *, const QString &, const QVariant &)),
                this, SLOT(slotPropertyAttributeChanged(QtProperty *, const QString &, const QVariant &)));
    QtVariantEditorFactory::disconnectPropertyManager(manager);
}

void VariantFactory::slotPropertyChanged(QtProperty *property,
                const QVariant &value)
{
    if (!theCreatedEditors.contains(property))
        return;

    QList<FunctionEdit *> editors = theCreatedEditors[property];
    QListIterator<FunctionEdit *> itEditor(editors);
    while (itEditor.hasNext())
        itEditor.next()->setFunctions(value.toString().split('|'));
}

void VariantFactory::slotPropertyAttributeChanged(QtProperty *property,
            const QString &attribute, const QVariant &value)
{
    if (!theCreatedEditors.contains(property))
        return;

    if (attribute == QLatin1String("supportevents")) {
        QList<FunctionEdit *> editors = theCreatedEditors[property];
        QListIterator<FunctionEdit *> itEditor(editors);
        while (itEditor.hasNext())
            itEditor.next()->setFunctions(value.toString().split('|'));
    }
}

void VariantFactory::slotSetValue(const QString &value)
{
    QObject *object = sender();
    QMap<FunctionEdit *, QtProperty *>::ConstIterator itEditor =
                theEditorToProperty.constBegin();
    while (itEditor != theEditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            QtVariantPropertyManager *manager = propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
        itEditor++;
    }
}

void VariantFactory::slotEditorDestroyed(QObject *object)
{
    QMap<FunctionEdit *, QtProperty *>::ConstIterator itEditor =
                theEditorToProperty.constBegin();
    while (itEditor != theEditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            FunctionEdit *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            theEditorToProperty.remove(editor);
            theCreatedEditors[property].removeAll(editor);
            if (theCreatedEditors[property].isEmpty())
                theCreatedEditors.remove(property);
            return;
        }
        itEditor++;
    }
}

