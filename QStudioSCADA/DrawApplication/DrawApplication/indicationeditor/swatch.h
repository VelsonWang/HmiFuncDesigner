#ifndef SWATCH_HPP
#define SWATCH_HPP
/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include <QPixmap>
#include <QSize>

class QColor;


static const QSize DefaultSwatchSize(24, 24);


QPixmap colorSwatch(const QColor &color,
                    const QSize &size=DefaultSwatchSize);
/*QPixmap brushSwatch(const Qt::BrushStyle style,
                    const QColor &color=Qt::black,
                    const QSize &size=DefaultSwatchSize);
QPixmap penStyleSwatch(const Qt::PenStyle style,
                       const QColor &color=Qt::black,
                       const QSize &size=DefaultSwatchSize);
QPixmap penCapSwatch(const Qt::PenCapStyle capStyle,
                     const QColor &color=Qt::black,
                     const QSize &size=DefaultSwatchSize);
QPixmap penJoinSwatch(const Qt::PenJoinStyle joinStyle,
                      const QColor &color=Qt::black,
                      const QSize &size=DefaultSwatchSize);*/


#endif // SWATCH_HPP
