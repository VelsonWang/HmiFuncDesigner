#ifndef PUBLICDEFINE_H
#define PUBLICDEFINE_H

#include <QDomDocument>
#include <QString>


enum _GraphType
{
    GRAPH_NONE = 0,
    GRAPH_RECTANGLE,
    GRAPH_LINE
};

typedef _GraphType tGraphType;

const QString GraphTypeName[] = {
    "",
    "Rectangle", // GRAPH_RECTANGLE
    "Line" // GRAPH_LINE
};






#endif // IELEMENT_H

