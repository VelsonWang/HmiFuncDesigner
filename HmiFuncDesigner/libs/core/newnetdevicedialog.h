#ifndef NewNetDeviceDialog_H
#define NewNetDeviceDialog_H

#include "corelibglobal.h"
#include "../shared/projdata/deviceinfo.h"
#include "../shared/property/qabstractproperty.h"
#include "./qpropertylist/qpropertylistview.h"
#include <QDialog>


namespace Ui
{
class NewNetDeviceDialog;
}

class CORELIB_EXPORT NewNetDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewNetDeviceDialog(QWidget *parent = NULL);
    ~NewNetDeviceDialog();

    void load(const QString &szName);
    void save(const QString &szName);

public:
    QString GetDeviceName() const;
    QString pluginName() const;
    // 更新PropertyEditor数据
    void updatePropertyEditor();

private:
    bool check_data();

public slots:
    void onPropertyEdit(QAbstractProperty *pro, const QVariant &value);

private slots:
    void on_btnHelp_clicked();
    void on_btnCheck_clicked();
    void on_btnOk_clicked();
    void on_btnExit_clicked();
    void on_btnDeviceSelect_clicked();
    void on_btnProtocolSelect_clicked();

private:
    QString getValue2ByValue1(const QString &szVal1, QVector<QPair<QString, QString>>& properties);
    void setValue2ByValue1(const QString &szVal1, const QString &szVal2, QVector<QPair<QString, QString>>& properties);

public:
    QString m_szDeviceName;
    QVector<QPair<QString, QString>> m_properties;
    QVector<QPair<QString, QString>> m_propType;

private:
    Ui::NewNetDeviceDialog *ui;
    QString m_szPluginName;
    QPropertyListView *m_propertyView;
};

#endif // NewNetDeviceDialog_H
