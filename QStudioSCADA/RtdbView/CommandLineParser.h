#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QString>
#include <QStringList>


class CommandLineParser
{
public:
    CommandLineParser(QString cmdline);
    QString mCmd;
    QStringList mArgs;
    int mNumber;
};

#endif // COMMANDLINEPARSER_H
