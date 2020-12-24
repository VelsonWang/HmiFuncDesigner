#ifndef NEWCOMDEVICEDIALOG_H
#define NEWCOMDEVICEDIALOG_H

#include "corelibglobal.h"
#include "../shared/projdata/deviceinfo.h"
#include "../shared/property/qabstractproperty.h"
#include "./qpropertylist/qpropertylistview.h"
#include <QDialog>
#include <QVector>

namespace Ui {
class NewComDeviceDialog;
}

class CORELIB_EXPORT NewComDeviceDialog : public QDialog
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
    QVector<QPair<QString, QString>> m_prop_type;

private:
    Ui::NewComDeviceDialog *ui;
    QString m_szPluginName;
    QPropertyListView *m_propertyView;
};

#endif // NEWCOMDEVICEDIALOG_H
