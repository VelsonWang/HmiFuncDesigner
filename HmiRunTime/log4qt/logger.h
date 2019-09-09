/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logger.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * changes:		Sep 2008, Martin Heinrich:
 * 				- Replaced usage of q_atomic_test_and_set_ptr with
 * 				  QBasicAtomicPointer
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

#ifndef LOG4QT_LOGGER_H
#define LOG4QT_LOGGER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QObject>

#include <QtCore/QAtomicPointer>
#include <QtCore/QList>
#include <QtCore/QReadWriteLock>
#include "log4qt/helpers/logerror.h"
#include "log4qt/helpers/classlogger.h"
#include "log4qt/helpers/logobjectptr.h"
#include "log4qt/level.h"

#if QT_VERSION >= QT_VERSION_CHECK(4, 4, 0)
#	ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
#		warning "QAtomicPointer test and set is not native. The macro Log4Qt::LOG4QT_DECLARE_STATIC_LOGGER is not thread-safe."
#	endif
#endif


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{

	/*!
	 * LOG4QT_DECLARE_STATIC_LOGGER declares a static function \a FUNCTION that
	 * returns a pointer to a \ref Log4Qt::Logger "Logger" named after \a CLASS.
	 *
	 * On the first invocation the \ref Log4Qt::Logger "Logger" is requested
	 * by calling \ref Log4Qt::Logger::logger(const char *pName)
	 * "Logger::logger( #CLASS )". The pointer is stored to be returned on
	 * subsequent invocations.
	 *
	 * The following example shows how to use the macro to define a logger to be
	 * used within a class not derived from QObject.
	 *
	 * \code
	 * #file: counter.h
	 *
	 * #include logger.h
	 *
	 * class Counter
	 * {
	 * public:
	 *     Counter();
	 *     Counter(int preset);
	 * private:
	 *     int mCount;
	 * }
	 * \endcode
	 * \code
	 * #file: counter.cpp
	 *
	 * #include counter.h
	 *
	 * LOG4QT_DECLARE_STATIC_LOGGER(logger, Counter)
	 *
	 * Counter::Counter() :
	 *     mCount(0)
	 * {}
	 *
	 * void Counter::Counter(int preset) :
	 *     mCount(preset)
	 * {
	 *     if (preset < 0)
	 *     {
	 *         logger()->warn("Invalid negative counter preset %1. Using 0 instead.", preset);
	 *         mCount = 0;
	 *     }
	 * }
	 * \endcode
	 *
	 * \note The function created by the macro is thread-safe.
	 *
	 * \sa \ref Log4Qt::Logger::logger(const char *pName) "Logger::logger(const char *pName)"
	 */
#if QT_VERSION < QT_VERSION_CHECK(4, 4, 0)
	#define LOG4QT_DECLARE_STATIC_LOGGER(FUNCTION, CLASS)                     \
		static Log4Qt::Logger *FUNCTION()                                     \
		{                                                                     \
			static Log4Qt::Logger *p_logger = 0;                              \
				if (!p_logger)                                                \
				{                                                             \
					q_atomic_test_and_set_ptr(                                \
						&p_logger,                                            \
						0,                                                    \
						Log4Qt::Logger::logger( #CLASS ));                    \
				}                                                             \
			return p_logger;                                                  \
		}
#elif QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #define LOG4QT_DECLARE_STATIC_LOGGER(FUNCTION, CLASS)                     \
        static Log4Qt::Logger *FUNCTION()                                     \
        {                                                                     \
            static QBasicAtomicPointer<Log4Qt::Logger > p_logger =            \
                Q_BASIC_ATOMIC_INITIALIZER(0);                                \
            if (!p_logger)                                                    \
            {                                                                 \
                p_logger.testAndSetOrdered(0,                                 \
                    Log4Qt::Logger::logger( #CLASS ));                        \
            }                                                                 \
            return p_logger;                                                  \
        }
#else
	#define LOG4QT_DECLARE_STATIC_LOGGER(FUNCTION, CLASS)                     \
	    static Log4Qt::Logger *FUNCTION()                                     \
	    {                                                                     \
			static QBasicAtomicPointer<Log4Qt::Logger > p_logger =            \
				Q_BASIC_ATOMIC_INITIALIZER(0);                                \
            if (!p_logger.loadAcquire())                                      \
	        {                                                                 \
	        	p_logger.testAndSetOrdered(0,                                 \
					Log4Qt::Logger::logger( #CLASS ));                        \
            }                                                                 \
            return p_logger.loadAcquire();                                    \
	    }
#endif

	/*!
	 * LOG4QT_DECLARE_QCLASS_LOGGER declares member functions to retrieve
	 * \ref Log4Qt::Logger "Logger" for the class it is used in.
	 *
	 * On the first invocation the \ref Log4Qt::Logger "Logger" is requested
	 * by a call to \ref Log4Qt::Logger::logger(const char *pName)
	 * "Logger::logger(const char *pName)". The pointer is stored to be
	 * returned on subsequent invocations.
	 *
	 * The following example shows how to use the macro to define a logger to be
	 * used within a class derived from QObject.
	 *
	 * \code
	 * #file: counter.h
	 *
	 * #include qobject.h
	 * #include logger.h
	 *
	 * class Counter : public QObject
	 * {
	 *     Q_OBJECT
	 *     LOG4QT_DECLARE_QCLASS_LOGGER
	 * public:
	 *     Counter();
	 *     Counter(int preset);
	 * private:
	 *     int mCount;
	 * }
	 * \endcode
	 * \code
	 * #file: counter.cpp
	 *
	 * #include counter.h
	 *
	 * Counter::Counter() :
	 *     mCount(0)
	 * {}
	 *
	 * void Counter::Counter(int preset)
	 *     mCount(preset)
	 * {
	 *     if (preset < 0)
	 *     {
	 *         logger()->warn("Invalid negative counter preset %1. Using 0 instead.", preset);
	 *         mCount = 0;
	 *     }
	 * }
	 * \endcode
	 *
	 * \note The function created by the macro is thread-safe.
	 *
	 * \sa \ref Log4Qt::Logger::logger(const char *pName) "Logger::logger(const char *pName)",
	 *     \ref Log4Qt::ClassLogger "ClassLogger"
	 */
	#define LOG4QT_DECLARE_QCLASS_LOGGER                                      \
	    private:                                                              \
	        mutable Log4Qt::ClassLogger mLog4QtClassLogger;                   \
	    public:                                                               \
	        inline Log4Qt::Logger *logger() const                             \
	        {   return mLog4QtClassLogger.logger(this);    }                  \
	    private:

	class Appender;
	class LoggingEvent;
	class LoggerRepository;

	/*!
	 * \brief The class Logger provides logging services.
	 *
	 * A pointer to a logger can be retrieved by calling Logger::logger() or
	 * LogManager::logger() with the class name as argument. Because a logger
	 * is never destroyed it is possible to store the pointer to the logger.
	 * This way the lookup of the pointer in the repository is only required
	 * on the first logging operation. The macros \ref
	 * Log4Qt::LOG4QT_DECLARE_STATIC_LOGGER "LOG4QT_DECLARE_STATIC_LOGGER" and
	 * \ref Log4Qt::LOG4QT_DECLARE_QCLASS_LOGGER "LOG4QT_DECLARE_QCLASS_LOGGER"
	 * provide a thread-safe implementation to store the logger pointer.
	 *
	 * \note All the functions declared in this class are thread-safe.
	 */
	class Logger : public QObject
	{
	    Q_OBJECT

	    /*!
	     * The property holds, if the logger is additive.
	     *
	     * The default is true for being additive.
	     *
	     * \sa additive(), setAdditive()
	     */
	    Q_PROPERTY(bool additivity READ additivity WRITE setAdditivity)

	    /*!
	     * The property holds the level used by the logger.
	     *
	     * The default is Level::NULL_INT.
	     * \sa level(), setLevel()
	     */
	    Q_PROPERTY(Level level READ level WRITE setLevel)

	    /*!
	     * The property holds the LoggerRepository of the logger.
	     *
	     * \sa loggerRepository()
	     */
	    Q_PROPERTY(LoggerRepository* loggerRepository READ loggerRepository)

	    /*!
	     * The property holds the name of the logger.
	     *
	     * \sa name()
	     */
	    Q_PROPERTY(QString name READ name)

	    /*!
	     * The property holds the parent logger of the logger.
	     *
	     * \sa parentLogger()
	     */
	    Q_PROPERTY(Logger* parentLogger READ parentLogger)

	    LOG4QT_DECLARE_QCLASS_LOGGER

	protected:
	    Logger(LoggerRepository* pLoggerRepository, Level level, const QString &rName, Logger *pParent = 0);
	    virtual ~Logger();
	private:
	    Logger(const Logger &rOther); // Not implemented
	    Logger &operator=(const Logger &rOther); // Not implemented

	public:
	    bool additivity() const;
	    QList<Appender*> appenders() const;
	    Level level() const;
	    LoggerRepository *loggerRepository() const;
	    QString name() const;
	    Logger *parentLogger() const;
	    // JAVA: ResourceBundle *resourceBundle() const;
	    // JAVA: void setResourceBundle(ResourceBundle *pResourceBundle);
	    void setAdditivity(bool additivity);
	    virtual void setLevel(Level level);

	    void addAppender(Appender *pAppender);
	    Appender *appender(const QString &rName) const;
	    void callAppenders(const LoggingEvent &rEvent) const;
	    bool isAttached(Appender *pAppender) const;

	    /*!
	     * Removes all appenders that have been previously added from this
	     * Logger.
	     *
	     * To allow configurators to collect events during the configuration
	     * process ListAppenders with the configuratorList property set, will
	     * not be removed.
	     *
	     * \sa LisAppender::setConfiguratorList()
	     */
	    void removeAllAppenders();

	    void removeAppender(Appender *pAppender);
	    void removeAppender(const QString &rName);
	    // JAVA: QString resourceBundleString(const QString &rKey) const;

	    Level effectiveLevel() const;
	    bool isDebugEnabled() const;

		/*!
		 * Checks if this logger is enabled for a given Level \a level. If the
		 * logger is enabled the function returns true. Otherwise it returns
		 * false.
		 *
		 * A logger is enabled for a level, if the level is greater or equal
		 * then the repository threshold and greater and equal then the loggers
		 * effective level.
		 *
		 * \sa LoggerRepository::isDisabled(), effectiveLevel()
		 */
	    bool isEnabledFor(Level level) const;

	    bool isErrorEnabled() const;
	    bool isFatalEnabled() const;
	    bool isInfoEnabled() const;
	    bool isTraceEnabled() const;
	    bool isWarnEnabled() const;

	    void debug(const QString &rMessage) const;
	    void debug(const LogError &rLogError) const;
	    void debug(const char *pMessage) const;
	    void debug(const char *pMessage,
	               const QString &rArg1) const;
	    void debug(const char *pMessage,
	               int arg1) const;
	    void debug(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2) const;
	    void debug(const char *pMessage,
	               const QString &rArg1,
	               int arg2) const;
	    void debug(const char *pMessage,
	               int arg1,
	               const QString &rArg2) const;
	    void debug(const char *pMessage,
	               int arg1,
	               int arg2) const;
	    void debug(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2,
	               const QString &rArg3) const;
	    void debug(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2,
	               int arg3) const;
	    void debug(const char *pMessage,
	               const QString &rArg1,
	               int arg2,
	               const QString &rArg3) const;
	    void debug(const char *pMessage,
	               const QString &rArg1,
	               int arg2,
	               int arg3) const;
	    void debug(const char *pMessage,
	               int arg1,
	               const QString &rArg2,
	               const QString &rArg3) const;
	    void debug(const char *pMessage,
	               int arg1,
	               const QString &rArg2,
	               int arg3) const;
	    void debug(const char *pMessage,
	               int arg1,
	               int arg2,
	               const QString &rArg3) const;
	    void debug(const char *pMessage,
	               int arg1,
	               int arg2,
	               int arg3) const;
	    void debug(const char *pMessage,
	               const QVariant &rArg1,
	               const QVariant &rArg2,
	               const QVariant &rArg3) const;

	    void error(const QString &rMessage) const;
	    void error(const LogError &rLogError) const;
	    void error(const char *pMessage) const;
	    void error(const char *pMessage,
	               const QString &rArg1) const;
	    void error(const char *pMessage,
	               int arg1) const;
	    void error(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2) const;
	    void error(const char *pMessage,
	               const QString &rArg1, int arg2) const;
	    void error(const char *pMessage,
	               int arg1,
	               const QString &rArg2) const;
	    void error(const char *pMessage,
	               int arg1,
	               int arg2) const;
	    void error(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2,
	               const QString &rArg3) const;
	    void error(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2,
	               int arg3) const;
	    void error(const char *pMessage,
	               const QString &rArg1,
	               int arg2,
	               const QString &rArg3) const;
	    void error(const char *pMessage,
	               const QString &rArg1,
	               int arg2,
	               int arg3) const;
	    void error(const char *pMessage,
	               int arg1,
	               const QString &rArg2,
	               const QString &rArg3) const;
	    void error(const char *pMessage,
	               int arg1,
	               const QString &rArg2,
	               int arg3) const;
	    void error(const char *pMessage,
	               int arg1,
	               int arg2,
	               const QString &rArg3) const;
	    void error(const char *pMessage,
	               int arg1,
	               int arg2,
	               int arg3) const;
	    void error(const char *pMessage,
	               const QVariant &rArg1,
	               const QVariant &rArg2,
	               const QVariant &rArg3) const;

	    void fatal(const QString &rMessage) const;
	    void fatal(const LogError &rLogError) const;
	    void fatal(const char *pMessage) const;
	    void fatal(const char *pMessage,
	               const QString &rArg1) const;
	    void fatal(const char *pMessage,
	               int arg1) const;
	    void fatal(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2) const;
	    void fatal(const char *pMessage,
	               const QString &rArg1,
	               int arg2) const;
	    void fatal(const char *pMessage,
	               int arg1,
	               const QString &rArg2) const;
	    void fatal(const char *pMessage,
	               int arg1,
	               int arg2) const;
	    void fatal(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2,
	               const QString &rArg3) const;
	    void fatal(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2,
	               int arg3) const;
	    void fatal(const char *pMessage,
	               const QString &rArg1,
	               int arg2,
	               const QString &rArg3) const;
	    void fatal(const char *pMessage,
	               const QString &rArg1,
	               int arg2,
	               int arg3) const;
	    void fatal(const char *pMessage,
	               int arg1,
	               const QString &rArg2,
	               const QString &rArg3) const;
	    void fatal(const char *pMessage,
	               int arg1,
	               const QString &rArg2,
	               int arg3) const;
	    void fatal(const char *pMessage,
	               int arg1,
	               int arg2,
	               const QString &rArg3) const;
	    void fatal(const char *pMessage,
	               int arg1,
	               int arg2,
	               int arg3) const;
	    void fatal(const char *pMessage,
	               const QVariant &rArg1,
	               const QVariant &rArg2,
	               const QVariant &rArg3) const;

	    void info(const QString &rMessage) const;
	    void info(const LogError &rLogError) const;
	    void info(const char *pMessage) const;
	    void info(const char *pMessage,
	              const QString &rArg1) const;
	    void info(const char *pMessage,
	              int arg1) const;
	    void info(const char *pMessage,
	              const QString &rArg1,
	              const QString &rArg2) const;
	    void info(const char *pMessage,
	              const QString &rArg1,
	              int arg2) const;
	    void info(const char *pMessage,
	              int arg1,
	              const QString &rArg2) const;
	    void info(const char *pMessage,
	              int arg1,
	              int arg2) const;
	    void info(const char *pMessage,
	              const QString &rArg1,
	              const QString &rArg2,
	              const QString &rArg3) const;
	    void info(const char *pMessage,
	              const QString &rArg1,
	              const QString &rArg2,
	              int arg3) const;
	    void info(const char *pMessage,
	              const QString &rArg1,
	              int arg2,
	              const QString &rArg3) const;
	    void info(const char *pMessage,
	              const QString &rArg1,
	              int arg2,
	              int arg3) const;
	    void info(const char *pMessage,
	              int arg1,
	              const QString &rArg2,
	              const QString &rArg3) const;
	    void info(const char *pMessage,
	              int arg1,
	              const QString &rArg2,
	              int arg3) const;
	    void info(const char *pMessage,
	              int arg1,
	              int arg2,
	              const QString &rArg3) const;
	    void info(const char *pMessage,
	              int arg1,
	              int arg2,
	              int arg3) const;
	    void info(const char *pMessage,
	              const QVariant &rArg1,
	              const QVariant &rArg2,
	              const QVariant &rArg3) const;

	    void log(Level level,
	             const QString &rMessage) const;
	    void log(Level level,
	             const LogError &rLogError) const;
	    void log(Level level,
	             const char *pMessage) const;
	    void log(Level level,
	             const char *pMessage,
	             const QString &rArg1) const;
	    void log(Level level,
	             const char *pMessage,
	             int arg1) const;
	    void log(Level level,
	             const char *pMessage,
	             const QString &rArg1,
	             const QString &rArg2) const;
	    void log(Level level,
	             const char *pMessage,
	             const QString &rArg1,
	             int arg2) const;
	    void log(Level level,
	             const char *pMessage,
	             int arg1,
	             const QString &rArg2) const;
	    void log(Level level,
	             const char *pMessage,
	             int arg1,
	             int arg2) const;
	    void log(Level level,
	             const char *pMessage,
	             const QString &rArg1,
	             const QString &rArg2,
	             const QString &rArg3) const;
	    void log(Level level,
	             const char *pMessage,
	             const QString &rArg1,
	             const QString &rArg2,
	             int arg3) const;
	    void log(Level level,
	             const char *pMessage,
	             const QString &rArg1,
	             int arg2,
	             const QString &rArg3) const;
	    void log(Level level,
	             const char *pMessage,
	             const QString &rArg1,
	             int arg2,
	             int arg3) const;
	    void log(Level level,
	             const char *pMessage,
	             int arg1,
	             const QString &rArg2,
	             const QString &rArg3) const;
	    void log(Level level,
	             const char *pMessage,
	             int arg1,
	             const QString &rArg2,
	             int arg3) const;
	    void log(Level level,
	             const char *pMessage,
	             int arg1,
	             int arg2,
	             const QString &rArg3) const;
	    void log(Level level,
	             const char *pMessage,
	             int arg1,
	             int arg2,
	             int arg3) const;
	    void log(Level level,
	             const char *pMessage,
	             const QVariant &rArg1,
	             const QVariant &rArg2,
	             const QVariant &rArg3) const;

	    // JAVA: void l7dlog(Level level,
	    //                   const QString &rKey);
	    // JAVA: void l7dlog(Level level,
	    //                   const QString &rKey,
	    //                   const QList<Object *> rParameters);

	    void trace(const QString &rMessage) const;
	    void trace(const LogError &rLogError) const;
	    void trace(const char *pMessage) const;
	    void trace(const char *pMessage,
	               const QString &rArg1) const;
	    void trace(const char *pMessage,
	               int arg1) const;
	    void trace(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2) const;
	    void trace(const char *pMessage,
	               const QString &rArg1,
	               int arg2) const;
	    void trace(const char *pMessage,
	               int arg1,
	               const QString &rArg2) const;
	    void trace(const char *pMessage,
	               int arg1,
	               int arg2) const;
	    void trace(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2,
	               const QString &rArg3) const;
	    void trace(const char *pMessage,
	               const QString &rArg1,
	               const QString &rArg2,
	               int arg3) const;
	    void trace(const char *pMessage,
	               const QString &rArg1,
	               int arg2,
	               const QString &rArg3) const;
	    void trace(const char *pMessage,
	               const QString &rArg1,
	               int arg2,
	               int arg3) const;
	    void trace(const char *pMessage,
	               int arg1,
	               const QString &rArg2,
	               const QString &rArg3) const;
	    void trace(const char *pMessage,
	               int arg1,
	               const QString &rArg2,
	               int arg3) const;
	    void trace(const char *pMessage,
	               int arg1,
	               int arg2,
	               const QString &rArg3) const;
	    void trace(const char *pMessage,
	               int arg1,
	               int arg2,
	               int arg3) const;
	    void trace(const char *pMessage,
	               const QVariant &rArg1,
	               const QVariant &rArg2,
	               const QVariant &rArg3) const;

	    void warn(const QString &rMessage) const;
	    void warn(const LogError &rLogError) const;
	    void warn(const char *pMessage) const;
	    void warn(const char *pMessage,
	              const QString &rArg1) const;
	    void warn(const char *pMessage,
	              int arg1) const;
	    void warn(const char *pMessage,
	              const QString &rArg1,
	              const QString &rArg2) const;
	    void warn(const char *pMessage,
	              const QString &rArg1,
	              int arg2) const;
	    void warn(const char *pMessage,
	              int arg1,
	              const QString &rArg2) const;
	    void warn(const char *pMessage,
	              int arg1,
	              int arg2) const;
	    void warn(const char *pMessage,
	              const QString &rArg1,
	              const QString &rArg2,
	              const QString &rArg3) const;
	    void warn(const char *pMessage,
	              const QString &rArg1,
	              const QString &rArg2,
	              int arg3) const;
	    void warn(const char *pMessage,
	              const QString &rArg1,
	              int arg2,
	              const QString &rArg3) const;
	    void warn(const char *pMessage,
	              const QString &rArg1,
	              int arg2,
	              int arg3) const;
	    void warn(const char *pMessage,
	              int arg1,
	              const QString &rArg2,
	              const QString &rArg3) const;
	    void warn(const char *pMessage,
	              int arg1,
	              const QString &rArg2,
	              int arg3) const;
	    void warn(const char *pMessage,
	              int arg1,
	              int arg2,
	              const QString &rArg3) const;
	    void warn(const char *pMessage,
	              int arg1,
	              int arg2,
	              int arg3) const;
	    void warn(const char *pMessage,
	              const QVariant &rArg1,
	              const QVariant &rArg2,
	              const QVariant &rArg3) const;

	    // LogManager operations
	    static Logger *logger(const QString &rName);
	    static Logger *logger(const char *pName);
	    static Logger *rootLogger();

	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream \a rDebug
	     * and returns the stream.
	     *
	     * <tt>
	     * %Logger(name:"Log4Qt" appenders:0 additivity:true Level("NULL")
	     *         parentLogger: "root" )
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const Appender &rAppender)
	     */
		virtual QDebug debug(QDebug &rDebug) const;
	    friend QDebug operator<<(QDebug debug,
                                 const Logger &rLogger);
#endif // QT_NO_DEBUG_STREAM

	    void forcedLog(Level level,
	                   const QString &rMessage) const;

	protected:
	    mutable QReadWriteLock mObjectGuard;
	private:
		const QString mName;
	    LoggerRepository* mpLoggerRepository;
	    volatile bool mAdditivity;
	    QList< LogObjectPtr<Appender> > mAppenders;
	    Level mLevel;
	    Logger *mpParent;

	    // Needs to be friend to create Logger objects
	    friend class Hierarchy;
	};


	/******************************************************************************
	 * Operators, Helper
	 ******************************************************************************/

#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates Logger
	 *
	 * Writes all object member variables to the given debug stream \a debug and
	 * returns the stream.
	 *
	 * To handle subclassing the function uses the virtual member function debug().
	 * This allows each class to generate its own output.
	 *
	 * \sa QDebug, debug()
	 */
	QDebug operator<<(QDebug debug,
	                  const Logger &rLogger);
#endif // QT_NO_DEBUG_STREAM


	/**************************************************************************
	 * Inline
	 **************************************************************************/

	inline bool Logger::additivity() const
	{   // QReadLocker locker(&mObjectGuard); // Read/Write of int is safe
	    return mAdditivity; }

	inline Level Logger::level() const
	{   // QReadLocker locker(&mObjectGuard); // Level is thread-safe
	    return mLevel;  }

	inline LoggerRepository *Logger::loggerRepository() const
	{   // QReadLocker locker(&mObjectGuard); // Constant for object lifetime
	    return mpLoggerRepository;    }

	inline QString Logger::name() const
	{   // QReadLocker locker(&mObjectGuard); // Constant for object lifetime
	    return mName;    }

	inline  Logger *Logger::parentLogger() const
	{   // QReadLocker locker(&mObjectGuard); // Constant for object lifetime
	    return mpParent;    }

	inline void Logger::setAdditivity(bool additivity)
	{   // QWriteLocker locker(&mObjectGuard); // Read/Write of int is safe
	    mAdditivity = additivity;  }

	// Level operations

	inline bool Logger::isDebugEnabled() const
	{   return isEnabledFor(Level::DEBUG_INT);  }

	inline bool Logger::isErrorEnabled() const
	{   return isEnabledFor(Level::ERROR_INT);  }

	inline bool Logger::isFatalEnabled() const
	{   return isEnabledFor(Level::FATAL_INT);  }

	inline bool Logger::isInfoEnabled() const
	{   return isEnabledFor(Level::INFO_INT);  }

	inline bool Logger::isTraceEnabled() const
	{   return isEnabledFor(Level::TRACE_INT);  }

	inline bool Logger::isWarnEnabled() const
	{   return isEnabledFor(Level::WARN_INT);  }

	// Log operations: debug

	inline void Logger::debug(const LogError &rLogError) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, rLogError.toString());  }

	inline void Logger::debug(const QString &rMessage) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, rMessage);  }

	inline void Logger::debug(const char *pMessage) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage)); }

	inline void Logger::debug(const char *pMessage,
	                          const QString &rArg1) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(rArg1));  }

	inline void Logger::debug(const char *pMessage,
	                          int arg1) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(arg1));  }

	inline void Logger::debug(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2));  }

	inline void Logger::debug(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2));  }

	inline void Logger::debug(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2));  }

	inline void Logger::debug(const char *pMessage,
	                          int arg1,
	                          int arg2) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2));  }

	inline void Logger::debug(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2, rArg3));  }

	inline void Logger::debug(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2).arg(arg3));  }

	inline void Logger::debug(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(rArg3));  }

	inline void Logger::debug(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(arg3));  }

	inline void Logger::debug(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(rArg3));  }

	inline void Logger::debug(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(arg3));  }

	inline void Logger::debug(const char *pMessage,
	                          int arg1,
	                          int arg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(rArg3));  }

	inline void Logger::debug(const char *pMessage,
	                          int arg1,
	                          int arg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(arg3));  }

	inline void Logger::debug(const char *pMessage,
	                          const QVariant &rArg1,
	                          const QVariant &rArg2,
	                          const QVariant &rArg3) const
	{   if (isEnabledFor(Level::DEBUG_INT))
	    forcedLog(Level::DEBUG_INT, QString::fromUtf8(pMessage).arg(rArg1.toString(), rArg2.toString(), rArg3.toString()));  }

	// Log operations: error

	inline void Logger::error(const QString &rMessage) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, rMessage);  }

	inline void Logger::error(const LogError &rLogError) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, rLogError.toString());  }

	inline void Logger::error(const char *pMessage) const
	{   if (isEnabledFor(Level::ERROR_INT))
	        forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage)); }

	inline void Logger::error(const char *pMessage,
	                          const QString &rArg1) const
	{   if (isEnabledFor(Level::ERROR_INT))
	        forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(rArg1));  }

	inline void Logger::error(const char *pMessage,
	                          int arg1) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(arg1));  }

	inline void Logger::error(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2));  }

	inline void Logger::error(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2));  }

	inline void Logger::error(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2));  }

	inline void Logger::error(const char *pMessage,
	                          int arg1,
	                          int arg2) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2));  }

	inline void Logger::error(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2, rArg3));  }

	inline void Logger::error(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2).arg(arg3));  }

	inline void Logger::error(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(rArg3));  }

	inline void Logger::error(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(arg3));  }

	inline void Logger::error(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(rArg3));  }

	inline void Logger::error(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(arg3));  }

	inline void Logger::error(const char *pMessage,
	                          int arg1,
	                          int arg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(rArg3));  }

	inline void Logger::error(const char *pMessage,
	                          int arg1,
	                          int arg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(arg3));  }

	inline void Logger::error(const char *pMessage,
	                          const QVariant &rArg1,
	                          const QVariant &rArg2,
	                          const QVariant &rArg3) const
	{   if (isEnabledFor(Level::ERROR_INT))
	    forcedLog(Level::ERROR_INT, QString::fromUtf8(pMessage).arg(rArg1.toString(), rArg2.toString(), rArg3.toString()));  }

	// Log operations: fatal

	inline void Logger::fatal(const QString &rMessage) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, rMessage);  }

	inline void Logger::fatal(const LogError &rLogError) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, rLogError.toString());  }

	inline void Logger::fatal(const char *pMessage) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage)); }

	inline void Logger::fatal(const char *pMessage,
	                          const QString &rArg1) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(rArg1));  }

	inline void Logger::fatal(const char *pMessage,
	                          int arg1) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(arg1));  }

	inline void Logger::fatal(const char *pMessage,
	                          const QString &rArg1, const QString &rArg2) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2));  }

	inline void Logger::fatal(const char *pMessage,
	                          const QString &rArg1, int arg2) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2));  }

	inline void Logger::fatal(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2));  }

	inline void Logger::fatal(const char *pMessage,
	                          int arg1,
	                          int arg2) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2));  }

	inline void Logger::fatal(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2, rArg3));  }

	inline void Logger::fatal(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2).arg(arg3));  }

	inline void Logger::fatal(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(rArg3));  }

	inline void Logger::fatal(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(arg3));  }

	inline void Logger::fatal(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(rArg3));  }

	inline void Logger::fatal(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(arg3));  }

	inline void Logger::fatal(const char *pMessage,
	                          int arg1,
	                          int arg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(rArg3));  }

	inline void Logger::fatal(const char *pMessage,
	                          int arg1,
	                          int arg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(arg3));  }

	inline void Logger::fatal(const char *pMessage,
	                          const QVariant &rArg1,
	                          const QVariant &rArg2,
	                          const QVariant &rArg3) const
	{   if (isEnabledFor(Level::FATAL_INT))
	    forcedLog(Level::FATAL_INT, QString::fromUtf8(pMessage).arg(rArg1.toString(), rArg2.toString(), rArg3.toString()));  }

	// Log operations: info

	inline void Logger::info(const QString &rMessage) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, rMessage);  }

	inline void Logger::info(const LogError &rLogError) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, rLogError.toString());  }

	inline void Logger::info(const char *pMessage) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage)); }

	inline void Logger::info(const char *pMessage,
	                         const QString &rArg1) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(rArg1));  }

	inline void Logger::info(const char *pMessage,
	                         int arg1) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(arg1));  }

	inline void Logger::info(const char *pMessage,
	                         const QString &rArg1,
	                         const QString &rArg2) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2));  }

	inline void Logger::info(const char *pMessage,
	                         const QString &rArg1,
	                         int arg2) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2));  }

	inline void Logger::info(const char *pMessage,
	                         int arg1,
	                         const QString &rArg2) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2));  }

	inline void Logger::info(const char *pMessage,
	                         int arg1,
	                         int arg2) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2));  }

	inline void Logger::info(const char *pMessage,
	                         const QString &rArg1,
	                         const QString &rArg2,
	                         const QString &rArg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2, rArg3));  }

	inline void Logger::info(const char *pMessage,
	                         const QString &rArg1,
	                         const QString &rArg2,
	                         int arg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2).arg(arg3));  }

	inline void Logger::info(const char *pMessage,
	                         const QString &rArg1,
	                         int arg2,
	                         const QString &rArg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(rArg3));  }

	inline void Logger::info(const char *pMessage,
	                         const QString &rArg1,
	                         int arg2,
	                         int arg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(arg3));  }

	inline void Logger::info(const char *pMessage,
	                         int arg1,
	                         const QString &rArg2,
	                         const QString &rArg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(rArg3));  }

	inline void Logger::info(const char *pMessage,
	                         int arg1,
	                         const QString &rArg2,
	                         int arg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(arg3));  }

	inline void Logger::info(const char *pMessage,
	                         int arg1,
	                         int arg2,
	                         const QString &rArg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(rArg3));  }

	inline void Logger::info(const char *pMessage,
	                         int arg1,
	                         int arg2,
	                         int arg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(arg3));  }

	inline void Logger::info(const char *pMessage,
	                         const QVariant &rArg1,
	                         const QVariant &rArg2,
	                         const QVariant &rArg3) const
	{   if (isEnabledFor(Level::INFO_INT))
	    forcedLog(Level::INFO_INT, QString::fromUtf8(pMessage).arg(rArg1.toString(), rArg2.toString(), rArg3.toString()));  }

	// Log operations: log

	inline void Logger::log(Level level,
	                        const QString &rMessage) const
	{   if (isEnabledFor(level))
	    forcedLog(level, rMessage);  }

	inline void Logger::log(Level level,
	                        const LogError &rLogError) const
	{   if (isEnabledFor(level))
	    forcedLog(level, rLogError.toString());  }

	inline void Logger::log(Level level,
	                        const char *pMessage) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage)); }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        const QString &rArg1) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(rArg1));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        int arg1) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(arg1));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        const QString &rArg1,
	                        const QString &rArg2) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(rArg1, rArg2));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        const QString &rArg1, int arg2) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        int arg1,
	                        const QString &rArg2) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        int arg1,
	                        int arg2) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(arg1).arg(arg2));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        const QString &rArg1,
	                        const QString &rArg2,
	                        const QString &rArg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(rArg1, rArg2, rArg3));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        const QString &rArg1,
	                        const QString &rArg2,
	                        int arg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(rArg1, rArg2).arg(arg3));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        const QString &rArg1,
	                        int arg2,
	                        const QString &rArg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(rArg3));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        const QString &rArg1,
	                        int arg2,
	                        int arg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(arg3));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        int arg1,
	                        const QString &rArg2,
	                        const QString &rArg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(rArg3));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        int arg1,
	                        const QString &rArg2,
	                        int arg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(arg3));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        int arg1,
	                        int arg2,
	                        const QString &rArg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(rArg3));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        int arg1,
	                        int arg2,
	                        int arg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(arg3));  }

	inline void Logger::log(Level level,
	                        const char *pMessage,
	                        const QVariant &rArg1,
	                        const QVariant &rArg2,
	                        const QVariant &rArg3) const
	{   if (isEnabledFor(level))
	    forcedLog(level, QString::fromUtf8(pMessage).arg(rArg1.toString(), rArg2.toString(), rArg3.toString()));  }

	// Log operations: trace

	inline void Logger::trace(const QString &rMessage) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, rMessage);  }

	inline void Logger::trace(const LogError &rLogError) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, rLogError.toString());  }

	inline void Logger::trace(const char *pMessage) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage)); }

	inline void Logger::trace(const char *pMessage,
	                          const QString &rArg1) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(rArg1));  }

	inline void Logger::trace(const char *pMessage,
	                          int arg1) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(arg1));  }

	inline void Logger::trace(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2));  }

	inline void Logger::trace(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2));  }

	inline void Logger::trace(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2));  }

	inline void Logger::trace(const char *pMessage,
	                          int arg1,
	                          int arg2) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2));  }

	inline void Logger::trace(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2, rArg3));  }

	inline void Logger::trace(const char *pMessage,
	                          const QString &rArg1,
	                          const QString &rArg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2).arg(arg3));  }

	inline void Logger::trace(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(rArg3));  }

	inline void Logger::trace(const char *pMessage,
	                          const QString &rArg1,
	                          int arg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(arg3));  }

	inline void Logger::trace(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(rArg3));  }

	inline void Logger::trace(const char *pMessage,
	                          int arg1,
	                          const QString &rArg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(arg3));  }

	inline void Logger::trace(const char *pMessage,
	                          int arg1,
	                          int arg2,
	                          const QString &rArg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(rArg3));  }

	inline void Logger::trace(const char *pMessage,
	                          int arg1,
	                          int arg2,
	                          int arg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(arg3));  }

	inline void Logger::trace(const char *pMessage,
	                          const QVariant &rArg1,
	                          const QVariant &rArg2,
	                          const QVariant &rArg3) const
	{   if (isEnabledFor(Level::TRACE_INT))
	    forcedLog(Level::TRACE_INT, QString::fromUtf8(pMessage).arg(rArg1.toString(), rArg2.toString(), rArg3.toString()));  }

	// Log operations: warn

	inline void Logger::warn(const QString &rMessage) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, rMessage);  }

	inline void Logger::warn(const LogError &rLogError) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, rLogError.toString());  }

	inline void Logger::warn(const char *pMessage) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage)); }

	inline void Logger::warn(const char *pMessage,
	                         const QString &rArg1) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(rArg1));  }

	inline void Logger::warn(const char *pMessage,
	                         int arg1) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(arg1));  }

	inline void Logger::warn(const char *pMessage,
	                         const QString &rArg1,
	                         const QString &rArg2) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2));  }

	inline void Logger::warn(const char *pMessage,
	                         const QString &rArg1,
	                         int arg2) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2));  }

	inline void Logger::warn(const char *pMessage,
	                         int arg1,
	                         const QString &rArg2) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2));  }

	inline void Logger::warn(const char *pMessage,
	                         int arg1,
	                         int arg2) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2));  }

	inline void Logger::warn(const char *pMessage,
	                         const QString &rArg1,
	                         const QString &rArg2,
	                         const QString &rArg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2, rArg3));  }

	inline void Logger::warn(const char *pMessage,
	                         const QString &rArg1,
	                         const QString &rArg2,
	                         int arg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(rArg1, rArg2).arg(arg3));  }

	inline void Logger::warn(const char *pMessage,
	                         const QString &rArg1,
	                         int arg2,
	                         const QString &rArg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(rArg3));  }

	inline void Logger::warn(const char *pMessage,
	                         const QString &rArg1,
	                         int arg2,
	                         int arg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(rArg1).arg(arg2).arg(arg3));  }

	inline void Logger::warn(const char *pMessage,
	                         int arg1,
	                         const QString &rArg2,
	                         const QString &rArg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(rArg3));  }

	inline void Logger::warn(const char *pMessage,
	                         int arg1,
	                         const QString &rArg2,
	                         int arg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(arg1).arg(rArg2).arg(arg3));  }

	inline void Logger::warn(const char *pMessage,
	                         int arg1,
	                         int arg2,
	                         const QString &rArg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(rArg3));  }

	inline void Logger::warn(const char *pMessage,
	                         int arg1,
	                         int arg2,
	                         int arg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(arg1).arg(arg2).arg(arg3));  }

	inline void Logger::warn(const char *pMessage,
	                         const QVariant &rArg1,
	                         const QVariant &rArg2,
	                         const QVariant &rArg3) const
	{   if (isEnabledFor(Level::WARN_INT))
	    forcedLog(Level::WARN_INT, QString::fromUtf8(pMessage).arg(rArg1.toString(), rArg2.toString(), rArg3.toString()));  }


} // namespace Log4Qt


// Q_DECLARE_TYPEinfo(Log4Qt::Logger, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_LOGGER_H
