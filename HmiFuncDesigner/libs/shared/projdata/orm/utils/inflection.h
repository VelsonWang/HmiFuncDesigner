#ifndef INFLECTION_H
#define INFLECTION_H

#include <QtCore/QString>
#include <QtCore/QHash>


class Utils
{
public:
    /**
     * Convert to snake case
     */
    static QString snakeCase(const QString& name);

    /**
     * Take an underscored name and make it into a camelized name
     *
     * @example
     * classify("i_am_sam") returns "IAmSam"
     * classify("comment") returns "Comment"
     */
    static QString classify(const QString& name);

    /**
     * Convert a string to UpperCamelCase
     *
     */
    static QString camelize(const QString& name);

    /**
     * Capitalize a string
     *
     * @return the capitalized version of the string
     */
    static QString capitalize(const QString& name);

    /**
     * Convert a string into a table name
     *
     * @return a version of the string useful as a database table name
     */
    static QString tableize(const QString& s);

#include "inflection_p.h"
};

#endif

