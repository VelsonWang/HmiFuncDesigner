#include "reader.h"

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "abstractdata.h"
#include "filechecker.h"
#include "symbols.h"

using namespace QtCSV;

struct ElementInfo
{
    bool isEnded;

    ElementInfo() : isEnded(true) {}
};

class ReaderPrivate
{
public:
    // Function that really reads csv-file and save it's data as strings to
    // QList<QStringList>
    static bool read(const QString& filePath,
                     QList<QStringList>& list,
                     const QString& separator,
                     const QString& textDelimiter,
                     QTextCodec* codec);

private:
    // Check if file path and separator are valid
    static bool checkParams(const QString& filePath,
                            const QString& separator);

    // Split string to elements
    static QStringList splitElements(const QString& line,
                                     const QString& separator,
                                     const QString& textDelimiter,
                                     ElementInfo& elemInfo);

    // Try to find end position of first or middle element
    static int FindMiddleElementPositioin(const QString& str,
                                          const int& startPos,
                                          const QString& separator,
                                          const QString& txtDelim);

    // Check if current element is the last element
    static bool IsElementLast(const QString& str,
                              const int& startPos,
                              const QString& separator,
                              const QString& txtDelim);

    // Remove text delimiter symbols
    static QStringList removeTextDelimiters(const QStringList& elements,
                                            const QString& textDelimiter);
};

// Function that really reads csv-file and save it's data as strings to
// QList<QStringList>
// @input:
// - filePath - string with absolute path to csv-file
// - list - refernce to list container where read data will be saved
// - separator - string or character that separate values in a row
// - textDelimiter - string or character that enclose row elements
// - codec - pointer to codec object that would be used for file reading
// @output:
// - bool - result of read operation
bool ReaderPrivate::read(const QString& filePath,
                            QList<QStringList>& list,
                            const QString& separator,
                            const QString& textDelimiter,
                            QTextCodec* codec)
{
    if ( false == checkParams(filePath, separator) )
    {
        return false;
    }

    QFile csvFile(filePath);
    if ( false == csvFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        qDebug() << __FUNCTION__ << "Error - can't open file:" << filePath;
        return false;
    }

    QTextStream stream(&csvFile);
    stream.setCodec(codec);

    // This list will contain elements of the row if its elements
    // are located on several lines
    QStringList row;

    ElementInfo elemInfo;
    while ( false == stream.atEnd() )
    {
        QString line = stream.readLine();
        QStringList elements = ReaderPrivate::splitElements(
                  line, separator, textDelimiter, elemInfo);
        if (elemInfo.isEnded)
        {
            // Current row ends on this line. Check if these elements are
            // end elements of the long row
            if (row.isEmpty())
            {
                // No, these elements constitute the entire row
                list << elements;
            }
            else
            {
                // Yes, these elements should be added to the row
                if (false == elements.isEmpty())
                {
                    row.last().append(elements.takeFirst());
                    row << elements;
                }

                list << row;
                row.clear();
            }
        }
        else
        {
            // These elements constitute long row that lasts on several lines
            if (false == elements.isEmpty())
            {
                if (false == row.isEmpty())
                {
                    row.last().append(elements.takeFirst());
                }

                row << elements;
            }
        }
    }

    csvFile.close();

    if (false == elemInfo.isEnded && false == row.isEmpty())
    {
        list << row;
    }

    return true;
}

// Check if file path and separator are valid
// @input:
// - filePath - string with absolute path to csv-file
// - separator - string or character that separate values in a row
// @output:
// - bool - True if file path and separator are valid, otherwise False
bool ReaderPrivate::checkParams(const QString& filePath,
                                const QString& separator)
{
    if ( separator.isEmpty() )
    {
        qDebug() << __FUNCTION__ << "Error - separator could not be empty";
        return false;
    }

    if ( filePath.isEmpty() )
    {
        qDebug() << __FUNCTION__ << "Error - file path is empty";
        return false;
    }

    if ( false == CheckFile(filePath, true) )
    {
        qDebug() << __FUNCTION__ << "Error - wrong file path:" << filePath;
        return false;
    }

    return true;
}

