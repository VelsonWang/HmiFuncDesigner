#ifndef NewNetDeviceDialog_H
#define NewNetDeviceDialog_H

#include "../Public/Public.h"
#include "DeviceInfo.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include <QDialog>


namespace Ui {
class NewNetDeviceDialog;
}


class NewNetDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewNetDeviceDialog(QWidget *parent = 0);
    ~NewNetDeviceDialog();

    void load(int id);
    void save(int id);

public:
    NetDevice* GetNetDevice();
    QString GetDeviceName() const;
    // 设置已经建立的设备名称列表
    void SetListDeviceName(QStringList l);
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
    Ui::NewNetDeviceDialog *ui;
    NetDevice m_dev;
    QStringList m_ListDeviceName;
    QtVariantPropertyManager *m_pVariantPropertyManager;
    QtTreePropertyBrowser *m_pPropertyEditor;
    QtVariantEditorFactory *m_pVariantEditorFactory;
    QMap<QtProperty *, QString> m_propertyToId;
    QMap<QString, QtVariantProperty *> m_idToProperty;
    QMap<QString, bool> m_idToExpanded;
    QList<QtProperty*> m_listProp;

};

#endif // NewNetDeviceDialog_H
