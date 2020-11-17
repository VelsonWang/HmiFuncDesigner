#ifndef TagEditDialog_H
#define TagEditDialog_H

#include <QDialog>
#include <QMap>
#include <QStringList>
#include <QJsonObject>
#include "Core_global.h"

namespace Ui {
class TagEditDialog;
}

class CORESHARED_EXPORT TagEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagEditDialog(QWidget *parent = nullptr);
    ~TagEditDialog();
    // 更新UI
    void updateUI();
    // 设置地址类型和数据类型
    void setAddrTypeAndDataType(QMap<QString, QStringList> mapAddrType, QMap<QString, QStringList> mapDataType);
    // 设置地址类型的限制范围
    void setAddrTypeLimit(QMap<QString, QMap<QString, quint32>> mapLimit);
    // 设置变量信息
    void setTagObj(QJsonObject &jsonTag);
    // 返回变量信息
    QJsonObject getTagObj();

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 地址类型改变
    void on_cboAddrType_currentTextChanged(const QString &szAddrType);
    // 地址类型改变
    void on_cboAddrType2_currentTextChanged(const QString &szAddrType);

private:
    Ui::TagEditDialog *ui;
    QJsonObject m_jsonTagObj; // 变量
    QMap<QString, QStringList> m_mapAddrType;
    QMap<QString, QStringList> m_mapAddrTypeToDataType;
    QMap<QString, QMap<QString, quint32>> m_mapAddrTypeToLimit;
};

#endif // TagEditDialog_H
