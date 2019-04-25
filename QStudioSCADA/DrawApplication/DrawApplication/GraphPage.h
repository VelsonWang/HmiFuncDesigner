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
#include "functionproperty.h"


class GraphPage : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphPage(const QRectF &rect,QObject *parent = 0);

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

    void setPropertyModel(PropertyModel *model);
    QUndoStack *undoStack() const;

    void setActive(bool);
    bool active();

    QString getFileName() const;
    void setFileName(const QString &);

    QString getProjectPath() const;
    void setProjectPath(const QString &);

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

    friend QDataStream &operator<<(QDataStream &out, GraphPage &page);
    friend QDataStream &operator>>(QDataStream &in, GraphPage &page);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
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

private slots:
    void slotGroupElements();
    void slotUngroupElements();
    void slotAlignElements();
    void slotHUniformDistributeElements();
    void slotVUniformDistributeElements();
    void slotUpLayerElements();
    void slotDownLayerElements();
    void slotSaveAsLibrary();
    void slotEditDelete();
    void slotEditCopy();
    void slotEditPaste();
    void slotSelectAll();

public slots:
    void slotSelectionChanged();
    void slotElementPropertyChanged(Property *);
    void slotGraphPagePropertyChanged(Property *);
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
    QString projpath_;
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
    bool unsavedFlag;

    PropertyModel *propertyModel;
    QUndoStack *m_undoStack;
    QPixmap gridPixmap;
    QList <Property *> propList;
    QList <QGraphicsItem*> copyList;

    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    ColorProperty *backgroundProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;
    FunctionProperty *funcProperty;

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
    QAction *upLayerAction_;
    QAction *downLayerAction_;
    QAction *saveAsLibraryAction;
    QAction *actionDelete;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSelectAll;

};

#endif // GRAPHPAGE_H