// Split string to elements
// @input:
// - line - string with data
// - separator - string or character that separate elements
// - textDelimiter - string that is used as text delimiter
// @output:
// - QStringList - list of elements
QStringList ReaderPrivate::splitElements(const QString& line,
                                            const QString& separator,
                                            const QString& textDelimiter,
                                            ElementInfo& elemInfo)
{
    // If separator is empty, return whole line. Can't work in this
    // conditions!
    if (separator.isEmpty())
    {
        elemInfo.isEnded = true;
        return (QStringList() << line);
    }

    if (line.isEmpty())
    {
        // If previous row was ended, then return empty QStringList.
        // Otherwise return list that contains one element - new line symbols
        if (elemInfo.isEnded)
        {
            return QStringList();
        }
        else
        {
            return (QStringList() << LF);
        }
    }

    QStringList result;
    int pos = 0;
    while(pos < line.size())
    {
        if (elemInfo.isEnded)
        {
            // This line is a new line, not a continuation of the previous
            // line.
            // Check if element starts with the delimiter symbol
            int delimiterPos = line.indexOf(textDelimiter, pos);
            if (delimiterPos == pos)
            {
                pos = delimiterPos + textDelimiter.size();

                // Element starts with the delimiter symbol. It means that
                // this element could contain any number of double
                // delimiters and separator symbols. This element could:
                // 1. Be the first or the middle element. Then it should end
                // with delimiter and the seprator symbols standing next to each
                // other.
                int midElemEndPos = FindMiddleElementPositioin(
                                        line, pos, separator, textDelimiter);
                if (midElemEndPos > 0)
                {
                    int length = midElemEndPos - pos;
                    result << line.mid(pos, length);
                    pos = midElemEndPos +
                          textDelimiter.size() + separator.size();
                    continue;
                }

                // 2. Be The last element on the line. Then it should end with
                // delimiter symbol.
                if (IsElementLast(line, pos, separator, textDelimiter))
                {
                    int length = line.size() - textDelimiter.size() - pos;
                    result << line.mid(pos, length);
                    break;
                }

                // 3. Not ends on this line
                int length = line.size() - pos;
                result << line.mid(pos, length);
                elemInfo.isEnded = false;
                break;
            }
            else
            {
                // Element do not starts with the delimiter symbol. It means
                // that this element do not contain double delimiters and it
                // ends at the next separator symbol.
                // Check if line contains separator symbol.
                int separatorPos = line.indexOf(separator, pos);
                if (separatorPos >= 0 )
                {
                    // If line contains separator symbol, then our element
                    // located between current position and separator
                    // position. Copy it into result list and move
                    // current position over the separator position.
                    result << line.mid(pos, separatorPos - pos);
                    pos = separatorPos + separator.size();
                }
                else
                {
                    // If line do not contains separator symbol, then
                    // this element ends at the end of the string.
                    // Copy it into result list and exit the loop.
                    result << line.mid(pos);
                    break;
                }
            }
        }
        else
        {
            // This line is a continuation of the previous. Last element of the
            // previous line did not end. It started with delimiter symbol.
            // It means that this element could contain any number of double
            // delimiters and separator symbols. This element could:
            // 1. Ends somewhere in the middle of the line. Then it should ends
            // with delimiter and the seprator symbols standing next to each
            // other.
            int midElemEndPos = FindMiddleElementPositioin(
                                line, pos, separator, textDelimiter);
            if (midElemEndPos > 0)
            {
                result << (LF + line.mid(pos, midElemEndPos - pos));
                pos = midElemEndPos + textDelimiter.size() + separator.size();
                elemInfo.isEnded = true;
                continue;
            }

            // 2. Ends at the end of the line. Then it should ends with
            // delimiter symbol.
            if (IsElementLast(line, pos, separator, textDelimiter))
            {
                int length = line.size() - textDelimiter.size() - pos;
                result << (LF + line.mid(pos, length));
                elemInfo.isEnded = true;
                break;
            }

            // 3. Not ends on this line
            result << (LF + line);
            break;
        }
    }

    return removeTextDelimiters(result, textDelimiter);
}

// Try to find end position of first or middle element
// @input:
// - str - string with data
// - startPos - start position of the current element in the string
// - separator - string or character that separate elements
// - textDelimiter - string that is used as text delimiter
// @output:
// - int - end position of the element or -1 if this element is not first
// or middle
int ReaderPrivate::FindMiddleElementPositioin(const QString& str,
                                              const int& startPos,
                                              const QString& separator,
                                              const QString& txtDelim)
{
    const int ERROR = -1;
    if (str.isEmpty() ||
            startPos < 0 ||
            separator.isEmpty() ||
            txtDelim.isEmpty())
    {
        return ERROR;
    }

    const QString elemEndSymbols = txtDelim + separator;
    int elemEndPos = startPos;
    while(elemEndPos < str.size())
    {
        // Find position of element end symbol
        elemEndPos = str.indexOf(elemEndSymbols, elemEndPos);
        if (elemEndPos < 0)
        {
            // This element could not be the middle element, becaise string
            // do not contains any end symbols
            return ERROR;
        }

        // Check that this is really the end symbols of the
        // element and we don't mix up it with double delimiter
        // and separator. Calc number of delimiter symbols from elemEndPos
        // to startPos that stands together.
        int numOfDelimiters = 0;
        for (int pos = elemEndPos; startPos <= pos; --pos, ++numOfDelimiters)
        {
            QStringRef strRef = str.midRef(pos, txtDelim.size());
            if (QStringRef::compare(strRef, txtDelim) != 0)
            {
                break;
            }
        }

        // If we have odd number of delimiter symbols that stand together,
        // then this is the even number of double delimiter symbols + last
        // delimiter symbol. That means that we have found end position of
        // the middle element.
        if (numOfDelimiters % 2 == 1)
        {
            return elemEndPos;
        }
        else
        {
            // Otherwise this is not the end of the middle element and we
            // should try again
            elemEndPos += elemEndSymbols.size();
        }
    }

    return ERROR;
}

