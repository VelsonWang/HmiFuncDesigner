#ifndef STYLESHEETHELPER_H
#define STYLESHEETHELPER_H

#include <QObject>


class StyleSheetHelper : public QObject
{
    Q_OBJECT

public:
    static void setStyle(const QString &style);

private:
    StyleSheetHelper(QObject *parent = NULL);
    ~StyleSheetHelper();
    Q_DISABLE_COPY(StyleSheetHelper)
};

#endif // STYLESHEETHELPER_H
