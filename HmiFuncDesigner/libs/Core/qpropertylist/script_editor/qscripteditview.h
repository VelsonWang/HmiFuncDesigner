#ifndef QSCRIPTEDITVIEW_H
#define QSCRIPTEDITVIEW_H

#include <QPlainTextEdit>
#include <QTimeLine>
#include <QMetaMethod>

class QAbstractHost;
class QAbstractProperty;
class QHostObjectMap;
class QScriptHighLight;
class QScriptEditView;
class StyledBar;
class QActionToolBar;
class QCompleterTreeWidget;

class QLineWidget : public QWidget
{
    Q_OBJECT
public:
    QLineWidget(QTextDocument *document, QWidget* parent = 0);

protected:
    void mousePressEvent(QMouseEvent *);
    QSize sizeHint()const;
    void paintEvent(QPaintEvent *);

protected:
    QScriptEditView *editor()const;

protected:
    QTextDocument *m_document;

};

class QScriptEditView : public QPlainTextEdit
{
    Q_OBJECT
public:
    QScriptEditView(QAbstractProperty* property,
                    QAbstractHost *host,
                    QWidget* parent = 0);

    int extraAreaWidth() const;

    void painteLineWidget(QPainter *p);
    void extraAreaMouseEvent(QMouseEvent *e);

    QWidget *toolBar();

    void insertMethod(const QMetaMethod& method, QAbstractHost *host);

public slots:
    void findNext();
    void findPrev();
    void find(const QString text);
    void replace(const QString &text);
    void replaceNext(const QString &text);
    void replaceAll(const QString &text);

signals:
    void save();
    void find();

protected:
    void keyPressEvent(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
    void focusOutEvent(QFocusEvent *e);

    QStringList findNameList(QTextCursor cursor);
    bool checkName(const QString& name);

    QString get_current_text();

protected slots:
    void zoomOut();
    void zoomIn();
    void changedFont();

    void clipboardDataChanged();
    void updateLineWidget();
    void updateExtraAreaWidth();

    void positionChanged();
    void finish();

    void onToolTipTimer();

    void auto_select(const QString &text);

protected:
    QScriptHighLight *m_highlight;
    QLineWidget *m_lineWidget;
    QString m_findText;
    QTimeLine *m_timeLine;
    bool m_needShowCursor;
    QActionToolBar *m_toolBar;
    QTimer *m_toolTipTimer;
    bool m_updateTooltip;
    QAbstractHost* m_property_host;
    QHostObjectMap *m_host_object_map;
    QCompleterTreeWidget *m_completer;
    QAbstractProperty* m_property;
};

#endif // QSCRIPTEDITVIEW_H
