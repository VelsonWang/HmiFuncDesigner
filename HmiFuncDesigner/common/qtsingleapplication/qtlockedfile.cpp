

#include "qtlockedfile.h"


QtLockedFile::QtLockedFile()
    : QFile()
{
#ifdef Q_OS_WIN
    wmutex = 0;
    rmutex = 0;
#endif
    m_lock_mode = NoLock;
}


QtLockedFile::QtLockedFile(const QString &name)
    : QFile(name)
{
#ifdef Q_OS_WIN
    wmutex = 0;
    rmutex = 0;
#endif
    m_lock_mode = NoLock;
}

bool QtLockedFile::open(OpenMode mode)
{
    if (mode & QIODevice::Truncate) {
        qWarning("QtLockedFile::open(): Truncate mode not allowed.");
        return false;
    }
    return QFile::open(mode);
}

bool QtLockedFile::isLocked() const
{
    return m_lock_mode != NoLock;
}

QtLockedFile::LockMode QtLockedFile::lockMode() const
{
    return m_lock_mode;
}

