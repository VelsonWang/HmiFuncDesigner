#include "qscripteditview.h"
#include "qscripthighlight.h"
#include "qhostobjectmap.h"
#include "qcompletertreewidget.h"
#include "../../stylehelper.h"
#include "../../styledbar.h"
#include "../../qactiontoolbar.h"
#include "../../qsoftcore.h"
#include "../../../shared/host/qabstracthost.h"
#include <QAction>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QLinearGradient>
#include <QScrollBar>
#include <QRegExp>
#include <QPainterPath>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QList>
#include <QMenu>
#include <QFontDialog>
#include <QMouseEvent>
#include <QRegExp>

static QString s_list = "+-*/\{}[] ;|()";

QLineWidget::QLineWidget(QTextDocument *document, QWidget *parent) :
    QWidget(parent),
    m_document(document)
{
}

QScriptEditView *QLineWidget::editor()const
{
    return qobject_cast<QScriptEditView*>(parent());
}

void QLineWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QRect re = this->rect().adjusted(0, 0, 1, 0);
    p.fillRect(re, QBrush(QColor(240, 240, 240)));
    editor()->painteLineWidget(&p);
}

QSize QLineWidget::sizeHint()const
{
    return QSize(editor()->extraAreaWidth(), 0);
}

void QLineWidget::mousePressEvent(QMouseEvent *e)
{
    editor()->extraAreaMouseEvent(e);
}


QScriptEditView::QScriptEditView(QAbstractProperty *property,
                                 QAbstractHost *host,
                                 QWidget *parent) :
    QPlainTextEdit(parent),
    m_lineWidget(new QLineWidget(document(), this)),
    m_needShowCursor(false),
    m_toolTipTimer(new QTimer(this)),
    m_updateTooltip(false),
    m_property_host(host),
    m_host_object_map(new QHostObjectMap(property, host)),
    m_completer(new QCompleterTreeWidget(this->viewport())),
    m_property(property)
{
    this->setWordWrapMode(QTextOption::NoWrap);
    this->setFrameStyle(QFrame::NoFrame);
    this->setMouseTracking(true);
    QFont f = this->font();
    f.setPointSize(12);
    f.setWeight(20);
    this->setFont(f);

    connect(m_completer, SIGNAL(select(QString)), this, SLOT(auto_select(QString)));

    m_completer->setVisible(false);

    m_highlight = new QScriptHighLight(document());

    m_toolBar = new QActionToolBar;

    QList<QAction*> list;
    QAction *ac;
    QSoftCore *core = QSoftCore::getCore();


    ac = core->getAction("ScriptEdit.Save");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::Save);
        connect(ac, SIGNAL(triggered()), this, SIGNAL(save()));
        ac->setEnabled(true);
        list.append(ac);
    }

    ac = new QAction(this);
    ac->setSeparator(true);
    list.append(ac);

    ac = core->getAction("ScriptEdit.Undo");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::Undo);
        connect(document(), SIGNAL(undoAvailable(bool)), ac, SLOT(setEnabled(bool)));
        connect(ac, SIGNAL(triggered()), this, SLOT(undo()));
        ac->setEnabled(false);
        list.append(ac);
    }

    ac = core->getAction("ScriptEdit.Redo");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::Redo);
        connect(document(), SIGNAL(redoAvailable(bool)), ac, SLOT(setEnabled(bool)));
        connect(ac, SIGNAL(triggered()), this, SLOT(redo()));
        ac->setEnabled(false);
        list.append(ac);
    }

    ac = new QAction(this);
    ac->setSeparator(true);
    list.append(ac);

    ac = core->getAction("ScriptEdit.Cut");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::Cut);
        connect(this, SIGNAL(copyAvailable(bool)), ac, SLOT(setEnabled(bool)));
        connect(ac, SIGNAL(triggered()), this, SLOT(cut()));
        ac->setEnabled(false);
        list.append(ac);
    }

    ac = core->getAction("ScriptEdit.Copy");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::Copy);
        connect(this, SIGNAL(copyAvailable(bool)), ac, SLOT(setEnabled(bool)));
        connect(ac, SIGNAL(triggered()), this, SLOT(copy()));
        ac->setEnabled(false);
        list.append(ac);
    }

    ac = core->getAction("ScriptEdit.Paste");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::Paste);
        connect(ac, SIGNAL(triggered()), this, SLOT(paste()));
        if(const QMimeData *md = QApplication::clipboard()->mimeData()) {
            ac->setEnabled(md->hasText());
        }
        connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
        list.append(ac);
    }

    ac = new QAction(this);
    ac->setSeparator(true);
    list.append(ac);

    ac = core->getAction("ScriptEdit.Zoomin");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::ZoomIn);
        connect(ac, SIGNAL(triggered()), this, SLOT(zoomIn()));
        list.append(ac);
    }

    ac = core->getAction("ScriptEdit.Zoomout");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::ZoomOut);
        connect(ac, SIGNAL(triggered()), this, SLOT(zoomOut()));
        list.append(ac);
    }

    ac = core->getAction("ScriptEdit.Font");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence("Ctrl+D"));
        list.append(ac);
        connect(ac, SIGNAL(triggered()), this, SLOT(changedFont()));
    }

    ac = new QAction(this);
    ac->setSeparator(true);
    list.append(ac);

    ac = core->getAction("ScriptEdit.Search");
    if(ac != NULL) {
        ac->setShortcut(QKeySequence::Find);
        connect(ac, SIGNAL(triggered()), this, SIGNAL(find()));
        list.append(ac);
    }

    ac = new QAction(this);
    ac->setSeparator(true);
    list.append(ac);


    m_toolBar->addButtonActions(list);

    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineWidget()));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineWidget()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(positionChanged()));

    m_timeLine = new QTimeLine(500, this);
    connect(m_timeLine, SIGNAL(finished()), this, SLOT(finish()));

    QPalette p = palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Normal, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Normal, QPalette::HighlightedText));
    setPalette(p);
    setProperty("no-ManhattanStyle", true);

    connect(m_toolTipTimer, SIGNAL(timeout()), this, SLOT(onToolTipTimer()));
    m_toolTipTimer->start(300);
}

