#ifndef GraphPageView_H
#define GraphPageView_H

#include <QWidget>
#include <QComboBox>
#include <QUndoStack>
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "GraphPage.h"

class QFormListWidget;
class StyledBar;
class QFormHost;
class QAbstractHost;
class QAbstractProperty;

class GraphPageView : public QWidget
{
    Q_OBJECT
public:
    explicit GraphPageView(QtVariantPropertyManager *propertyMgr,
                           QtTreePropertyBrowser *propertyEditor,
                           QWidget *parent = Q_NULLPTR);
    ~GraphPageView();

    GraphPage *createGraphPage(const QString &szName);

public slots:
    void setSelectObj(QAbstractHost* host);
    void onGraphPageSelected(QAbstractHost* host);

signals:
    void selectObj(QAbstractHost* host);

protected:
    QFormListWidget *m_formListWidget;
    QtVariantPropertyManager *m_pPropertyMgrObj = Q_NULLPTR;
    QtTreePropertyBrowser *m_pPropertyEditorObj = Q_NULLPTR;

};

#endif // GraphPageView_H
