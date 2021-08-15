#ifndef QGRADIENTMAKE_H
#define QGRADIENTMAKE_H

#include "../../sharedlibglobal.h"

#include <QString>
#include <QGradient>
#include <QStringList>

class SHAREDLIB_EXPORT QGradientMaker
{
public:
    static QString styleSheetCode(const QGradient &gradient);

protected:
    static QString styleSheetFillName(const QGradient &gradient);
    static QStringList styleSheetStops(const QGradient &gradient);
    static QStringList styleSheetParameters(const QGradient &gradient);

};

#endif // QGRADIENTMAKE_H