void QScriptEditView::zoomOut()
{
    QFont f = font();
    f.setPointSize(f.pointSize() - 1);
    setFont(f);
    updateLineWidget();
}

void QScriptEditView::zoomIn()
{
    QFont f = font();
    f.setPointSize(f.pointSize() + 1);
    setFont(f);
    updateLineWidget();
}

void QScriptEditView::clipboardDataChanged()
{
    if(const QMimeData *md = QApplication::clipboard()->mimeData()) {
        QSoftCore *core = QSoftCore::getCore();
        QAction *ac = core->getAction("ScriptEdit.Paste");
        if(ac != NULL) {
            ac->setEnabled(md->hasText());
        }
    }
}

int QScriptEditView::extraAreaWidth()const
{
    int space = 0;
    const QFontMetrics fm(fontMetrics());
    int digits = 1;
    int max = qMax(1, document()->blockCount() + 1);
    while(max >= 10) {
        max /= 10;
        ++digits;
    }
    space += fm.width(QLatin1Char('9')) * digits;
    int markWidth = fm.lineSpacing();
    space += markWidth;
    return space;
}

void QScriptEditView::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect re = contentsRect();
    m_lineWidget->setGeometry(QRect(re.left(), re.top(), extraAreaWidth(), re.height()));
    updateExtraAreaWidth();
}

void QScriptEditView::updateExtraAreaWidth()
{
    setViewportMargins(extraAreaWidth(), 0, 0, 0);
}

