/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logmanager.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * Copyright 2007 Martin Heinrich
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

#ifndef LOG4QT_LOGMANAGER_H
#define LOG4QT_LOGMANAGER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QObject>

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include "log4qt/level.h"
#include "log4qt/logger.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	 
	class LoggerRepository;
	
	/*!
	 * \brief The class LogManager manages Logger in the default 
	 *        LoggerRepository.
	 * 
	 * The LogManager manages logger in a single Hierarchy instance. It 
	 * provides access to special logger over the logLogger(), qtLogger() 
	 * and rootLogger() member functions.
	 * 
	 * The LogManager is handling the initialisation on startup. The 
	 * initialisation procedure will first attempt to configure the package 
	 * based on environment variables. If the attempt fails it will check for 
	 * the existence of configuration files in several location. For detailed 
	 * description of the initialisation procedure see \ref Init 
	 * "Initialization procedure".
	 *
	 * Messages created by qDebug(), qWarning(), qCritical() and qFatal() can 
	 * be can be handled by the LogManager. By default the message handling 
	 * is disabled. It can be enabled by calling setHandleQtMessages(). Once
	 * enabled all messages are logged using the logger qtLogger().
	 * 
	 * The Log4Qt runtime version is accessible over version(). The macros 
     * \ref Log4Qt::LOG4QT_VERSION "LOG4QT_VERSION" and 
     * \ref Log4Qt::LOG4QT_VERSION_STR "LOG4QT_VERSION_STR" provide the 
     * compile time version.
     * 
	 * \note All the functions declared in this class are thread-safe.
	 */
	class LogManager
	{
	private:
	    LogManager();
	    LogManager(const LogManager &rOther); // Not implemented
	    virtual ~LogManager();
	    LogManager &operator=(const LogManager &rOther); // Not implemented
	        
	public:
		/*!
		 * Returns if the handling of messages created by calls to qDebug(), 
		 * qWarning(), qCritical() and qFatal() is activated.
		 *  
		 * \sa setHandleQtMessages()
		 */
	    static bool handleQtMessages();
	    
	    static LoggerRepository *loggerRepository();
	
		/*!
		 * Returns the logger used for logging internal messages. See 
		 * \ref LogLog "Logging within the package" for more details.
		 * 
		 * Calling this function is equivalent to calling logger("Log4Qt").
		 */
	    static Logger *logLogger();
	
		/*!
		 * Returns a pointer to the logger used for logging messages created by 
		 * calls to qDebug(), qWarning(), qCritical() and qFatal(). 
		 * 
		 * Calling this function is equivalent to calling logger("Qt").
		 * 
		 * \sa setHandleQtMessages()
		 */
	    static Logger *qtLogger();
	
	    static Logger *rootLogger();
	    static QList<Logger*> loggers();
	    static Level threshold();
	    static void setThreshold(Level level);
	    
		/*!
		 * Activates or deactivates the handling of messages created by calls 
		 * to qDebug(), qWarning(), qCritical() and qFatal() is activated.
		 * 
		 * If activated, a Qt message handler is installed. Messages are logged 
		 * using the logger returned by qtLogger(). For fatal messages the same 
		 * exit procedure is implemented as for qFatal().
		 * 
		 * The following mappping is used from QtMsgType to Level:
		 * 
	     * <table align="center" border="1" cellpadding="2" cellspacing="0" bordercolor="#84b0c7">
	     * <tr bgcolor="#d5e1e8">
	     * <th> &nbsp;&nbsp;&nbsp; QtMsgType &nbsp;&nbsp;&nbsp;</th>
	     * <th> %Level </th>
	     * </tr><tr>
	     * <td> QtDebugMsg </td>
	     * <td> Level::DEBUG_INT </td>
	     * </tr><tr bgcolor="#ffffff">
	     * <td> QtWarningMsg </td>
	     * <td> Level::WARN_INT </td>
	     * </tr><tr>
	     * <td> QtCriticalMsg </td>
	     * <td> Level::ERROR_INT </td>
	     * </tr><tr bgcolor="#ffffff">
	     * <td> QtFatalMsg </td>
	     * <td> Level::FATAL_INT </td>
	     * </tr><tr>
	     * <td> QtSystemMsg </td>
	     * <td> Level::TRACE_INT </td>
	     * </tr>
	     * </table>
		 *  
		 * The default value is false for not handling Qt messages.
		 * 
		 * \sa handleQtMessages(), qInstallMsgHandler(), qFatal()
		 */
	    static void setHandleQtMessages(bool handleQtMessages);
	
	    /*!
	     * Configures the logging for the package to its default behaviour.
	     * 
	     * The logger logLogger() is configured to be not additive. Messages 
	     * with the level Level::ERROR_INT and Level::FATAL_INT are written 
	     * to \c stderr using a ConsoleAppender. The remaining messages are 
	     * written to \c stdout using a second ConsoleAppender. The level is 
	     * read from the system environment or application settings using 
	     * InitialisationHelper::setting() with the key \c Debug. If a level
	     * value is found, but it is not a valid Level string, 
	     * Level::DEBUG_INT is used. If no level string is found 
	     * Level::ERROR_INT is used.
	     * 
	     * The function does not remove any appender from the logger 
	     * logLogger().
	     *
	     * \sa \ref LogLog "Logging within the package",
	     *     \ref Env "Environment Variables",
	     *     resetConfiguration(), InitialisationHelper::setting()
	     */
	    static void configureLogLogger();
	    
	    static bool exists(const char *pName);
	    // JAVA: void fireAddAppenderEvent(Logger *pLogger, Appender *pAppender);
	    
        /*!
         * Returns the LogManager instance.
         */
        static LogManager *instance();

        static Logger *logger(const QString &rName);
	    
	    /*!
	     * Reset all values contained in logger repository to their default.
	     * 
	     * All appenders are removed from all loggers. The loggers are handled
	     * in no particular order. The last loggers to be reset are qtLogger(),
	     * logLogger() and rootLogger() in that order.
	     * 
	     * The handling of messages created by calls to qDebug(), qWarning(),
	     * qCritical() and qFatal() is deactivated. 
	     * 
	     * The internal logging is initialised to its default bahaviour
	     * using configureLogLogger().
		 * 
		 * \sa LoggerRepository::resetConfiguration(), setHandleQtMessages(),
		 *     configureLogLogger() 
	     */
	    static void resetConfiguration();
	    
	    static void shutdown();

	    /*!
	     * Executes the default initialisation procedure of the package.
	     * 
	     * The function will test for the setting \c DefaultInitOverride in 
	     * the system environment and application settings using 
	     * \ref InitialisationHelper::setting(). If the value is present and 
	     * set to anything else then \c false, the initialisation is aborted.
	     * <br>
	     * The system environment and application settings are tested for the 
	     * setting \c Configuration. If it is found and it is a valid path to 
	     * a file, the package is configured with the file using 
	     * \ref PropertyConfigurator::doConfigure(const QString &, LoggerRepository *)
	     * "PropertyConfigurator::doConfigure()". If the setting 
	     * \c Configuration is not available and a QCoreApplication object is 
	     * present, the application settings are tested for a group 
	     * \c Log4Qt/Properties. If the group exists, the package is configured 
	     * with the setting using the 
	     * \ref PropertyConfigurator::doConfigure(const QSettings &r, LoggerRepository *)
	     * "PropertyConfiguratordoConfigure()". If neither a configuration 
	     * file nor configuration settings could be found, the current working
	     * directory is searched for the file \c "log4qt.properties". If it is
	     * found, the package is configured with the file using 
	     * \ref PropertyConfigurator::doConfigure(const QString &, LoggerRepository *)
	     * "PropertyConfigurator::doConfigure()".
	     * 
	     * \sa \ref Init "Initialization procedure",
	     *     \ref Env "Environment Variables",
	     *     InitialisationHelper::setting()
	     */
	    static void startup();

        /*!
         * Returns the version number of Log4Qt at run-time. This may be a 
         * different version than the version the application was compiled 
         * against.
         *
         * \sa \ref Log4Qt::LOG4QT_VERSION "LOG4QT_VERSION", 
         *     \ref Log4Qt::LOG4QT_VERSION_STR "LOG4QT_VERSION_STR" 

         */
        static const char* version();        
	    
	private:
		void doSetHandleQtMessages(bool handleQtMessages);
		void doConfigureLogLogger();
		void doStartup();
		void welcome();
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        static void qtMessageHandler(QtMsgType type,
                                     const char *pMessage);
#else
        static void qtMessageHandler(QtMsgType type,
                                     const QMessageLogContext &context,
                                     const QString &message);
#endif

	
	private:
	    mutable QMutex mObjectGuard;
	    LoggerRepository *mpLoggerRepository;
	    Logger *mpNullLogger;
	    bool mHandleQtMessages;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QtMsgHandler mOldQtMsgHandler;
#else
        QtMessageHandler mOldQtMsgHandler;
#endif
	    static LogManager *mspInstance;
	};
	
	
	/***************************************************************************
	 * Operators, Helper
	 ***************************************************************************/
	
	#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates LogManager
	 * 
	 * Writes all object member variables to the given debug stream \a rDebug and
	 * returns the stream.
	 *
	 * <tt>
	 * %LogManager(loggerrepository:Hierarchy(loggers:6 root-level:"DEBUG" 
	 *             root-appenders:0 log-level: "NULL" log-appenders:0 
	 *             qt-level: "NULL" qt-appenders:0 handleqtmessages: false )  
	 * </tt>
	 * \sa QDebug
	 */
	QDebug operator<<(QDebug debug, 
	                  const LogManager &rLogManager);
	#endif // QT_NO_DEBUG_STREAM
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline LoggerRepository *LogManager::loggerRepository()
    {   // QMutexLocker locker(&instance()->mObjectGuard); // Constant for object lifetime
	    return instance()->mpLoggerRepository;  }
	
	inline bool LogManager::handleQtMessages()
	{   // QMutexLocker locker(&instance()->mObjectGuard);  // Read/Write of bool is safe
        return instance()->mHandleQtMessages;   }
	
	inline Logger *LogManager::logLogger()
	{   // QMutexLocker locker(&instance()->mObjectGuard); // Constant for object lifetime
        return logger(QLatin1String("Log4Qt"));   }
	
	inline Logger *LogManager::qtLogger()
	{   // QMutexLocker locker(&instance()->mObjectGuard); // Constant for object lifetime
        return logger(QLatin1String("Qt"));    }
	
	inline void LogManager::setHandleQtMessages(bool handleQtMessages)
	{	instance()->doSetHandleQtMessages(handleQtMessages);	}
	
	inline void LogManager::configureLogLogger()
	{	instance()->doConfigureLogLogger();	}
	
	inline void LogManager::startup()
	{	instance()->doStartup();	}

} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::LogManager, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_LOGMANAGER_H
