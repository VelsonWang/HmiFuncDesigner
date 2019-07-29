#ifndef QTCSVREADER_H
#define QTCSVREADER_H

#include <QList>
#include <QTextCodec>

#include "qtcsv_global.h"

class QStringList;

namespace QtCSV
{
    class AbstractData;

    // Reader class is a file reader that work with csv-files. It needs an
    // absolute path to the csv-file that you are going to read.
    // Additionally you cant specify:
    // - a separator character (or string) that is used as values
    // separator in this csv-file. Comma (",") is usually used as separator.
    // - text delimiter character (or string) that enclose each element in a
    // row. Typical delimiter characters: none (""), quote ("'")
    // and double quotes ("\"").
    // - text codec.
    // Reader can save information to:
    // - QList<QStringList>, where each QStringList contains values of one row;
    // - AbstractData-based container class.
    class QTCSVSHARED_EXPORT Reader
    {
    public:
        // Read csv-file and save it's data as strings to QList<QStringList>
        static QList<QStringList> readToList(const QString& filePath,
                        const QString& separator = QString(","),
                        const QString& textDelimiter = QString("\""),
                        QTextCodec* codec = QTextCodec::codecForName("UTF-8"));

        // Read csv-file and save it's data to AbstractData-based container
        // class
        static bool readToData(const QString& filePath,
                        AbstractData& data,
                        const QString& separator = QString(","),
                        const QString& textDelimiter = QString("\""),
                        QTextCodec* codec = QTextCodec::codecForName("UTF-8"));
    };
}

#endif // QTCSVREADER_H
