#include "variantfactory.h"
#include "variantmanager.h"
#include "functionedit.h"
#include "tagcolorlistedit.h"
#include "tagtextlistedit.h"
#include "fileedit.h"
#include "scriptedit.h"

VariantFactory::~VariantFactory()
{
    QList<QWidget *> editors = theEditorToProperty.keys();
    QListIterator<QWidget *> it(editors);
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
                                      QtProperty *property,
                                      QWidget *parent)
{
    if (manager->propertyType(property) == VariantManager::functionTypeId()) {
        FunctionEdit *editor = new FunctionEdit(parent);
        editor->setFunctions(manager->value(property).toString().split('|'));
        editor->setSupportEvents(manager->attributeValue(property, QLatin1String("supportevents")).toString().split('|'));
        theCreatedEditors[property].append(editor);
        theEditorToProperty[editor] = property;

        connect(editor, SIGNAL(functionsChanged(const QString &)),
                this, SLOT(slotSetValue(const QString &)));
        connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
        return editor;
    } else if (manager->propertyType(property) == VariantManager::tagColorListTypeId()) {
        TagColorListEdit *editor = new TagColorListEdit(parent);
        editor->setValueColorList(manager->value(property).toString().split('|'));
        theCreatedEditors[property].append(editor);
        theEditorToProperty[editor] = property;

        connect(editor, SIGNAL(valueColorListChanged(const QString &)),
                this, SLOT(slotSetValue(const QString &)));
        connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
        return editor;
    } else if (manager->propertyType(property) == VariantManager::TagTextListTypeId()) {
        TagTextListEdit *editor = new TagTextListEdit(parent);
        editor->setValueTextList(manager->value(property).toString().split('|'));
        theCreatedEditors[property].append(editor);
        theEditorToProperty[editor] = property;

        connect(editor, SIGNAL(valueTextListChanged(const QString &)),
                this, SLOT(slotSetValue(const QString &)));
        connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
        return editor;
    } else if (manager->propertyType(property) == VariantManager::filePathTypeId()) {
        FileEdit *editor = new FileEdit(parent);
        editor->setFilePath(manager->value(property).toString());
        editor->setFilter(manager->attributeValue(property, QLatin1String("filter")).toString());
        theCreatedEditors[property].append(editor);
        theEditorToProperty[editor] = property;

        connect(editor, SIGNAL(filePathChanged(const QString &)),
                this, SLOT(slotSetValue(const QString &)));
        connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
        return editor;
    } else if (manager->propertyType(property) == VariantManager::scriptTypeId()) {
        ScriptEdit *editor = new ScriptEdit(parent);
        editor->setScript(manager->value(property).toString());
        editor->setSupportEvents(manager->attributeValue(property, QLatin1String("supportevents")).toString().split('|'));
        theCreatedEditors[property].append(editor);
        theEditorToProperty[editor] = property;

        connect(editor, SIGNAL(scriptChanged(const QString &)),
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

void VariantFactory::slotPropertyChanged(QtProperty *property, const QVariant &value)
{
    if (!theCreatedEditors.contains(property))
        return;

    QtVariantPropertyManager *manager = propertyManager(property);
    QList<QWidget *> editors = theCreatedEditors[property];
    QListIterator<QWidget *> itEditor(editors);
    while (itEditor.hasNext()) {
        if (manager->propertyType(property) == VariantManager::functionTypeId()) {
            FunctionEdit *pFunctionEdit = dynamic_cast<FunctionEdit *>(itEditor.next());
            pFunctionEdit->setFunctions(value.toString().split('|'));
        } else if (manager->propertyType(property) == VariantManager::tagColorListTypeId()) {
            TagColorListEdit *pTagColorListEdit = dynamic_cast<TagColorListEdit *>(itEditor.next());
            pTagColorListEdit->setValueColorList(value.toString().split('|'));
        } else if (manager->propertyType(property) == VariantManager::TagTextListTypeId()) {
            TagTextListEdit *pTagTextListEdit = dynamic_cast<TagTextListEdit *>(itEditor.next());
            pTagTextListEdit->setValueTextList(value.toString().split('|'));
        } else if (manager->propertyType(property) == VariantManager::filePathTypeId()) {
            FileEdit *pFileEdit = dynamic_cast<FileEdit *>(itEditor.next());
            pFileEdit->setFilePath(value.toString());
        } else if (manager->propertyType(property) == VariantManager::scriptTypeId()) {
            ScriptEdit *pScriptEdit = dynamic_cast<ScriptEdit *>(itEditor.next());
            pScriptEdit->setScript(value.toString());
        }
    }
}

void VariantFactory::slotPropertyAttributeChanged(QtProperty *property,
            const QString &attribute, const QVariant &value)
{
    if (!theCreatedEditors.contains(property))
        return;

    QtVariantPropertyManager *manager = propertyManager(property);
    if (manager->propertyType(property) == VariantManager::functionTypeId()) {
        if (attribute == QLatin1String("supportevents")) {
            QList<QWidget *> editors = theCreatedEditors[property];
            QListIterator<QWidget *> itEditor(editors);
            while (itEditor.hasNext()) {
                FunctionEdit *pFunctionEdit = dynamic_cast<FunctionEdit *>(itEditor.next());
                pFunctionEdit->setSupportEvents(value.toString().split('|'));
            }
        }
    } else if (manager->propertyType(property) == VariantManager::tagColorListTypeId()) {

    } else if (manager->propertyType(property) == VariantManager::TagTextListTypeId()) {

    } else if (manager->propertyType(property) == VariantManager::filePathTypeId()) {
        if (attribute == QLatin1String("filter")) {
            QList<QWidget *> editors = theCreatedEditors[property];
            QListIterator<QWidget *> itEditor(editors);
            while (itEditor.hasNext()) {
                FileEdit *pFileEdit = dynamic_cast<FileEdit *>(itEditor.next());
                pFileEdit->setFilter(value.toString());
            }
        }
    } else if (manager->propertyType(property) == VariantManager::scriptTypeId()) {
        if (attribute == QLatin1String("supportevents")) {
            QList<QWidget *> editors = theCreatedEditors[property];
            QListIterator<QWidget *> itEditor(editors);
            while (itEditor.hasNext()) {
                ScriptEdit *pScriptEdit = dynamic_cast<ScriptEdit *>(itEditor.next());
                pScriptEdit->setSupportEvents(value.toString().split('|'));
            }
        }
    }
}

void VariantFactory::slotSetValue(const QString &value)
{
    QObject *object = sender();
    QMap<QWidget *, QtProperty *>::ConstIterator itEditor = theEditorToProperty.constBegin();
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
    QMap<QWidget *, QtProperty *>::ConstIterator itEditor = theEditorToProperty.constBegin();
    while (itEditor != theEditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QWidget *editor = itEditor.key();
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

