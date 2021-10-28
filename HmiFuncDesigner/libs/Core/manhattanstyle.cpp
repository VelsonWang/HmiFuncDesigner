#include "manhattanstyle.h"
#include "stylehelper.h"
#include <QStyleFactory>
#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QPainter>
#include <QPixmap>
#include <QStatusBar>
#include <QStyleFactory>
#include <QStyleOption>
#include <QToolBar>
#include <QToolButton>

bool styleEnabled(const QWidget *widget)
{
    const QWidget *p = widget;
    while (p) {
        if (p->property("_q_custom_style_disabled").toBool()) {
            return false;
        }
        p = p->parentWidget();
    }
    return true;
}

// Consider making this a QStyle state
bool panelWidget(const QWidget *widget)
{
    if (!widget) {
        return false;
    }

    // Do not style dialogs or explicitly ignored widgets
    //if ((widget->window()->windowFlags() & Qt::WindowType_Mask) == Qt::Dialog)
    //    return false;

    if (qobject_cast<const QTabBar *>(widget)) {
        return styleEnabled(widget);
    }

    const QWidget *p = widget;
    while (p) {
        if (qobject_cast<const QToolBar *>(p) ||
                qobject_cast<const QStatusBar *>(p) ||
                qobject_cast<const QMenuBar *>(p) ||
                p->property("panelwidget").toBool()) {
            return styleEnabled(widget);
        }
        if(p->property("no-ManhattanStyle").toBool()) {
            return false;
        }
        p = p->parentWidget();

    }
    return false;
}

bool lightColored(const QWidget *widget)
{
    if (!widget) {
        return false;
    }

    // Don't style dialogs or explicitly ignored widgets
    if ((widget->window()->windowFlags() & Qt::WindowType_Mask) == Qt::Dialog) {
        return false;
    }

    const QWidget *p = widget;
    while (p) {
        if (p->property("lightColored").toBool()) {
            return true;
        }
        p = p->parentWidget();
    }
    return false;
}

QPalette panelPalette(const QPalette &oldPalette, bool lightColored = false)
{
    QColor color = StyleHelper::panelTextColor(lightColored);
    QPalette pal = oldPalette;
    pal.setBrush(QPalette::All, QPalette::WindowText, color);
    pal.setBrush(QPalette::All, QPalette::ButtonText, color);
    pal.setBrush(QPalette::All, QPalette::Foreground, color);
    color.setAlpha(100);
    pal.setBrush(QPalette::Disabled, QPalette::WindowText, color);
    pal.setBrush(QPalette::Disabled, QPalette::ButtonText, color);
    pal.setBrush(QPalette::Disabled, QPalette::Foreground, color);
    return pal;
}


ManhattanStyle::ManhattanStyle(const QString &baseStyleName) :
    QProxyStyle(QStyleFactory::create(baseStyleName)),
    m_lineEditImage(":/images/inputfield.png"),
    m_lineEditImageDisabled(":/images/inputfield_disabled.png")
{
}

ManhattanStyle::~ManhattanStyle()
{
}

int ManhattanStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    int retval = 0;
    retval = QProxyStyle::pixelMetric(metric, option, widget);
    switch (metric) {
        case PM_SplitterWidth:
            if (widget && widget->property("minisplitter").toBool()) {
                retval = 1;
            }
            break;
        case PM_ToolBarIconSize:
            if (panelWidget(widget)) {
                retval = 16;
            }
            break;
        case PM_DockWidgetHandleExtent:
        case PM_DockWidgetSeparatorExtent:
            return 1;
        case PM_MenuPanelWidth:
        case PM_MenuBarHMargin:
        case PM_MenuBarVMargin:
        case PM_ToolBarFrameWidth:
            if (panelWidget(widget)) {
                retval = 1;
            }
            break;
        case PM_ButtonShiftVertical:
        case PM_ButtonShiftHorizontal:
        case PM_MenuBarPanelWidth:
        case PM_ToolBarItemMargin:
        case PM_ToolBarItemSpacing:
            if (panelWidget(widget)) {
                retval = 0;
            }
            break;
        case PM_DefaultFrameWidth:
            if (qobject_cast<const QLineEdit*>(widget) && panelWidget(widget)) {
                return 1;
            }
            break;
        default:
            break;
    }
    return retval;
}

