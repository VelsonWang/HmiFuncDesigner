#include "swatch.h"
#include <QPainter>
#include <QPixmapCache>

QPixmap colorSwatch(const QColor &color, const QSize &size)
{
    QString key = QString("COLORSWATCH:%1:%2x%3").arg(color.name())
        .arg(size.width()).arg(size.height());
    QPixmap pixmap(size);
#if QT_VERSION >= 0x040600
    if (!QPixmapCache::find(key, &pixmap)) {
#else
    if (!QPixmapCache::find(key, pixmap)) {
#endif
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHints(QPainter::Antialiasing|
                               QPainter::TextAntialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.drawEllipse(0, 0, size.width(), size.height());
        if (size.width() > 32) {
            painter.setFont(QFont("Helvetica",
                    qMax(8, size.height() - 6), QFont::Black));
            QColor textColor(color.darker());
            if (color.red() == color.green() &&
                color.red() == color.blue())
                textColor = (color.red() > 90 ? Qt::black
                                              : Qt::white);
            painter.setPen(textColor);
            painter.drawText(
                    QRectF(0, 0, size.width(), size.height()),
                    QObject::tr("C"), QTextOption(Qt::AlignCenter));
        }
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}