void QScriptEditView::painteLineWidget(QPainter *p)
{
    QRect re = rect();
    const QFontMetrics fm(fontMetrics());
    int extraAreaWidth = m_lineWidget->width();
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    qreal top = blockBoundingGeometry(block).translated(contentOffset()).top() + 2;
    qreal bottom = top + blockBoundingRect(block).height();

    while(block.isValid() && top < re.bottom()) {
        if(block.isVisible() && bottom >= re.top()) {
            int lineNumber = blockNumber + 1;
            if(blockNumber == textCursor().block().blockNumber()) {
                p->setPen(QColor(40, 40, 40));
            } else {
                p->setPen(QColor(120, 120, 120));
            }
            QString number = QString::number(lineNumber);
            p->drawText(re.x(), (int)top, re.x() + extraAreaWidth - 4, fm.height(), Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void QScriptEditView::extraAreaMouseEvent(QMouseEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress) {
        if(e->button() == Qt::LeftButton) {
            QTextCursor cursor = cursorForPosition(QPoint(10, e->pos().y()));
            cursor.setPosition(cursor.block().position());
            cursor.select(QTextCursor::LineUnderCursor);
            setTextCursor(cursor);
            m_lineWidget->update();
        }
    }
}

void QScriptEditView::updateLineWidget()
{
    updateExtraAreaWidth();
    m_lineWidget->update();
    update();
}

void QScriptEditView::find(const QString text)
{
    if(text == m_findText) {
        return;
    }
    m_findText = text;
    if(m_findText != "") {
        QTextCursor t = document()->find(m_findText);
        if(!t.isNull()) {
            setTextCursor(t);
        }
    }
    finish();
    updateLineWidget();
}

void QScriptEditView::positionChanged()
{
    if(m_timeLine->state() == QTimeLine::Running) {
        m_timeLine->stop();
    }
    m_needShowCursor = false;
    viewport()->update();
    m_timeLine->setCurrentTime(0);
    m_timeLine->start();
    updateLineWidget();
}

void QScriptEditView::paintEvent(QPaintEvent *e)
{
    int r = 3;
    int dd = 2 * r;
    if(m_needShowCursor) {
        QTextCursor text = textCursor();

        text.select(QTextCursor::WordUnderCursor);
        QString str = text.selectedText();
        text.clearSelection();
        QRegExp reg("[_a-zA-Z][_a-zA-Z0-9]*");
        if(reg.indexIn(str) == 0 && reg.matchedLength() == str.size()) {
            QPainter p(viewport());
            int pos = 0;
            while(1) {
                text = document()->find(str, pos, QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords);
                if(text.isNull()) {
                    break;
                }
                text.clearSelection();
                text.movePosition(QTextCursor::PreviousWord);
                text.movePosition(QTextCursor::StartOfWord);
                QRect rr = cursorRect(text);
                text.movePosition(QTextCursor::EndOfWord);
                QRect re = cursorRect(text);
                pos = text.position();
                re = rr.united(re);
                QPen pen = QPen(QColor(0, 0, 0, 80), 1);
                pen.setJoinStyle(Qt::RoundJoin);
                p.setPen(pen);
                QLinearGradient d(re.topLeft(), re.bottomLeft());
                d.setColorAt(0, QColor(0, 0, 0, 10));
                d.setColorAt(1, QColor(0, 0, 0, 30));
                p.setBrush(d);
                QPainterPath pa;
                pa.moveTo(re.left() + r, re.top());
                pa.arcTo(re.left(), re.top(), dd, dd, 90, 90);
                pa.lineTo(re.left(), re.bottom() - r);
                pa.arcTo(re.left(), re.bottom() - dd, dd, dd, 180, 90);
                pa.lineTo(re.right() - r, re.bottom());
                pa.arcTo(re.right() - dd, re.bottom() - dd, dd, dd, 270, 90);
                pa.lineTo(re.right(), re.top() + r);
                pa.arcTo(re.right() - dd, re.top(), dd, dd, 0, 90);
                pa.closeSubpath();
                p.drawPath(pa);
            }
        }

    }
    QString ss = m_findText;
    ss.remove(" ");
    if(ss.size() > 0 && m_findText.size() > 1) {
        QPainter p(viewport());
        int pos = 0;
        QTextCursor text;
        while(1) {
            text = document()->find(m_findText, pos);
            if(text.isNull()) {
                break;
            }
            QTextCursor te = text;
            te.setPosition(text.selectionStart());
            QRect rr = cursorRect(te);
            te = text;
            te.setPosition(text.selectionEnd());
            QRect re = cursorRect(te);
            pos = text.position();
            re = rr.united(re);
            QPen pen = QPen(QColor(255, 239, 11).darker(150), 1);
            pen.setJoinStyle(Qt::RoundJoin);
            p.setPen(pen);
            QLinearGradient d(re.topLeft(), re.bottomLeft());
            d.setColorAt(0, QColor(255, 255, 0, 190));
            d.setColorAt(1, QColor(255, 255, 0, 130));
            p.setBrush(d);
            QPainterPath pa;
            pa.moveTo(re.left() + r, re.top());
            pa.arcTo(re.left(), re.top(), dd, dd, 90, 90);
            pa.lineTo(re.left(), re.bottom() - r);
            pa.arcTo(re.left(), re.bottom() - dd, dd, dd, 180, 90);
            pa.lineTo(re.right() - r, re.bottom());
            pa.arcTo(re.right() - dd, re.bottom() - dd, dd, dd, 270, 90);
            pa.lineTo(re.right(), re.top() + r);
            pa.arcTo(re.right() - dd, re.top(), dd, dd, 0, 90);
            pa.closeSubpath();
            p.drawPath(pa);
        }
    }

    QPlainTextEdit::paintEvent(e);
}

void QScriptEditView::finish()
{
    m_needShowCursor = true;
    viewport()->update();
}

void QScriptEditView::findNext()
{
    if(m_findText == "") {
        return;
    }
    QTextCursor t = document()->find(m_findText, textCursor());
    if(!t.isNull()) {
        setTextCursor(t);
        updateLineWidget();
    } else {
        QTextCursor t = document()->find(m_findText);
        if(!t.isNull()) {
            setTextCursor(t);
            updateLineWidget();
        }
    }
}

void QScriptEditView::findPrev()
{
    if(m_findText == "") {
        return;
    }
    QTextCursor t = document()->find(m_findText, textCursor(), QTextDocument::FindBackward);
    if(!t.isNull()) {
        setTextCursor(t);
        updateLineWidget();
    } else {
        QTextCursor t = document()->find(m_findText, this->toPlainText().size(), QTextDocument::FindBackward);
        if(!t.isNull()) {
            setTextCursor(t);
            updateLineWidget();
        }
    }
}

void QScriptEditView::replace(const QString &text)
{
    if(m_findText == text) {
        return;
    }
    if(m_findText == "") {
        return;
    }
    QTextCursor tt = textCursor();
    if(tt.selectedText().toLower() == m_findText.toLower()) {
        tt.insertText(text);
        updateLineWidget();
    }
}

void QScriptEditView::replaceNext(const QString &text)
{
    if(m_findText == "") {
        return;
    }
    replace(text);
    findNext();

}

void QScriptEditView::replaceAll(const QString &text)
{
    if(m_findText == "") {
        return;
    }
    QTextCursor tt;
    while(1) {
        tt = document()->find(m_findText, tt);
        if(tt.isNull()) {
            break;
        }
        tt.insertText(text);
    }
    updateLineWidget();
}

void QScriptEditView::keyPressEvent(QKeyEvent *e)
{
    QPlainTextEdit::keyPressEvent(e);

    QString text = e->text();
    QRegExp reg("[_a-zA-Z][_a-zA-Z0-9]{,64}");
    if(reg.exactMatch(text) || e->key() == Qt::Key_Backspace) {
        QString str = get_current_text();
        if(!str.startsWith(".") && str != "") {
            tagReturnList return_info = m_host_object_map->getInfo(str);
            if(return_info.m_returns.size() > 0) {
                m_completer->init(&return_info, str);
                QTextCursor cursor = textCursor();
                QRect r = cursorRect(cursor);
                m_completer->move_position(r);
                m_completer->setVisible(true);
            } else {
                m_completer->setVisible(false);
            }
        } else {
            m_completer->hide();
        }
    } else if(e->key() == Qt::Key_F4) {
        if(m_completer->isVisible()) {
            m_completer->hide();
        } else {
            QString str = get_current_text();
            tagReturnList return_info = m_host_object_map->getInfo(str);
            if(return_info.m_returns.size() > 0) {
                m_completer->init(&return_info, str);
                QTextCursor cursor = textCursor();
                QRect r = cursorRect(cursor);
                m_completer->move_position(r);
                m_completer->setVisible(true);
            } else {
                m_completer->setVisible(false);
            }
        }
    }
}

void QScriptEditView::focusOutEvent(QFocusEvent *e)
{
    m_completer->setVisible(false);
    QPlainTextEdit::focusOutEvent(e);
}

void QScriptEditView::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = new QMenu(this);

    menu->addActions(m_toolBar->actions());

    menu->exec(e->globalPos());

    delete menu;
}

void QScriptEditView::changedFont()
{
    QFont f = font();
    bool ok;
    f = QFontDialog::getFont(&ok, f, this, tr("Select Font"));
    if(ok) {
        setFont(f);
        f.setBold(false);
        f.setItalic(false);
        f.setUnderline(false);
        f.setStrikeOut(false);
        m_lineWidget->setFont(f);
        updateLineWidget();
    }
}

QWidget* QScriptEditView::toolBar()
{
    return m_toolBar;
}

void QScriptEditView::mouseMoveEvent(QMouseEvent *e)
{
    if(e->button() == Qt::NoButton) {
        m_updateTooltip = true;
    }
    QPlainTextEdit::mouseMoveEvent(e);
}

void QScriptEditView::mousePressEvent(QMouseEvent *e)
{
    m_completer->hide();
    QPlainTextEdit::mousePressEvent(e);
}

void QScriptEditView::onToolTipTimer()
{
}

QStringList QScriptEditView::findNameList(QTextCursor cursor)
{
    QStringList  list;

    QString temp;
    list.append(temp);

    return list;
}

bool QScriptEditView::checkName(const QString &name)
{
    QRegExp reg("[_a-zA-Z0-9]{,64}");
    return reg.exactMatch(name);
}

void QScriptEditView::insertMethod(const QMetaMethod &method, QAbstractHost *host)
{
    QString str;
    QAbstractHost *temp = host;
    while(!temp->inherits("QProjectHost")) {
        if(temp == m_property_host) {
            str = "self." + str;
            break;
        } else if(temp == m_property_host->getParent()) {
            str = "parent." + str;
            break;
        } else {
            str = temp->getPropertyValue("objectName").toString() + "." + str;
        }
        temp = temp->getParent();
    }
    QString name = method.methodSignature();
    int s = name.indexOf("(");
    name = name.left(s);
    str += (name + "()");
    if(method.typeName() == QString("")) {
        str += ";";
    }
    insertPlainText(str);
    QTextCursor c = textCursor();
    if(method.parameterNames().size() > 0) {
        if(str.endsWith(";")) {
            c.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 2);
        } else {
            c.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
        }
        setTextCursor(c);
    }
    setFocus();
}

QString QScriptEditView::get_current_text()
{
    QString str;

    QTextCursor cursor = this->textCursor();
    if(!cursor.atBlockEnd()) {
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
        QString str = cursor.selectedText();
        if(s_list.indexOf(str) == -1) {
            return "";
        }
    }
    cursor = this->textCursor();
    QString ss;
    QString temp;
    QRegExp reg("[_a-zA-Z][_a-zA-Z0-9]{,64}");
    while(1) {
        if(cursor.atBlockStart()) {
            if(temp != "") {
                if(!reg.exactMatch(temp)) {
                    return "";
                }
            }
            str = temp + str;
            break;
        }
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
        ss = cursor.selectedText().at(0);
        if((ss >= "A" && ss <= "Z") || (ss >= "a" && ss <= "z") || (ss == "_") || (ss >= "0" && ss <= "9")) {
            temp = ss + temp;
        } else if(ss == ".") {
            if(temp != "") {
                if(!reg.exactMatch(temp)) {
                    return "";
                }
            }
            str = "." + temp + str;
            temp = "";
        } else {
            if(temp != "") {
                if(!reg.exactMatch(temp)) {
                    return "";
                }
            }
            str = temp + str;
            break;
        }
    }
    QStringList l = str.split(".");
    if(l.size() > 1) {
        for(int i = 0; i < l.size() - 1; i++) {
            if(l.at(i) == "") {
                return "";
            }
        }
    }
    return str;
}

void QScriptEditView::auto_select(const QString &text)
{
    QTextCursor cur = textCursor();
    int count = 0;
    while(1) {
        if(cur.atBlockStart()) {
            break;
        }
        cur.clearSelection();
        cur.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
        QString s = cur.selectedText();
        if((s >= "A" && s <= "Z") || (s >= "a" && s <= "z") || (s == "_") || (s >= "0" && s <= "9")) {
            count++;
        } else {
            break;
        }

    }
    if(count > 0) {
        cur = textCursor();
        cur.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, count);
        setTextCursor(cur);
    }

    tagReturnList   *ret_list = m_completer->returnInfo();
    QString str = text;
    tagReturnInfo info = ret_list->m_name_to_return.value(str);
    if(info.m_type == TYPE_FORM) {
        str += ".";
        insertPlainText(str);
        QString s = m_completer->get_source_text();
        int index = s.lastIndexOf(".");
        if(index > 0) {
            s = s.left(index) + "." + text;
        } else {
            s = text;
        }
        s += ".";
        tagReturnList return_info = m_host_object_map->getInfo(s);
        if(return_info.m_returns.size() > 0) {
            m_completer->init(&return_info, s);
            QTextCursor cursor = textCursor();
            QRect r = cursorRect(cursor);
            m_completer->move_position(r);
            m_completer->setVisible(true);
        } else {
            m_completer->setVisible(false);
        }
    } else if(info.m_type == TYPE_FUNCTION) {
        QMetaMethod method = info.m_method;
        if(method.typeName() == QString("")) {
            str += "();";
            insertPlainText(str);
            if(method.parameterNames().size() > 0) {
                cur = textCursor();
                cur.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 2);
                setTextCursor(cur);
            }
        } else {
            str += "()";
            insertPlainText(str);
            if(method.parameterNames().size() > 0) {
                cur = textCursor();
                cur.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
                setTextCursor(cur);
            }
        }
    } else if(info.m_type == TYPE_DATA) {
        if(!info.m_is_last) {
            insertPlainText(str);
            QString s = m_completer->get_source_text();
            int index = s.lastIndexOf(".");
            if(index > 0) {
                s = s.left(index) + "." + text;
            } else {
                s = text;
            }
            s += ".";
            tagReturnList return_info = m_host_object_map->getInfo(s);
            if(return_info.m_returns.size() > 0) {
                m_completer->init(&return_info, s);
                QTextCursor cursor = textCursor();
                QRect r = cursorRect(cursor);
                m_completer->move_position(r);
                m_completer->setVisible(true);
            } else {
                m_completer->setVisible(false);
            }
        } else {
            insertPlainText(str);
        }
    } else {
        insertPlainText(str);
    }
}