void ManhattanStyle::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);

    if(panelWidget(widget)) {
        widget->setAttribute(Qt::WA_LayoutUsesWidgetRect, true);

        if(qobject_cast<QStatusBar*>(widget)) {
            widget->setFixedHeight(StyleHelper::navigationWidgetHeight());
        } else if(qobject_cast<QComboBox*>(widget)) {
            widget->setMaximumHeight(23);
            widget->setAttribute(Qt::WA_Hover);
        } else if (qobject_cast<QToolButton*>(widget)) {
            widget->setAttribute(Qt::WA_Hover);
            widget->setMaximumHeight(22);
        } else if (qobject_cast<QLineEdit*>(widget)) {
            widget->setAttribute(Qt::WA_Hover);
            widget->setMaximumHeight(StyleHelper::navigationWidgetHeight() - 2);
        }
    }
}

void ManhattanStyle::drawControl(ControlElement element, const QStyleOption *option,
                                 QPainter *painter, const QWidget *widget) const
{
    if (!panelWidget(widget)) {
        return QProxyStyle::drawControl(element, option, painter, widget);
    }

    switch (element) {
        case CE_Splitter:
            painter->fillRect(option->rect, StyleHelper::borderColor());
            break;

        case CE_TabBarTabShape:
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QStyleOptionTab adjustedTab = *tab;
                if (tab->cornerWidgets == QStyleOptionTab::NoCornerWidgets && (
                            tab->position == QStyleOptionTab::Beginning ||
                            tab->position == QStyleOptionTab::OnlyOneTab)) {
                    if (option->direction == Qt::LeftToRight) {
                        adjustedTab.rect = adjustedTab.rect.adjusted(-1, 0, 0, 0);
                    } else {
                        adjustedTab.rect = adjustedTab.rect.adjusted(0, 0, 1, 0);
                    }
                }
                QProxyStyle::drawControl(element, &adjustedTab, painter, widget);
                return;
            }
            break;

        case CE_MenuBarItem:
            painter->save();
            if (const QStyleOptionMenuItem *mbi = qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
                QColor highlightOutline = StyleHelper::borderColor().lighter(120);
                bool act = mbi->state & State_Sunken;
                bool dis = !(mbi->state & State_Enabled);
                StyleHelper::menuGradient(painter, option->rect, option->rect);
                QStyleOptionMenuItem item = *mbi;
                item.rect = mbi->rect;
                QPalette pal = mbi->palette;
                pal.setBrush(QPalette::ButtonText, dis ? Qt::gray : Qt::black);
                item.palette = pal;
                QCommonStyle::drawControl(element, &item, painter, widget);
                QRect r = option->rect;

                if (act) {
                    QColor baseColor = StyleHelper::baseColor();
                    QLinearGradient grad(option->rect.topLeft(), option->rect.bottomLeft());
                    grad.setColorAt(0, baseColor.lighter(120));
                    grad.setColorAt(1, baseColor.lighter(130));
                    painter->fillRect(option->rect.adjusted(1, 1, -1, 0), grad);

                    // Outline
                    painter->setPen(QPen(highlightOutline, 0));
                    painter->drawLine(QPoint(r.left(), r.top() + 1), QPoint(r.left(), r.bottom()));
                    painter->drawLine(QPoint(r.right(), r.top() + 1), QPoint(r.right(), r.bottom()));
                    painter->drawLine(QPoint(r.left() + 1, r.top()), QPoint(r.right() - 1, r.top()));
                    highlightOutline.setAlpha(60);
                    painter->setPen(QPen(highlightOutline, 0));
                    painter->drawPoint(r.topLeft());
                    painter->drawPoint(r.topRight());

                    QPalette pal = mbi->palette;
                    uint alignment = Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
                    if (!styleHint(SH_UnderlineShortcut, mbi, widget)) {
                        alignment |= Qt::TextHideMnemonic;
                    }
                    pal.setBrush(QPalette::Text, dis ? Qt::gray : QColor(0, 0, 0, 60));
                    drawItemText(painter, item.rect.translated(0, 1), alignment, pal, mbi->state & State_Enabled, mbi->text, QPalette::Text);
                    pal.setBrush(QPalette::Text, dis ? Qt::gray : Qt::white);
                    drawItemText(painter, item.rect, alignment, pal, mbi->state & State_Enabled, mbi->text, QPalette::Text);
                }
            }
            painter->restore();
            break;

        case CE_ComboBoxLabel:
            if (const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
                if (panelWidget(widget)) {
                    painter->save();
                    QRect editRect = subControlRect(CC_ComboBox, cb, SC_ComboBoxEditField, widget);
                    QPalette customPal = cb->palette;
                    bool drawIcon = !(widget && widget->property("hideicon").toBool());

                    if (!cb->currentIcon.isNull() && drawIcon) {
                        QIcon::Mode mode = cb->state & State_Enabled ? QIcon::Normal
                                           : QIcon::Disabled;
                        QPixmap pixmap = cb->currentIcon.pixmap(cb->iconSize, mode);
                        QRect iconRect(editRect);
                        iconRect.setWidth(cb->iconSize.width() + 4);
                        iconRect = alignedRect(cb->direction,
                                               Qt::AlignLeft | Qt::AlignVCenter,
                                               iconRect.size(), editRect);
                        if (cb->editable) {
                            painter->fillRect(iconRect, customPal.brush(QPalette::Base));
                        }
                        drawItemPixmap(painter, iconRect, Qt::AlignCenter, pixmap);

                        if (cb->direction == Qt::RightToLeft) {
                            editRect.translate(-4 - cb->iconSize.width(), 0);
                        } else {
                            editRect.translate(cb->iconSize.width() + 4, 0);
                        }
                        editRect.adjust(0, 0, -13, 0);
                    }

                    QLatin1Char asterisk('*');
                    int elideWidth = editRect.width();

                    bool notElideAsterisk = widget && widget->property("notelideasterisk").toBool()
                                            && cb->currentText.endsWith(asterisk)
                                            && option->fontMetrics.horizontalAdvance(cb->currentText) > elideWidth;

                    QString text;
                    if (notElideAsterisk) {
                        elideWidth -= option->fontMetrics.horizontalAdvance(asterisk);
                        text = asterisk;
                    }
                    text.prepend(option->fontMetrics.elidedText(cb->currentText, Qt::ElideRight, elideWidth));

                    if ((option->state & State_Enabled)) {
                        painter->setPen(QColor(0, 0, 0, 70));
                        painter->drawText(editRect.adjusted(1, 0, -1, 0), Qt::AlignLeft | Qt::AlignVCenter, text);
                    } else {
                        painter->setOpacity(0.8);
                    }
                    painter->setPen(StyleHelper::panelTextColor());
                    painter->drawText(editRect.adjusted(1, 0, -1, 0), Qt::AlignLeft | Qt::AlignVCenter, text);

                    painter->restore();
                } else {
                    QProxyStyle::drawControl(element, option, painter, widget);
                }
            }
            break;

        case CE_MenuBarEmptyArea: {
            StyleHelper::menuGradient(painter, option->rect, option->rect);
            painter->save();
            painter->setPen(StyleHelper::borderColor());
            painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());
            painter->restore();
        }
        break;

        case CE_ToolBar: {
            QRect rect = option->rect;
            bool horizontal = option->state & State_Horizontal;
            rect = option->rect;

            // Map offset for global window gradient
            QPoint offset = widget->window()->mapToGlobal(option->rect.topLeft()) -
                            widget->mapToGlobal(option->rect.topLeft());
            QRect gradientSpan;
            if (widget) {
                gradientSpan = QRect(offset, widget->window()->size());
            }

            bool drawLightColored = lightColored(widget);
            if (horizontal) {
                StyleHelper::horizontalGradient(painter, gradientSpan, rect, drawLightColored);
            } else {
                StyleHelper::verticalGradient(painter, gradientSpan, rect, drawLightColored);
            }

            if (!drawLightColored) {
                painter->setPen(StyleHelper::borderColor());
            } else {
                painter->setPen(QColor(0x888888));
            }

            if (horizontal) {
                // Note: This is a hack to determine if the
                // toolbar should draw the top or bottom outline
                // (needed for the find toolbar for instance)
                QColor lighter(StyleHelper::sidebarHighlight());
                if (drawLightColored) {
                    lighter = QColor(255, 255, 255, 180);
                }
                if (widget && widget->property("topBorder").toBool()) {
                    painter->drawLine(rect.topLeft(), rect.topRight());
                    painter->setPen(lighter);
                    painter->drawLine(rect.topLeft() + QPoint(0, 1), rect.topRight() + QPoint(0, 1));
                } else {
                    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
                    painter->setPen(lighter);
                    painter->drawLine(rect.topLeft(), rect.topRight());
                }
            } else {
                painter->drawLine(rect.topLeft(), rect.bottomLeft());
                painter->drawLine(rect.topRight(), rect.bottomRight());
            }
        }
        break;
        case CE_HeaderEmptyArea:
            return;
            break;
        case CE_HeaderSection: {
            StyleHelper::menuGradient(painter, option->rect, option->rect);
            painter->save();
            painter->setPen(StyleHelper::borderColor());
            painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());
            painter->restore();
            QRect re = option->rect;
            re.setLeft(re.right());
            drawButtonSeparator(painter, re, true);
        }
        break;
        default:
            QProxyStyle::drawControl(element, option, painter, widget);
            break;
    }
}

void ManhattanStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(!panelWidget(widget)) {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
    QRect rect = option->rect;
    switch(element) {
        case PE_IndicatorDockWidgetResizeHandle:
            painter->fillRect(rect, StyleHelper::borderColor());
            break;
        case PE_FrameDockWidget:
            QCommonStyle::drawPrimitive(element, option, painter, widget);
            break;
        case PE_FrameStatusBarItem:
            break;
        case PE_PanelStatusBar: {
            painter->save();

            QLinearGradient grad(rect.topLeft(), QPoint(rect.center().x(), rect.bottom()));
            QColor startColor = StyleHelper::shadowColor().darker(164);
            QColor endColor = StyleHelper::baseColor().darker(130);
            grad.setColorAt(0, startColor);
            grad.setColorAt(1, endColor);
            painter->fillRect(rect, grad);
            painter->setPen(QColor(255, 255, 255, 60));
            painter->drawLine(rect.topLeft() + QPoint(0, 1), rect.topRight() + QPoint(0, 1));
            painter->setPen(StyleHelper::borderColor().darker(110));
            painter->drawLine(rect.topLeft(), rect.topRight());

            painter->restore();
        }
        break;
        case PE_IndicatorArrowDown:
        case PE_IndicatorArrowLeft:
        case PE_IndicatorArrowRight:
        case PE_IndicatorArrowUp:
            StyleHelper::drawArrow(element, painter, option);
            break;
        case PE_PanelButtonTool: {
            bool pressed = option->state & State_Sunken || option->state & State_On;
            QColor shadow(0, 0, 0, 30);
            painter->setPen(shadow);
            if(pressed) {
                QColor shade(0, 0, 0, 40);
                painter->fillRect(rect, shade);
                painter->drawLine(rect.topLeft() + QPoint(1, 0), rect.topRight() - QPoint(1, 0));
                painter->drawLine(rect.topLeft(), rect.bottomLeft());
                painter->drawLine(rect.topRight(), rect.bottomRight());
                QColor hightlight(255, 255, 255, 30);
                painter->setPen(hightlight);
            } else if(option->state & State_Enabled && option->state & State_MouseOver) {
                QColor lighter(255, 255, 255, 37);
                painter->fillRect(rect, lighter);
            }
            if(option->state & State_HasFocus && (option->state & State_KeyboardFocusChange)) {
                QColor highlight = option->palette.highlight().color();
                highlight.setAlphaF(0.4);
                painter->setPen(QPen(highlight.lighter(), 1));
                highlight.setAlphaF(0.3);
                painter->setBrush(highlight);
                painter->setRenderHint(QPainter::Antialiasing);
                QRectF rect = option->rect;
                rect.translate(0.5, 0.5);
                painter->drawRoundedRect(rect.adjusted(2, 2, -3, -3), 2, 2);
            }
        }
        break;
        case PE_PanelLineEdit: {
            painter->save();

            QRect filedRect = option->rect.adjusted(1, 1, -1, -1);
            painter->setBrushOrigin(filedRect.topLeft());
            painter->fillRect(filedRect, option->palette.base());

            if(option->state & State_Enabled) {
                StyleHelper::drawCornerImage(m_lineEditImage, painter, option->rect, 5, 5, 5, 5);
            } else {
                StyleHelper::drawCornerImage(m_lineEditImageDisabled, painter, option->rect, 5, 5, 5, 5);
            }

            if(option->state & State_HasFocus || option->state & State_MouseOver) {
                QColor hover = StyleHelper::baseColor();
                if(option->state & State_HasFocus) {
                    hover.setAlpha(100);
                } else {
                    hover.setAlpha(50);
                }
                painter->setPen(QPen(hover, 1));
                painter->drawRect(option->rect.adjusted(1, 1, -2, -2));
            }

            painter->restore();
        }
        break;
        default:
            QProxyStyle::drawPrimitive(element, option, painter, widget);
            break;
    }
}

void ManhattanStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(!panelWidget(widget)) {
        QProxyStyle::drawComplexControl(control, option, painter, widget);
        return;
    }
    QRect rect = option->rect;
    switch(control) {
        case CC_ToolButton:
            if (const QStyleOptionToolButton *toolbutton = qstyleoption_cast<const QStyleOptionToolButton *>(option)) {
                bool reverse = option->direction == Qt::RightToLeft;
                bool drawborder = (widget && widget->property("showborder").toBool());

                if (drawborder) {
                    drawButtonSeparator(painter, rect, reverse);
                }

                QRect button, menuarea;
                button = subControlRect(control, toolbutton, SC_ToolButton, widget);
                menuarea = subControlRect(control, toolbutton, SC_ToolButtonMenu, widget);

                State bflags = toolbutton->state;
                if (bflags & State_AutoRaise) {
                    if (!(bflags & State_MouseOver)) {
                        bflags &= ~State_Raised;
                    }
                }

                State mflags = bflags;
                if (toolbutton->state & State_Sunken) {
                    if (toolbutton->activeSubControls & SC_ToolButton) {
                        bflags |= State_Sunken;
                    }
                    if (toolbutton->activeSubControls & SC_ToolButtonMenu) {
                        mflags |= State_Sunken;
                    }
                }

                QStyleOption tool(0);
                tool.palette = toolbutton->palette;
                if (toolbutton->subControls & SC_ToolButton) {
                    tool.rect = button;
                    tool.state = bflags;
                    drawPrimitive(PE_PanelButtonTool, &tool, painter, widget);
                }

                QStyleOptionToolButton label = *toolbutton;

                label.palette = panelPalette(option->palette, lightColored(widget));
                int fw = pixelMetric(PM_DefaultFrameWidth, option, widget);
                label.rect = button.adjusted(fw, fw, -fw, -fw);

                drawControl(CE_ToolButtonLabel, &label, painter, widget);

                if (toolbutton->subControls & SC_ToolButtonMenu) {
                    tool.state = mflags;
                    tool.rect = menuarea.adjusted(1, 1, -1, -1);
                    if (mflags & (State_Sunken | State_On | State_Raised)) {
                        painter->setPen(Qt::gray);
                        painter->drawLine(tool.rect.topLeft(), tool.rect.bottomLeft());
                        if (mflags & (State_Sunken)) {
                            QColor shade(0, 0, 0, 50);
                            painter->fillRect(tool.rect.adjusted(0, -1, 1, 1), shade);
                        }
                    }
                    tool.rect = tool.rect.adjusted(2, 2, -2, -2);
                    drawPrimitive(PE_IndicatorArrowDown, &tool, painter, widget);
                } else if (toolbutton->features & QStyleOptionToolButton::HasMenu
                           && !widget->property("noArrow").toBool()) {
                    int arrowSize = 6;
                    QRect ir = toolbutton->rect.adjusted(1, 1, -1, -1);
                    QStyleOptionToolButton newBtn = *toolbutton;
                    newBtn.palette = panelPalette(option->palette);
                    newBtn.rect = QRect(ir.right() - arrowSize - 1,
                                        ir.height() - arrowSize - 2, arrowSize, arrowSize);
                    drawPrimitive(PE_IndicatorArrowDown, &newBtn, painter, widget);
                }
            }
            break;
        case CC_ComboBox:
            if (const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
                painter->save();

                bool isEmpty = cb->currentText.isEmpty() && cb->currentIcon.isNull();
                bool reverse = option->direction == Qt::RightToLeft;
                bool drawborder = !(widget && widget->property("hideborder").toBool());
                bool alignarrow = !(widget && widget->property("alignarrow").toBool());

                if(drawborder) {
                    drawButtonSeparator(painter, rect, reverse);
                }
                QStyleOption toolbutton = *option;

                if(isEmpty) {
                    toolbutton.state &= ~(State_Enabled | State_Sunken);
                }
                painter->save();
                if(drawborder) {
                    painter->setClipRect(toolbutton.rect.adjusted(0, 0, -2, 0));
                }
                drawPrimitive(PE_PanelButtonTool, &toolbutton, painter, widget);
                painter->restore();

                int menuButtonWidth = 12;
                int left = !reverse ? rect.right() - menuButtonWidth : rect.left();
                int right = !reverse ? rect.right() : rect.left() + menuButtonWidth;

                QRect arrowRect((left + right) / 2 + (reverse ? 6 : -6), rect.center().y() - 3, 9, 9);
                if(!alignarrow) {
                    int labelwidth = option->fontMetrics.horizontalAdvance(cb->currentText);
                    if(reverse) {
                        arrowRect.moveLeft(qMax(rect.width() - labelwidth - menuButtonWidth - 2, 4));
                    } else {
                        arrowRect.moveLeft(qMin(labelwidth + menuButtonWidth - 2, rect.width() - menuButtonWidth - 4));
                    }
                }
                if(option->state & State_On) {
                    arrowRect.translate(QProxyStyle::pixelMetric(PM_ButtonShiftHorizontal, option, widget),
                                        QProxyStyle::pixelMetric(PM_ButtonShiftVertical, option, widget));
                }
                QStyleOption arrowOpt = *option;
                arrowOpt.rect = arrowRect;
                if(isEmpty) {
                    arrowOpt.state &= ~(State_Enabled | State_Sunken);
                }
                if(styleHint(SH_ComboBox_Popup, option, widget)) {
                    arrowOpt.rect.translate(0, -3);
                    drawPrimitive(PE_IndicatorArrowUp, &arrowOpt, painter, widget);
                    arrowOpt.rect.translate(0, 6);
                    drawPrimitive(PE_IndicatorArrowDown, &arrowOpt, painter, widget);
                } else {
                    drawPrimitive(PE_IndicatorArrowDown, &arrowOpt, painter, widget);
                }
                painter->restore();
            }
            break;
        default:
            QProxyStyle::drawComplexControl(control, option, painter, widget);
            break;
    }
}

void ManhattanStyle::drawButtonSeparator(QPainter *painter, const QRect &rect, bool reverse) const
{
    QLinearGradient grad(rect.topRight(), rect.bottomRight());
    grad.setColorAt(0, QColor(255, 255, 255, 20));
    grad.setColorAt(0.4, QColor(255, 255, 255, 60));
    grad.setColorAt(0.7, QColor(255, 255, 255, 50));
    grad.setColorAt(1, QColor(255, 255, 255, 40));
    painter->setPen(QPen(grad, 0));
    painter->drawLine(rect.topRight(), rect.bottomRight());
    grad.setColorAt(0, QColor(0, 0, 0, 30));
    grad.setColorAt(0.4, QColor(0, 0, 0, 70));
    grad.setColorAt(0.7, QColor(0, 0, 0, 70));
    grad.setColorAt(1, QColor(0, 0, 0, 40));
    painter->setPen(QPen(grad, 0));
    if(!reverse) {
        painter->drawLine(rect.topRight() - QPoint(1, 0), rect.bottomRight() - QPoint(1, 0));
    } else {
        painter->drawLine(rect.topLeft(), rect.bottomLeft());
    }
}
