#include "elementlibrarylistwidgetitem.h"

ElementLibraryListWidgetItem::ElementLibraryListWidgetItem()
{
}

void ElementLibraryListWidgetItem::setFilePath(const QString &path) {

    filePath = path;
}

QString ElementLibraryListWidgetItem::getFilePath() const {

    return filePath;
}
