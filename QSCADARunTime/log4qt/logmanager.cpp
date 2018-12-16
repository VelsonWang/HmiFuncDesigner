/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logmanager.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * changes:		Sep 2008, Martin Heinrich:
 * 				- Resolved compilation problem with Microsoft Visual Studio 2005
 *              Feb 2009, Martin Heinrich
 *              - Fixed VS 2008 unreferenced formal parameter warning by using 
 *                Q_UNUSED in operator<<.
 *
 *
 * Copyright 2007 - 2009 Martin Heinrich
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


#include "log4qt/logmanager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QMutex>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include "log4qt/consoleappender.h"
#include "log4qt/helpers/datetime.h"
#include "log4qt/helpers/initialisationhelper.h"
#include "log4qt/helpers/optionconverter.h"
#include "log4qt/hierarchy.h"
#include "log4qt/propertyconfigurator.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/varia/denyallfilter.h"
#include "log4qt/varia/levelrangefilter.h"


namespace Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/

    
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
    LOG4QT_DECLARE_STATIC_LOGGER(static_logger, Log4Qt::LogManager)
    LOG4QT_GLOBAL_STATIC(QMutex, singleton_guard)
    
    
	
	/**************************************************************************
	 * Class implementation: LogManager
	 **************************************************************************/
	
	
	LogManager::LogManager() :
	    mObjectGuard(QMutex::Recursive), // Recursive for doStartup() to call doConfigureLogLogger()
	    mpLoggerRepository(new Hierarchy()),
	    mHandleQtMessages(false),
	    mOldQtMsgHandler(0)
	{
	}
	
	
	LogManager::~LogManager()
	{
        static_logger()->warn("Unexpected destruction of LogManager");
        
        // doSetConfigureHandleQtMessages(false);
        // delete mpLoggerRepository;
	}
	
	
	Logger *LogManager::rootLogger()
	{
	    return instance()->mpLoggerRepository->rootLogger();
	}
	
	
	QList<Logger*> LogManager::loggers()
	{
	    return instance()->mpLoggerRepository->loggers();
	}
	
	
	Level LogManager::threshold()
	{
	    return instance()->mpLoggerRepository->threshold();
	}
	
	
	void LogManager::setThreshold(Level level)
	{
	    instance()->mpLoggerRepository->setThreshold(level);
	}
	
	
	bool LogManager::exists(const char *pName)
	{
	    return instance()->mpLoggerRepository->exists(QLatin1String(pName));
	}
	
	
    LogManager *LogManager::instance()
    {
        // Do not use LOG4QT_GLOBAL_STATIC. The LogManager is rather expensive 
        // to construct, an exit handler must be set and doStartup must be 
        // called.
        
        if (!mspInstance)
        {
            QMutexLocker locker(singleton_guard()); 
            if (!mspInstance)
            {
                mspInstance = new LogManager;
                // qAddPostRoutine(shutdown); 
                atexit(shutdown);
                mspInstance->doConfigureLogLogger();
                mspInstance->welcome();
                mspInstance->doStartup();
            }
        }
        return mspInstance;
    }
    
    
	Logger *LogManager::logger(const QString &rName)
	{
	    return instance()->mpLoggerRepository->logger(rName);
	}
	
	
	void LogManager::resetConfiguration()
	{
		setHandleQtMessages(false);
	    instance()->mpLoggerRepository->resetConfiguration();
	    configureLogLogger();
	}
	
	
    const char* LogManager::version()
    {
        return LOG4QT_VERSION_STR;
    }
    
    
	void LogManager::shutdown()
	{
	    instance()->mpLoggerRepository->shutdown();
	}
	
	
	void LogManager::doSetHandleQtMessages(bool handleQtMessages)
	{
	    QMutexLocker locker(&mObjectGuard);
	
	    if (instance()->mHandleQtMessages == handleQtMessages)
	        return;
	    
	    instance()->mHandleQtMessages = handleQtMessages;
	    if (instance()->mHandleQtMessages)
	    {
	        static_logger()->trace("Activate Qt message handling");
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            instance()->mOldQtMsgHandler = qInstallMsgHandler(qtMessageHandler);
#else
            instance()->mOldQtMsgHandler = qInstallMessageHandler(qtMessageHandler);
#endif
	    }
	    else
	    {
	        static_logger()->trace("Deactivate Qt message handling");
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            qInstallMsgHandler(instance()->mOldQtMsgHandler);
#else
            qInstallMessageHandler(instance()->mOldQtMsgHandler);
#endif
	    }
	}
	
	
	void LogManager::doConfigureLogLogger()
	{
		QMutexLocker locker(&instance()->mObjectGuard);
	    
		// Level
		QString value = InitialisationHelper::setting(QLatin1String("Debug"), 
		                                              QLatin1String("ERROR"));
	    logLogger()->setLevel(OptionConverter::toLevel(value, Level::DEBUG_INT));
		
		// Common layout
	    TTCCLayout *p_layout = new TTCCLayout();
	    p_layout->setName(QLatin1String("LogLog TTCC"));
	    p_layout->setContextPrinting(false);
	    p_layout->activateOptions();
	    
	    // Common deny all filter
	    Filter *p_denyall = new DenyAllFilter();
	    p_denyall->activateOptions();
	
	    // ConsoleAppender on stdout for all events <= INFO 
	    ConsoleAppender *p_appender;
	    LevelRangeFilter *p_filter;
	    p_appender = new ConsoleAppender(p_layout, ConsoleAppender::STDOUT_TARGET);
	    p_filter = new LevelRangeFilter();
	    p_filter->setNext(p_denyall);
	    p_filter->setLevelMin(Level::NULL_INT);
	    p_filter->setLevelMax(Level::INFO_INT);
	    p_filter->activateOptions();
	    p_appender->setName(QLatin1String("LogLog stdout"));
	    p_appender->addFilter(p_filter);
	    p_appender->activateOptions();
	    logLogger()->addAppender(p_appender);
	    
	    // ConsoleAppender on stderr for all events >= WARN
	    p_appender = new ConsoleAppender(p_layout, ConsoleAppender::STDERR_TARGET);
	    p_filter = new LevelRangeFilter();
	    p_filter->setNext(p_denyall);
	    p_filter->setLevelMin(Level::WARN_INT);
	    p_filter->setLevelMax(Level::OFF_INT);
	    p_filter->activateOptions();
	    p_appender->setName(QLatin1String("LogLog stderr"));
	    p_appender->addFilter(p_filter);
	    p_appender->activateOptions();
	    logLogger()->addAppender(p_appender);
	}
	
	
	void LogManager::doStartup()
	{
	    QMutexLocker locker(&instance()->mObjectGuard);
		
	    // Override
	    QString default_value = QLatin1String("false");
        QString value = InitialisationHelper::setting(QLatin1String("DefaultInitOverride"), 
                                                      default_value);
	    if (value != default_value)
	    {
	        static_logger()->debug("DefaultInitOverride is set. Aborting default initialisation");
	        return;
	    }
	    
	    // Configuration using setting Configuration
	    value = InitialisationHelper::setting(QLatin1String("Configuration")); 
	    if (QFile::exists(value))
	    {
            static_logger()->debug("Default initialisation configures from file '%1' specified by Configure", value);
	        PropertyConfigurator::configure(value);
	        return;
	    }
	    
        // Configuration using setting
	    if (QCoreApplication::instance()) 
	    {
            const QLatin1String log4qt_group("Log4Qt");
            const QLatin1String properties_group("Properties");
	        QSettings s;
	        s.beginGroup(log4qt_group);
	        if (s.childGroups().contains(properties_group))
	        {
	            const QString group(QLatin1String("Log4Qt/Properties"));
	            static_logger()->debug("Default initialisation configures from setting '%1/%2'", log4qt_group, properties_group);
	            s.beginGroup(properties_group);
	            PropertyConfigurator::configure(s);
	            return;
	        }
	    }

        // Configuration using default file
	    const QString default_file(QLatin1String("log4qt.properties"));
        if (QFile::exists(default_file))
        {
            static_logger()->debug("Default initialisation configures from default file '%1'", default_file);
            PropertyConfigurator::configure(default_file);
            return;
        }
        
        static_logger()->debug("Default initialisation leaves package unconfigured");
	}
	
	
    void LogManager::welcome()
    {
        static_logger()->info("Initialising Log4Qt %1",
                              QLatin1String(LOG4QT_VERSION_STR));
        
        // Debug: Info
        if (static_logger()->isDebugEnabled())
        {
            // Create a nice timestamp with UTC offset
            DateTime start_time = DateTime::fromMilliSeconds(InitialisationHelper::startTime());
            QString offset;
            {
                QDateTime utc = start_time.toUTC();
                QDateTime local = start_time.toLocalTime();
                QDateTime local_as_utc = QDateTime(local.date(), local.time(), Qt::UTC);
                int min = utc.secsTo(local_as_utc) / 60;
                if (min < 0)
                    offset += QLatin1Char('-');
                else
                    offset += QLatin1Char('+');
                min = abs(min);
                offset += QString::number(min / 60).rightJustified(2, QLatin1Char('0'));
                offset += QLatin1Char(':');
                offset += QString::number(min % 60).rightJustified(2, QLatin1Char('0'));
            }
            static_logger()->debug("Program startup time is %1 (UTC%2)",
                                   start_time.toString(QLatin1String("ISO8601")),
                                   offset);
            static_logger()->debug("Internal logging uses the level %1", 
                                   logLogger()->level().toString());
        }
        
        // Trace: Dump settings
        if (static_logger()->isTraceEnabled())
        {
            static_logger()->trace("Settings from the system environment:");
            QString entry;
            Q_FOREACH (entry, InitialisationHelper::environmentSettings().keys())
                static_logger()->trace("    %1: '%2'", 
                                       entry, 
                                       InitialisationHelper::environmentSettings().value(entry));
            
            static_logger()->trace("Settings from the application settings:");
            if (QCoreApplication::instance())
            {
                const QLatin1String log4qt_group("Log4Qt");
                const QLatin1String properties_group("Properties");
                static_logger()->trace("    %1:", log4qt_group);
                QSettings s;
                s.beginGroup(log4qt_group);
                Q_FOREACH (entry, s.childKeys())
                    static_logger()->trace("        %1: '%2'", 
                                           entry,
                                           s.value(entry).toString());
                static_logger()->trace("    %1/%2:", log4qt_group, properties_group);
                s.beginGroup(properties_group);
                    Q_FOREACH (entry, s.childKeys())
                        static_logger()->trace("        %1: '%2'", 
                                               entry,
                                               s.value(entry).toString());
            } else
                static_logger()->trace("    QCoreApplication::instance() is not available");
        }
    }
    
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    void LogManager::qtMessageHandler(QtMsgType type, const char *pMessage)
    {
        Level level;
        switch (type)
        {
            case QtDebugMsg:
                level = Level::DEBUG_INT;
                break;
            case QtWarningMsg:
                level = Level::WARN_INT;
                break;
            case QtCriticalMsg:
                level = Level::ERROR_INT;
                break;
            case QtFatalMsg:
                level = Level::FATAL_INT;
                break;
            default:
                level = Level::TRACE_INT;
        }
        instance()->qtLogger()->log(level, pMessage);

        // Qt fatal behaviour copied from global.cpp qt_message_output()
        // begin {

        if ((type == QtFatalMsg) ||
            ((type == QtWarningMsg) && (!qgetenv("QT_FATAL_WARNINGS").isNull())) )
        {
#if defined(Q_CC_MSVC) && defined(QT_DEBUG) && defined(_DEBUG) && defined(_CRT_ERROR)
            // get the current report mode
            int reportMode = _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
            _CrtSetReportMode(_CRT_ERROR, reportMode);
            int ret = _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, QT_VERSION_STR, pMessage);
            if (ret == 0  && reportMode & _CRTDBG_MODE_WNDW)
                return; // ignore
            else if (ret == 1)
                _CrtDbgBreak();
#endif

#if defined(Q_OS_UNIX) && defined(QT_DEBUG)
            abort(); // trap; generates core dump
#else
            exit(1); // goodbye cruel world
#endif
        }

        // } end
    }
