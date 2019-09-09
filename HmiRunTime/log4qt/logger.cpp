/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logger.cpp
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


#include "log4qt/logger.h"

#include <QtCore/QDebug>
#include "log4qt/appenderskeleton.h"
#include "log4qt/varia/listappender.h"
#include "log4qt/loggingevent.h"
#include "log4qt/log4qt.h"
#include "log4qt/loggerrepository.h"
#include "log4qt/logmanager.h"


namespace Log4Qt
{


	/**************************************************************************
	 * Declarations
	 **************************************************************************/



	/**************************************************************************
	 * C helper functions
	 **************************************************************************/



	/**************************************************************************
	 * Class implementation: Logger
	 **************************************************************************/


	Logger::Logger(LoggerRepository* pLoggerRepository, Level level, const QString &rName, Logger *pParent) :
	    QObject(0),
#if QT_VERSION < QT_VERSION_CHECK(4, 4, 0)
	    mObjectGuard(),
#else
	    mObjectGuard(QReadWriteLock::Recursive),
#endif
	    mName(rName),
	    mpLoggerRepository(pLoggerRepository),
	    mAdditivity(true),
	    mAppenders(),
	    mLevel(level),
	    mpParent(pParent)
	{
	    Q_ASSERT_X(pLoggerRepository, "Logger::Logger()", "Construction of Logger with null LoggerRepository");

	    setObjectName(mName);
	}


	Logger::~Logger()
	{
        logger()->warn("Unexpected destruction of Logger");

        // QWriteLocker locker(&mObjectGuard);
        //
        // QMutableListIterator< LogObjectPtr<Appender> > i(mAppenders);
        // while (i.hasNext())
        // {
        //     i.next();
        //     i.remove();
        // }
	}


	QList<Appender *> Logger::appenders() const
	{
	    QReadLocker locker(&mObjectGuard);

	    QList<Appender *> result;
	    Appender *p_appender;
	    Q_FOREACH(p_appender, mAppenders)
	    	result << p_appender;
	    return result;
	}


	void Logger::setLevel(Level level)
	{
	    // QWriteLocker locker(&mObjectGuard); // Read/Write int is safe

	    if ((parentLogger() == 0) && (level == Level::NULL_INT))
	    {
	        logger()->warn("Invalid root logger level NULL_INT. Using DEBUG_INT instead");
	        level = Level::DEBUG_INT;
	    }
	    mLevel = level;
	}


	void Logger::addAppender(Appender *pAppender)
	{
        // Avoid deadlock:
	    // - Handle warnings, before write lock is aquired

        // Keep objects with a 0 reference count safe
        LogObjectPtr<Appender> p_appender = pAppender;

	    {
	        QReadLocker locker(&mObjectGuard);

	        if(!p_appender)
	        {
	            logger()->warn("Adding null Appender to Logger '%1'", name());
	            return;
	        }
	        if(mAppenders.contains(p_appender))
	        {
	            logger()->warn("Adding of duplicate appender '%2' to logger '%1'", name(), p_appender->name());
	            return;
	        }
	    }
	    {
	        QWriteLocker locker(&mObjectGuard);

	        if(mAppenders.contains(p_appender))
	            return;
	        mAppenders.append(p_appender);
	    }
	}


	Appender *Logger::appender(const QString &rName) const
	{
	    QReadLocker locker(&mObjectGuard);

	    Appender *p_appender;
	    Q_FOREACH(p_appender, mAppenders)
	        if (p_appender->name() == rName)
	            return p_appender;
	    return 0;
	}



	void Logger::callAppenders(const LoggingEvent &rEvent) const
	{
	    QReadLocker locker(&mObjectGuard);

	    Appender *p_appender;
	    Q_FOREACH(p_appender, mAppenders)
	        p_appender->doAppend(rEvent);
	    if (additivity() && (parentLogger() != 0))
	        parentLogger()->callAppenders(rEvent);
	}


