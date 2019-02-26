#ifndef XMLGRAPHPAHECONFIGWRITER_H
#define XMLGRAPHPAHECONFIGWRITER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QFile>
#include "GraphPage.h"

class XmlGraphPageConfigWriter : QObject
{
public:
    XmlGraphPageConfigWriter(QObject *parent = 0);
    void writeGraphPageConfig(QFile &file, GraphPage *screen);
    void writeLibraryConfig(QFile &file,GraphPage *screen);
};

#endif // XMLGRAPHPAHECONFIGWRITER_H
