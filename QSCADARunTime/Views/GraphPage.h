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
#include "IDrawGraphPage.h"
#include "propertymodel.h"
#include "emptyproperty.h"
#include "textproperty.h"
#include "colorproperty.h"
#include "integerproperty.h"
#include "enumproperty.h"
#include "UndoCommand.h"
#include "Element.h"
#include <QFile>

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

    QUndoStack *undoStack() const;

    void setActive(bool);
    bool active();

    QString getFileName() const;
    void setFileName(const QString &);

    QString getProjectPath() const;
    void setProjectPath(const QString &);

    void loadAsXML(const QString &filename);
    void loadAsBinary(const QString &filename);
    void readItems(QDataStream &in,int offset,bool select);

    void addElementEvent();
    void removeElementEvent();
    void cleanPropertyModel();
    bool getUnsavedFlag();

    friend QDataStream &operator>>(QDataStream &in, GraphPage &GraphPage);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void createItems(const QString &, QPointF);
    void fillGridPixmap();
    void createPropertyList();
    void createContextMenuActions();
    void populateCoordinates(const Qt::Alignment &,
                             QVector<double> *,
                             const QList <QGraphicsItem*> &);
    void moveSelectedElements(int xOffset, int yOffset);

    //XML LOAD
    void readGraphPageConfig(QFile &file);
    void readGraphPageTag(QXmlStreamReader &xml);
    void setGraphPageAttributes(QXmlStreamReader &xml);
    Element *createElement(const QString &internalType);

    void loadLibrary(QByteArray &data);
    void readLibraryConfig(QFile &file);
    void readLibraryTag(QXmlStreamReader &xml);

private slots:
    void slotGroupElements();
    void slotUngroupElements();
    void slotAlignElements();
    void slotFrontPlanElements();
    void slotBehindPlanElements();
    void slotShowAdditionalProperties();
    void slotAddNodePoint();
    void slotRemoveNodePoint();

signals:
    void newElementAdded();
    void changeGraphPageName();
    void elementsDeleted();
    void elementIdChanged();
    void signalShowAddProperties();
    void GraphPageSaved();

private:
    QString projpath_;
    bool gridVisible;
    Element *currentItem;
    static const int gridSize = 20;
    static const int pasteOffset = 20;
    QString GraphPageId;
    QString GraphPagePriority;
    QString filename;
    QColor GraphPageBackground;
    int GraphPageWidth;
    int GraphPageHeight;

    bool onActive;
    bool unsavedFlag;

    QUndoStack *m_undoStack;
    QPixmap gridPixmap;
    QList <Property *> propList;
    QList <QGraphicsItem*> copyList;

    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    ColorProperty *backgroundProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;
};

// 注册元素对象创建函数
extern void registerCreateObjectFunc();

#endif // GRAPHPAGE_H