#else
    void LogManager::qtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
    {
        Level level;
        switch (type)
        {
            case QtDebugMsg:
                level = Level::DEBUG_INT;
                break;
            case QtWarningMsg:
                level = Level::WARN_INT;
                break;
            case QtCriticalMsg:
                level = Level::ERROR_INT;
                break;
            case QtFatalMsg:
                level = Level::FATAL_INT;
                break;
            default:
                level = Level::TRACE_INT;
        }
        instance()->qtLogger()->log(level, message);

        // Qt fatal behaviour copied from global.cpp qt_message_output()
        // begin {

        if ((type == QtFatalMsg) ||
            ((type == QtWarningMsg) && (!qgetenv("QT_FATAL_WARNINGS").isNull())) )
        {
#if defined(Q_CC_MSVC) && defined(QT_DEBUG) && defined(_DEBUG) && defined(_CRT_ERROR)
            // get the current report mode
            int reportMode = _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
            _CrtSetReportMode(_CRT_ERROR, reportMode);
            int ret = _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, QT_VERSION_STR, message.toUtf8().constData());
            if (ret == 0  && reportMode & _CRTDBG_MODE_WNDW)
                return; // ignore
            else if (ret == 1)
                _CrtDbgBreak();
#endif

#if defined(Q_OS_UNIX) && defined(QT_DEBUG)
            abort(); // trap; generates core dump
#else
            exit(1); // goodbye cruel world
#endif
        }

        // } end
    }
#endif

	
	
	LogManager *LogManager::mspInstance = 0;
	
	
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug, const LogManager &rLogManager)
	{
		Q_UNUSED(rLogManager); // To avoid warning C4100 on VS 2008
	    QList<Logger *> loggers = rLogManager.loggers();
	    debug.nospace() << "LogManager("
	        << "loggerrepository:" << *rLogManager.loggerRepository()
	        << "log-level:" << rLogManager.logLogger()->level().toString()
	        << "log-appenders:" << rLogManager.logLogger()->appenders().count()
	        << "qt-level:" << rLogManager.qtLogger()->level().toString()
	        << "qt-appenders:" << rLogManager.qtLogger()->appenders().count()
	        << "handleqtmessages:" << rLogManager.handleQtMessages()
	        << ")";
	    return debug.space();
	}
#endif // QT_NO_DEBUG_STREAM
	
	
}  // namespace Log4Qt
