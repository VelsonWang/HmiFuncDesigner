#include "XmlGraphPageConfigWriter.h"

XmlGraphPageConfigWriter::XmlGraphPageConfigWriter(QObject *parent)
    : QObject(parent)
{
}

void XmlGraphPageConfigWriter::writeGraphPageConfig(QFile &file, GraphPage *graphPage) {

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("graphPage");
    writer.writeAttribute("filename",graphPage->getFileName());
    writer.writeAttribute("graphPageId",graphPage->getGraphPageId());
    writer.writeAttribute("priority",graphPage->getGraphPagePriority());
    writer.writeAttribute("width",QString::number(graphPage->getGraphPageWidth()));
    writer.writeAttribute("height",QString::number(graphPage->getGraphPageHeight()));
    writer.writeAttribute("background",graphPage->getGraphPageBackground().name());

    QListIterator <QGraphicsItem*> it(graphPage->items());

    while (it.hasNext()) {

        Figure *fig = static_cast<Figure*>(it.next());
        fig->writeAsXml(writer);
    }

    writer.writeEndElement();
    writer.writeEndDocument();
}

void XmlGraphPageConfigWriter::writeLibraryConfig(QFile &file, GraphPage *graphPage) {

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("Library");

    QListIterator <QGraphicsItem*> it(graphPage->selectedItems());

    while (it.hasNext()) {

        Figure *fig = static_cast<Figure*>(it.next());
        fig->writeAsXml(writer);
    }

    writer.writeEndElement();
    writer.writeEndDocument();
}
