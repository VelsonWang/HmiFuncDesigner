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

#ifndef QTGRADIENTDIALOG_H
#define QTGRADIENTDIALOG_H

#include "gradientEditor_global.h"

#include <QDialog>

class QtGradientDialog;

namespace Ui
{
    class QtGradientDialog;
}

class QtGradientDialogPrivate
{
    QtGradientDialog *q_ptr;
    Q_DECLARE_PUBLIC(QtGradientDialog)
public:
    QtGradientDialogPrivate();
    void slotAboutToShowDetails(bool details, int extensionWidthHint);

    Ui::QtGradientDialog *m_ui;
};

class GRADIENTEDITORSHARED_EXPORT QtGradientDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QGradient gradient READ gradient WRITE setGradient)
    Q_PROPERTY(bool backgroundCheckered READ isBackgroundCheckered WRITE setBackgroundCheckered)
    Q_PROPERTY(bool detailsVisible READ detailsVisible WRITE setDetailsVisible)
    Q_PROPERTY(bool detailsButtonVisible READ isDetailsButtonVisible WRITE setDetailsButtonVisible)
public:
    QtGradientDialog(QWidget *parent = 0);
    ~QtGradientDialog();

    void setGradient(const QGradient &gradient);
    QGradient gradient() const;

    bool isBackgroundCheckered() const;
    void setBackgroundCheckered(bool checkered);

    bool detailsVisible() const;
    void setDetailsVisible(bool visible);

    bool isDetailsButtonVisible() const;
    void setDetailsButtonVisible(bool visible);

    QColor::Spec spec() const;
    void setSpec(QColor::Spec spec);

    static QGradient getGradient(bool *ok, const QGradient &initial, QWidget *parent = 0, const QString &caption = QString());
    static QGradient getGradient(bool *ok, QWidget *parent = 0, const QString &caption = QString());

private:
    QScopedPointer<class QtGradientDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtGradientDialog)
    Q_DISABLE_COPY(QtGradientDialog)
    Q_PRIVATE_SLOT(d_func(), void slotAboutToShowDetails(bool details, int extensionWidthHint))
};
#endif
