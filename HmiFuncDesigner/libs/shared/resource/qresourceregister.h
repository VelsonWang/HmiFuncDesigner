#ifndef QRESOURCEREGISTER_H
#define QRESOURCEREGISTER_H

#include "../sharedlibglobal.h"

#include <QObject>
#include <QMap>


class QResourceFile;

class SHAREDLIB_EXPORT QResourceRegister
{
public:
    static bool    reg(QResourceFile* file);
    static void    unreg();

signals:

public slots:

protected:
    static uchar*    m_data;

};

#endif // QRESOURCEREGISTER_H
