#ifndef NEWCOMDEVICEDIALOG_H
#define NEWCOMDEVICEDIALOG_H

#include "../Public/Public.h"
#include "DeviceInfo.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include <QDialog>

namespace Ui {
class NewComDeviceDialog;
}


class NewComDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewComDeviceDialog(QWidget *parent = Q_NULLPTR);
    ~NewComDeviceDialog();

    void load(const QString &szName);
    void save(const QString &szName);

public:
    QString GetDeviceName() const;
    // 更新PropertyEditor数据
    void updatePropertyEditor();

private:
    bool check_data();

public slots:
    void onPropertyValueChanged(QtProperty *property, const QVariant &value);

private slots:
    void on_btnHelp_clicked();
    void on_btnCheck_clicked();
    void on_btnOk_clicked();
    void on_btnExit_clicked();
    void on_btnDeviceSelect_clicked();
    void on_btnProtocolSelect_clicked();

private:
    void createPropertyList();
    void updatePropertyModel();
    void addProperty(QtVariantProperty *property, const QString &id, bool bAddToList = true);
    void clearProperties();
    QString getValue2ByValue1(const QString &szVal1, QVector<QPair<QString, QString>>& properties);
    void setValue2ByValue1(const QString &szVal1, const QString &szVal2, QVector<QPair<QString, QString>>& properties);

public:
    QString m_szDeviceName;
    QVector<QPair<QString, QString>> m_properties;
    QVector<QPair<QString, QString>> m_prop_type;

private:
    Ui::NewComDeviceDialog *ui;
    QString m_szPluginName;
    QtVariantPropertyManager *m_pVariantPropertyManager;
    QtTreePropertyBrowser *m_pPropertyEditor;
    QtVariantEditorFactory *m_pVariantEditorFactory;
    QMap<QtProperty *, QString> m_propertyToId;
    QMap<QString, QtVariantProperty *> m_idToProperty;
    QMap<QString, bool> m_idToExpanded;
    QList<QtProperty*> m_listProp;

};

#endif // NEWCOMDEVICEDIALOG_H