	bool Logger::isAttached(Appender *pAppender) const
	{
	    QReadLocker locker(&mObjectGuard);

        // Keep objects with a 0 reference count safe
        LogObjectPtr<Appender> p_appender = pAppender;

	    return mAppenders.contains(p_appender);
	}


	void Logger::removeAllAppenders()
	{
        // Avoid deadlock:
        // - Only log warnings without having the write log aquired
        // - Hold a reference to all appenders so that the remove does not
        //   destruct the appender over the reference count while the write
        //   log is held. The appender may log messages.

	    logger()->trace("Removing all appenders from logger '%1'", name());

        QList< LogObjectPtr<Appender> > appenders;
        {
            QWriteLocker locker(&mObjectGuard);
        	QMutableListIterator< LogObjectPtr<Appender> > i(mAppenders);
            while (i.hasNext())
            {
                Appender *p_appender = i.next();
                ListAppender *p_listappender = qobject_cast<ListAppender*>(p_appender);
                if (p_listappender && p_listappender->configuratorList())
                    continue;
                else
                {
                    appenders << p_appender;
                    i.remove();
                }
            }
        }
        appenders.clear();
	}


	void Logger::removeAppender(Appender *pAppender)
	{
	    // Avoid deadlock:
	    // - Only log warnings without having the write log aquired
	    // - Hold a reference to the appender so that the remove does not
	    //   destruct the appender over the reference count while the write
	    //   log is held. The appender may log messages.

        LogObjectPtr<Appender> p_appender = pAppender;

        if(!p_appender)
	    {
	        logger()->warn("Request to remove null Appender from Logger '%1'", name());
	        return;
	    }
	    int n;
	    {
	        QWriteLocker locker(&mObjectGuard);

	        n = mAppenders.removeAll(p_appender);
	    }
	    if (n == 0)
	    {
	        logger()->warn("Request to remove Appender '%2', which is not part of Logger '%1' appenders", name(), p_appender->name());
	        return;
	    }
	}


	void Logger::removeAppender(const QString &rName)
	{
		Appender *p_appender = appender(rName);
		if (p_appender)
			removeAppender(p_appender);
	}


	Level Logger::effectiveLevel() const
	{
	    Q_ASSERT_X(LogManager::rootLogger()->level() != Level::NULL_INT, "Logger::effectiveLevel()", "Root logger level must not be NULL_INT");

	    QReadLocker locker(&mObjectGuard);

	    const Logger *p_logger = this;
	    while (p_logger->level() == Level::NULL_INT)
	        p_logger = p_logger->parentLogger();
	    return p_logger->level();
	}


	bool Logger::isEnabledFor(Level level) const
	{
	    if (mpLoggerRepository->isDisabled(level))
	        return false;
	    return (effectiveLevel() <= level);
	}


	Logger *Logger::logger(const QString &rName)
	{
	    return LogManager::logger(rName);
	}


	Logger *Logger::logger(const char *pName)
	{
	    return LogManager::logger(QLatin1String(pName));
	}


	Logger *Logger::rootLogger()
	{
	    return LogManager::rootLogger();
	}


#ifndef QT_NO_DEBUG_STREAM
	QDebug Logger::debug(QDebug &rDebug) const
	{
	    QReadLocker locker(&mObjectGuard);

	    QString parent_logger;
	    if (mpParent)
	        parent_logger = mpParent->name();

	    rDebug.nospace() << "Logger("
		    << "name:" << name() << " "
		    << "appenders:" << mAppenders.count() << " "
		    << "additivity:" << mAdditivity << " "
		    << mLevel
		    << "parentLogger:" << parent_logger
		    << ")";
	    return rDebug.space();
	}
#endif // QT_NO_DEBUG_STREAM


	void Logger::forcedLog(Level level, const QString &rMessage) const
	{
	    QReadLocker locker(&mObjectGuard);

	    LoggingEvent event(this, level, rMessage);
	    callAppenders(event);
	}



	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/


#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug, const Logger &rLogger)
	{
	    return rLogger.debug(debug);
	}
#endif // QT_NO_DEBUG_STREAM



} // namespace Log4Qt
