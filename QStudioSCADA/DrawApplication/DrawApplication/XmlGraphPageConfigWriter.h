#ifndef XMLGRAPHPAHECONFIGWRITER_H
#define XMLGRAPHPAHECONFIGWRITER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QFile>
#include "GraphPage.h"

class XmlGraphPageConfigWriter : public QObject
{
public:
    XmlGraphPageConfigWriter(QObject *parent = 0);
    void writeGraphPageConfig(QFile &file, GraphPage *graphPage);
    void writeLibraryConfig(QFile &file, GraphPage *graphPage);
};

#endif // XMLGRAPHPAHECONFIGWRITER_H
