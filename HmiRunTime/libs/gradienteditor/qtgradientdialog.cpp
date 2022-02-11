/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtgradientdialog.h"
#include "ui_qtgradientdialog.h"
#include <QPushButton>
#include <QDialogButtonBox>

QtGradientDialogPrivate::QtGradientDialogPrivate():
    m_ui(new Ui::QtGradientDialog)
{

}

void QtGradientDialogPrivate::slotAboutToShowDetails(bool details, int extensionWidthHint)
{
    if (details) {
        q_ptr->resize(q_ptr->size() + QSize(extensionWidthHint, 0));
    } else {
        q_ptr->setMinimumSize(1, 1);
        q_ptr->resize(q_ptr->size() - QSize(extensionWidthHint, 0));
        q_ptr->setMinimumSize(0, 0);
    }
}

QtGradientDialog::QtGradientDialog(QWidget *parent)
    : QDialog(parent), d_ptr(new QtGradientDialogPrivate())
{
//    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    d_ptr->q_ptr = this;
    d_ptr->m_ui->setupUi(this);
    QPushButton *button = d_ptr->m_ui->buttonBox->button(QDialogButtonBox::Ok);
    if (button)
        button->setAutoDefault(false);
    button = d_ptr->m_ui->buttonBox->button(QDialogButtonBox::Cancel);
    if (button)
        button->setAutoDefault(false);
    connect(d_ptr->m_ui->gradientEditor, SIGNAL(aboutToShowDetails(bool,int)),
                this, SLOT(slotAboutToShowDetails(bool,int)));
}

/*!
    Destroys the gradient dialog
*/

QtGradientDialog::~QtGradientDialog()
{
}

/*!
    \property QtGradientDialog::gradient
    \brief the gradient of the dialog
*/
void QtGradientDialog::setGradient(const QGradient &gradient)
{
    d_ptr->m_ui->gradientEditor->setGradient(gradient);
}

QGradient QtGradientDialog::gradient() const
{
    return d_ptr->m_ui->gradientEditor->gradient();
}

/*!
    \property QtGradientDialog::backgroundCheckered
    \brief whether the background of widgets able to show the colors with alpha channel is checkered.

    \table
    \row
    \o \inlineimage qtgradientdialogbackgroundcheckered.png
    \o \inlineimage qtgradientdialogbackgroundtransparent.png
    \row
    \o \snippet doc/src/snippets/code/tools_shared_qtgradienteditor_qtgradientdialog.cpp 2
    \o \snippet doc/src/snippets/code/tools_shared_qtgradienteditor_qtgradientdialog.cpp 3
    \endtable

    When this property is set to true (the default) widgets inside gradient dialog like color button,
    color sliders, gradient stops editor and gradient editor will show checkered background
    in case of transparent colors. Otherwise the background of these widgets is transparent.
*/

bool QtGradientDialog::isBackgroundCheckered() const
{
    return d_ptr->m_ui->gradientEditor->isBackgroundCheckered();
}

void QtGradientDialog::setBackgroundCheckered(bool checkered)
{
    d_ptr->m_ui->gradientEditor->setBackgroundCheckered(checkered);
}

/*!
    \property QtGradientDialog::detailsVisible
    \brief whether details extension is visible.

    When this property is set to true the details extension is visible. By default
    this property is set to false and the details extension is hidden.

    \sa detailsButtonVisible
*/
bool QtGradientDialog::detailsVisible() const
{
    return d_ptr->m_ui->gradientEditor->detailsVisible();
}

void QtGradientDialog::setDetailsVisible(bool visible)
{
    d_ptr->m_ui->gradientEditor->setDetailsVisible(visible);
}

/*!
    \property QtGradientDialog::detailsButtonVisible
    \brief whether the details button allowing for showing and hiding details extension is visible.

    When this property is set to true (the default) the details button is visible and the user
    can show and hide details extension interactively. Otherwise the button is hidden and the details
    extension is always visible or hidded depending on the value of detailsVisible property.

    \sa detailsVisible
*/
bool QtGradientDialog::isDetailsButtonVisible() const
{
    return d_ptr->m_ui->gradientEditor->isDetailsButtonVisible();
}

void QtGradientDialog::setDetailsButtonVisible(bool visible)
{
    d_ptr->m_ui->gradientEditor->setDetailsButtonVisible(visible);
}

/*!
    Returns the current QColor::Spec used for the color sliders in the dialog.
*/
QColor::Spec QtGradientDialog::spec() const
{
    return d_ptr->m_ui->gradientEditor->spec();
}

/*!
    Sets the current QColor::Spec to \a spec used for the color sliders in the dialog.
*/
void QtGradientDialog::setSpec(QColor::Spec spec)
{
    d_ptr->m_ui->gradientEditor->setSpec(spec);
}


QGradient QtGradientDialog::getGradient(bool *ok, const QGradient &initial, QWidget *parent, const QString &caption)
{
    QtGradientDialog dlg(parent);
    if (!caption.isEmpty())
        dlg.setWindowTitle(caption);
    dlg.setGradient(initial);
    const int res = dlg.exec();
    if (ok) {
        *ok = (res == QDialog::Accepted) ? true : false;
    }
    if (res == QDialog::Accepted)
        return dlg.gradient();
    return initial;
}


QGradient QtGradientDialog::getGradient(bool *ok, QWidget *parent, const QString &caption)
{
    return getGradient(ok, QLinearGradient(), parent, caption);
}
