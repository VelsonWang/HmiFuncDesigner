/******************************************************************************
 *
 * package:     Log4Qt
 * file:        hierarchy.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * changes:		Sep 2008, Martin Heinrich:
 * 				- Fixed problem in Qt 4.4 where QReadWriteLock is by default
 *  			  non-recursive.
 *
 *
 * Copyright 2007 - 2008 Martin Heinrich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/



/******************************************************************************
 * Dependencies
 ******************************************************************************/


#include "log4qt/hierarchy.h"

#include <QtCore/QDebug>
#include "log4qt/logger.h"



namespace Log4Qt
{


	/**************************************************************************
	 * Declarations
	 **************************************************************************/



	/**************************************************************************
	 * C helper functions
	 **************************************************************************/


    LOG4QT_DECLARE_STATIC_LOGGER(static_logger, Log4Qt::LoggerRepository)



	/**************************************************************************
	 * Class implementation: Hierarchy
	 **************************************************************************/


	Hierarchy::Hierarchy() :
#if QT_VERSION < QT_VERSION_CHECK(4, 4, 0)
	    mObjectGuard(),
#else
	    mObjectGuard(QReadWriteLock::Recursive),
#endif
	    mLoggers(),
	    mThreshold(Level::NULL_INT),
	    mpRootLogger(logger(QString()))
	{
	    // Store root logger to allow rootLogger() to be const
	}


	Hierarchy::~Hierarchy()
	{
        static_logger()->warn("Unexpected destruction of Hierarchy");

        // QWriteLocker locker(&mObjectGuard);
	    //
        // resetConfiguration();
	    // clear();
	    // delete mpRootLogger;
	}


	bool Hierarchy::exists(const QString &rName) const
	{
	    QReadLocker locker(&mObjectGuard);

	    return mLoggers.contains(rName);
	}


	Logger *Hierarchy::logger(const QString &rName)
	{
	    QWriteLocker locker(&mObjectGuard);

	    return createLogger(rName);
	}


	QList<Logger *> Hierarchy::loggers() const
	{
	    QReadLocker locker(&mObjectGuard);

	    return mLoggers.values();
	}


	void Hierarchy::setThreshold(const QString &rThreshold)
	{
		setThreshold(Level::fromString(rThreshold));
	}


	void Hierarchy::resetConfiguration()
	{
        QWriteLocker locker(&mObjectGuard);

        // Reset all loggers.
	    // Leave log, qt and root logger to the last to allow debugging of shutdown.

	    Logger *p_logging_logger = logger(QLatin1String("Log4Qt"));
	    Logger *p_qt_logger = logger(QLatin1String("Qt"));
	    Logger *p_root_logger = rootLogger();

	    Logger *p_logger;
	    Q_FOREACH(p_logger, mLoggers)
	    {
	        if ((p_logger == p_logging_logger) || (p_logger == p_qt_logger) || (p_logger == p_root_logger))
	            continue;
	        resetLogger(p_logger, Level::NULL_INT);
	    }
	    resetLogger(p_qt_logger, Level::NULL_INT);
	    resetLogger(p_logging_logger, Level::NULL_INT);
	    resetLogger(p_root_logger, Level::DEBUG_INT);
	}


	void Hierarchy::shutdown()
	{
	    static_logger()->debug("Shutting down Hierarchy");
	    resetConfiguration();
	}


#ifndef QT_NO_DEBUG_STREAM
	QDebug Hierarchy::debug(QDebug &rDebug) const
	{
	    rDebug.nospace() << "Hierarchy("
            << "loggers:" << loggers().count() << " "
            << "threshold:" << threshold().toString() << " "
	        << "root-level:" << rootLogger()->level().toString() << " "
	        << "root-appenders:" << rootLogger()->appenders().count()
	        << ")";
	    return rDebug.space();
	}
#endif // QT_NO_DEBUG_STREAM


	Logger *Hierarchy::createLogger(const QString &rName)
	{
	    // Q_ASSERT_X(, "Hierarchy::createLogger", "Lock must be held by caller")

	    const QString name_separator = QLatin1String("::");

	    Logger *p_logger = mLoggers.value(rName, 0);
	    if (p_logger != 0)
	        return p_logger;

	    if (rName.isEmpty())
	    {
	        p_logger = new Logger(this, Level::DEBUG_INT, QLatin1String("root"), 0);
	        mLoggers.insert(QString(), p_logger);
	        return p_logger;
	    }
	    QString parent_name;
	    int index = rName.lastIndexOf(name_separator);
	    if (index >=0)
	        parent_name = rName.left(index);
	    p_logger = new Logger(this, Level::NULL_INT, rName, createLogger(parent_name));
	    mLoggers.insert(rName, p_logger);
	    return p_logger;
	}


	void Hierarchy::resetLogger(Logger *pLogger, Level level) const
	{
        // Q_ASSERT_X(, "Hierarchy::resetLogger", "Lock must be held by caller")

	    pLogger->removeAllAppenders();
	    pLogger->setAdditivity(true);
	    pLogger->setLevel(level);
	}



	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/



} // namespace Log4Qt
