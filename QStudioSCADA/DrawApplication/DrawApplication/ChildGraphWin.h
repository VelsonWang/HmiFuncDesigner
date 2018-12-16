#ifndef CHILDGRAPHWIN_H
#define CHILDGRAPHWIN_H

#include <QWidget>
#include <QJsonObject>
#include <QVariant>
#include <QtProperty>
#include <QMap>
#include <QUndoCommand>
#include <QResizeEvent>

#include "PaintArea.h"
#include "GraphPage.h"
#include "IElement.h"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"




namespace Ui {
class ChildGraphWin;
}

class ChildGraphWin : public QWidget
{
    Q_OBJECT

public:
    explicit ChildGraphWin(QString filePathName, QString page, QWidget *parent = 0);
    ~ChildGraphWin();
    void ToolButtonClick(StdElement *e); // 工具箱选中控件元素

public:
    // 加载文档
    void load(const QJsonObject &json);
    // 保存文档
    void save(QJsonObject &json);
    // 获取画面属性
    void GetGraphPageProperty(QtTreePropertyBrowser *propertyBrowser);
    // 设置属性编辑器
    void SetPropertyBrowser(QtTreePropertyBrowser *propertyBrowser);
    // 绘制页面
    void DrawGraphPage();
    // 添加一个图形元素
    void AddGraphElement(StdElement *e);
    // 删除一个图形元素
    void DelGraphElement(StdElement *e);

public:
    bool isModified;
    QString m_FilePathName; // 全路径文件名

private:
    Ui::ChildGraphWin *ui;
    PaintArea* m_PaintArea;
    QtTreePropertyBrowser *m_pPropertyBrowser;
    QtVariantPropertyManager *m_pVariantManager;
    QtVariantEditorFactory *m_pVariantFactory;
    bool isSaved;
    QString curFile;  //保存当前路径
    bool maybeSave();  //是否保存文件
    bool saveFile(QString fileName);

    void addProperty(QtTreePropertyBrowser *propertyBrowser, QtVariantProperty *property, const QString &id);


private:
    QString m_GraphPageName; 
    GraphPage m_GraphPage;

    QMap<QtProperty *, QString> propertyToId;
    QMap<QString, QtVariantProperty *> idToProperty;
    QMap<QString, bool> idToExpanded;

private: // 画面属性
    // 背景颜色
    QColor m_BackColor;
    // 背景图片
    QImage m_Image;
    // 是否背景
    bool m_IsBackground;
    // 宽度
    int m_Width;
    // 高度
    int m_Height;

private:
    // 当前选中的元件
    StdElement *m_pCurrentGraph;

private slots:
    // 单击事件响应
    void mousePress(QMouseEvent * event);
    // 双击事件响应
    void mouseDoubleClick(QMouseEvent * event);
    // 左键拖动响应
    void mouseMove(QMouseEvent * event);
    // 鼠标释放
    void mouseRelease(QMouseEvent * event);

    void valueChanged(QtProperty *property, const QVariant &value);
    void slotsPaintAreaUpdate();

    // 删除元素
    void deleteElement();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void signalPaintAreaUpdate();

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void closeEvent(QCloseEvent *event);  // 关闭事件

};

#endif // CHILDGRAPHWIN_H