// Check if current element is the last element
// @input:
// - str - string with data
// - startPos - start position of the current element in the string
// - separator - string or character that separate elements
// - textDelimiter - string that is used as text delimiter
// @output:
// - bool - True if the current element is the last element of the string,
// False otherwise
bool ReaderPrivate::IsElementLast(const QString& str,
                                  const int& startPos,
                                  const QString& separator,
                                  const QString& txtDelim)
{
    if (str.isEmpty() ||
            startPos < 0 ||
            separator.isEmpty() ||
            txtDelim.isEmpty())
    {
        return false;
    }

    // Check if string ends with text delimiter. If not, then this element
    // do not ends on this line
    if (false == str.endsWith(txtDelim))
    {
        return false;
    }

    // Check that this is really the end symbols of the
    // element and we don't mix up it with double delimiter.
    // Calc number of delimiter symbols from end
    // to startPos that stands together.
    int numOfDelimiters = 0;
    for (int pos = str.size() - 1; startPos <= pos; --pos, ++numOfDelimiters)
    {
        QStringRef strRef = str.midRef(pos, txtDelim.size());
        if (QStringRef::compare(strRef, txtDelim) != 0)
        {
            break;
        }
    }

    // If we have odd number of delimiter symbols that stand together,
    // then this is the even number of double delimiter symbols + last
    // delimiter symbol. That means that this element is the last on the line.
    if (numOfDelimiters % 2 == 1)
    {
        return true;
    }

    return false;
}

// Remove text delimiter symbols
// @input:
// - elements - list of row elements
// - textDelimiter - string that is used as text delimiter
// @output:
// - QStringList - list of elements
QStringList ReaderPrivate::removeTextDelimiters(const QStringList& elements,
                                                const QString& textDelimiter)
{
    if (elements.isEmpty() || textDelimiter.isEmpty())
    {
        return elements;
    }

    QStringList result;
    const QString doubleTextDelim = textDelimiter + textDelimiter;
    for (int i = 0; i < elements.size(); ++i)
    {
        QString str = elements.at(i);
        if (str.startsWith(textDelimiter))
        {
            str.remove(0, textDelimiter.size());
        }

        if (str.endsWith(textDelimiter))
        {
            str.chop(textDelimiter.size());
        }

        // Also replace double text delimiter with one text delimiter symbol
        str.replace(doubleTextDelim, textDelimiter);
        result << str;
    }

    return result;
}

// Read csv-file and save it's data as strings to QList<QStringList>
// @input:
// - filePath - string with absolute path to csv-file
// - separator - string or character that separate elements in a row
// - textDelimiter - string or character that enclose each element in a row
// - codec - pointer to codec object that would be used for file reading
// @output:
// - QList<QStringList> - list of values (as strings) from csv-file. In case of
// error will return empty QList<QStringList>.
QList<QStringList> Reader::readToList(const QString& filePath,
                                      const QString& separator,
                                      const QString& textDelimiter,
                                      QTextCodec* codec)
{
    QList<QStringList> data;
    ReaderPrivate::read(filePath, data, separator, textDelimiter, codec);

    return data;
}

// Read csv-file and save it's data to AbstractData-based container class
// @input:
// - filePath - string with absolute path to csv-file
// - data - AbstractData object where all file content will be saved
// - separator - string or character that separate elements in a row
// - textDelimiter - string or character that enclose each element in a row
// - codec - pointer to codec object that would be used for file reading
// @output:
// - bool - True if file was successfully read, otherwise False
bool Reader::readToData(const QString& filePath,
                        AbstractData& data,
                        const QString& separator,
                        const QString& textDelimiter,
                        QTextCodec* codec)
{
    QList<QStringList> list;
    if (false == ReaderPrivate::read(filePath, list, separator, textDelimiter,
                                     codec))
    {
        return false;
    }

    for (int i = 0; i < list.size(); ++i)
    {
        data.addRow( list.at(i) );
    }

    return true;
}
