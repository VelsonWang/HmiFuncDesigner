#ifndef ELEMENTLIBRARYLISTWIDGETITEM_H
#define ELEMENTLIBRARYLISTWIDGETITEM_H

#include <QListWidgetItem>

class ElementLibraryListWidgetItem : public QListWidgetItem
{
public:
    ElementLibraryListWidgetItem();

    void setFilePath(const QString &);
    QString getFilePath() const;

private:
    QString filePath;
};

#endif // ELEMENTLIBRARYLISTWIDGETITEM_H
