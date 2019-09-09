#include "CommandLineParser.h"
#include <QDebug>

CommandLineParser::CommandLineParser(QString cmdline)
{
    //qDebug()<<"cmdline="<<cmdline;
    if(cmdline.endsWith(";"))
    {
        QStringList list1 = cmdline.split(' ');

        if(list1.count() > 0)
        {
            mCmd = list1.at(0);
        }
        QString argsline = list1.at(1);
        argsline = argsline.replace(";","");
        mArgs = argsline.split(',');
    }
    else
    {
        mCmd = "";
        mArgs.clear();
    }

    //qDebug()<<mCmd;
    //for(int i=0; i<mArgs.count(); i++)
    //   qDebug()<<mArgs.at(i);
}
