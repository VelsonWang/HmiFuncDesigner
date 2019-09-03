#ifndef GRAPHPAGE_H
#define GRAPHPAGE_H

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsItem>
#include <QMimeData>
#include <QMenu>
#include <QDataStream>
#include <QUndoStack>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QFile>
#include "propertymodel.h"
#include "emptyproperty.h"
#include "textproperty.h"
#include "colorproperty.h"
#include "integerproperty.h"
#include "enumproperty.h"
#include "UndoCommand.h"
#include "Element.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

class GraphPage : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphPage(const QRectF &rect,
              QtVariantPropertyManager *propertyMgr,
              QtTreePropertyBrowser *propertyEditor,
              QObject *parent = Q_NULLPTR);

    void setGridVisible(bool);
    bool isGridVisible() const;

    void setGraphPageId(const QString &);
    QString getGraphPageId() const;

    void setGraphPageBackground(const QColor &);
    QColor getGraphPageBackground() const;

    void setGraphPageWidth(int);
    int getGraphPageWidth() const;

    void setGraphPageHeight(int);
    int getGraphPageHeight() const;

    void setSelectedFunctions(QStringList funcs);
    QStringList getSelectedFunctions();

    QUndoStack *undoStack() const;

    void setActive(bool);
    bool active();

    QString getFileName() const;
    void setFileName(const QString &);

    QString getProjectPath() const;
    void setProjectPath(const QString &);

    QString getProjectName() const;
    void setProjectName(const QString &);

    void fillGraphPagePropertyModel();

    void saveAsXML(const QString &filename);
    void saveAsBinary(const QString &filename);
    void loadAsXML(const QString &filename);
    void loadAsBinary(const QString &filename);

    void writeItems(QDataStream &out,const QList<QGraphicsItem*> &items);
    void readItems(QDataStream &in,int offset,bool select);

    void addElementEvent();
    void removeElementEvent();
    void cleanPropertyModel();
    void connectItem(Element *item);
    bool getUnsavedFlag();
    void setUnsavedFlag(bool bFlag = true);

    friend QDataStream &operator<<(QDataStream &out, GraphPage &page);
    friend QDataStream &operator>>(QDataStream &in, GraphPage &page);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void createItems(const QString &, QPointF);
    void fillGridPixmap();
    void createPropertyList();
    void createContextMenuActions();
    void populateCoordinates(const Qt::Alignment &,
                             QVector<double> *,
                             const QList <QGraphicsItem*> &);
    void copyItems(const QList<QGraphicsItem*> &items);
    void updateActions();
    void moveSelectedElements(int xOffset, int yOffset);

    //XML LOAD
    void readGraphPageConfig(QFile &file);
    void readGraphPageTag(QXmlStreamReader &xml);
    void setGraphPageAttributes(QXmlStreamReader &xml);
    Element *createElement(const QString &internalType);

    void loadLibrary(QByteArray &data);
    void readLibraryConfig(QFile &file);
    void readLibraryTag(QXmlStreamReader &xml);

    void getSupportEvents(QStringList &listValue);

    void writeGraphPage(QFile &file, GraphPage *graphPage);
    void writeLibrary(QFile &file, GraphPage *graphPage);
    void horizontalSort(QList<QGraphicsItem *> &dat);
    void verticalSort(QList<QGraphicsItem *> &dat);
    // 更新当前页面子控件所属页面大小
    void updateAllElementGraphPageSize(int width, int height);

public:
    void onAlignElements(Qt::Alignment alignment, QList<QGraphicsItem*> items);
    void onHUniformDistributeElements(QList<QGraphicsItem*> items);
    void onVUniformDistributeElements(QList<QGraphicsItem*> items);
    void onSetTheSameSizeElements(QList<QGraphicsItem*> items);
    void onUpLayerElements(QList<QGraphicsItem*> items);
    void onDownLayerElements(QList<QGraphicsItem*> items);
    void onEditDelete(QList<QGraphicsItem*> items);
    void onEditCopy(QList<QGraphicsItem*> items);
    void onEditPaste();

private slots:
    void slotGroupElements();
    void slotUngroupElements();
    void slotAlignElements();
    void slotHUniformDistributeElements();
    void slotVUniformDistributeElements();
    // 设置选中控件大小一致
    void slotSetTheSameSizeElements();
    void slotUpLayerElements();
    void slotDownLayerElements();
    void slotSaveAsLibrary();
    void slotEditDelete();
    void slotEditCopy();
    void slotEditPaste();
    void slotSelectAll();



public slots:
    void slotSelectionChanged();
    void slotElementPropertyChanged(QtProperty *property, const QVariant &value);
    void slotGraphPagePropertyChanged(QtProperty *property, const QVariant &value);
    void slotElementMoved(QPointF);
    void slotElementResized(int,int,QPointF);

signals:
    void newElementAdded();
    void changeGraphPageName();
    void elementsDeleted();
    void elementIdChanged();
    void signalShowAddProperties();
    void elementPropertyChanged();
    void GraphPagePropertyChanged();
    void GraphPageSaved();

private:
    QString szProjPath_;
    QString szProjName_;
    bool gridVisible;
    Element *currentItem;
    static const int gridSize = 20;
    static const int pasteOffset = 20;
    QString graphPageId;
    QString graphPagePriority;
    QString filename;
    QColor graphPageBackground;
    int graphPageWidth;
    int graphPageHeight;
    QStringList funcs_;

    bool onActive;
    bool unsavedFlag_;

    QUndoStack *m_undoStack;
    QPixmap gridPixmap;
    QList<QGraphicsItem*> copyList;

    QMenu contextMenu;
    QMenu contextServiceMenu;

    QAction *inGroupAction;
    QAction *outGroupAction;
    QAction *alignTopAction;
    QAction *alignDownAction;
    QAction *alignRightAction;
    QAction *alignLeftAction;
    QAction *hUniformDistributeAction_; // 水平均匀分布
    QAction *vUniformDistributeAction_; // 垂直均匀分布
    QAction *setTheSameSizeAction_; // 设置选中控件大小一致
    QAction *upLayerAction_;
    QAction *downLayerAction_;
    QAction *saveAsLibraryAction;
    QAction *actionDelete;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSelectAll;

private:
    void addProperty(QtVariantProperty *property, const QString &id);
    void updateExpandState();
    QList <QtProperty *> propList;
    QtVariantPropertyManager *variantPropertyManager_;
    QtTreePropertyBrowser *propertyEditor_;
    QMap<QtProperty *, QString> propertyToId_;
    QMap<QString, QtVariantProperty *> idToProperty_;
    QMap<QString, bool> idToExpanded_;

};

#endif // GRAPHPAGE_H
