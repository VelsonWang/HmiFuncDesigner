#include "qgradientmaker.h"

QStringList QGradientMaker::styleSheetParameters(const QGradient &gradient)
{
    QStringList result;

    if (gradient.type() != QGradient::ConicalGradient) {
        QString spread;
        switch (gradient.spread()) {
            case QGradient::PadSpread:
                spread = QLatin1String("pad");
                break;
            case QGradient::ReflectSpread:
                spread = QLatin1String("reflect");
                break;
            case QGradient::RepeatSpread:
                spread = QLatin1String("repeat");
                break;
            default:
                break;
        }
        result << QLatin1String("spread:") + spread;
    }

    switch (gradient.type()) {
    case QGradient::LinearGradient: {
        const QLinearGradient *linearGradient = static_cast<const QLinearGradient*>(&gradient);
        result << QLatin1String("x1:") + QString::number(linearGradient->start().x())
                << QLatin1String("y1:")    + QString::number(linearGradient->start().y())
                << QLatin1String("x2:")    + QString::number(linearGradient->finalStop().x())
                << QLatin1String("y2:")    + QString::number(linearGradient->finalStop().y());
        break;
    }
    case QGradient::RadialGradient: {
        const QRadialGradient *radialGradient = static_cast<const QRadialGradient*>(&gradient);
        result << QLatin1String("cx:")  + QString::number(radialGradient->center().x())
                << QLatin1String("cy:")     + QString::number(radialGradient->center().y())
                << QLatin1String("radius:") + QString::number(radialGradient->radius())
                << QLatin1String("fx:")     + QString::number(radialGradient->focalPoint().x())
                << QLatin1String("fy:")     + QString::number(radialGradient->focalPoint().y());
        break;
    }
    case QGradient::ConicalGradient: {
        const QConicalGradient *conicalGradient = static_cast<const QConicalGradient*>(&gradient);
        result << QLatin1String("cx:") + QString::number(conicalGradient->center().x())
                << QLatin1String("cy:")    + QString::number(conicalGradient->center().y())
                << QLatin1String("angle:") + QString::number(conicalGradient->angle());
        break;
    }
    default:
        break;
    }

    return result;
}

QStringList QGradientMaker::styleSheetStops(const QGradient &gradient)
{
    QStringList result;
    foreach (const QGradientStop &stop, gradient.stops()) {
        const QColor color = stop.second;

        const QString stopDescription = QLatin1String("stop:") + QString::number(stop.first) + QLatin1String(" rgba(")
                + QString::number(color.red()) + QLatin1String(", ")
                + QString::number(color.green()) + QLatin1String(", ")
                + QString::number(color.blue()) + QLatin1String(", ")
                + QString::number(color.alpha()) + QLatin1Char(')');
        result << (stopDescription);
    }

    return result;
}


QString QGradientMaker::styleSheetFillName(const QGradient &gradient)
{
    QString result;

    switch (gradient.type()) {
        case QGradient::LinearGradient:
            result += QLatin1String("qlineargradient");
            break;
        case QGradient::RadialGradient:
            result += QLatin1String("qradialgradient");
            break;
        case QGradient::ConicalGradient:
            result += QLatin1String("qconicalgradient");
            break;
        default:
        break;
    }

    return result;
}

QString QGradientMaker::styleSheetCode(const QGradient &gradient)
{
    QString fillname = styleSheetFillName(gradient);

    QString ret;

    QStringList list = styleSheetParameters(gradient);

    ret = fillname + "(" + list.join(QLatin1String(", "));

    list = styleSheetStops(gradient);
    foreach(const QString &str, list) {
        ret += ("\n" + QString(25,' ') + str);
    }
    ret += ")";
    return ret;
}

