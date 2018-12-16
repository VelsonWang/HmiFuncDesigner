/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logging.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * changes:		Sep 2008, Martin Heinrich:
 * 				- Added a compile time version check for the Qt version
 * 				Jan 2009, Martin Heinrich:
 * 				- Updated documentation and version information for version 0.2
 * 				Feb 2009, Martin Heinrich:
 * 				- Updated version information for version 0.3
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

#ifndef LOG4QT_H
#define LOG4QT_H


/*!
 * \mainpage
 *
 * %Log4Qt is a C++ port of the Apache Software Foundation Log4j package
 * using the Trolltech Qt Framework.
 *
 * The documentation describes classes and methods that have been added or
 * changed compared to Log4j.
 *
 * The following sections are describing the implementation in more detail:
 * - \ref Changes "Differences to Log4j"
 * - \ref Ownership "Object ownership"
 * - \ref LogLog "Logging within the package"
 * - \ref Init "Initialization procedure"
 * - \ref Env "Environment Variables"
 * - \ref Undocumented "Undocumented functions"
 * - \ref Assumptions "Assumptions"
 *
 * \author Martin Heinrich
 * \version 0.3 (January 2009)
 *
 */

/*!
 * \page Changes Differences to Log4j
 *
 * The following fundamental differences exist between %Log4Qt and Log4j:
 *
 * - As a JAVA package Log4j does not have to manage object ownership and
 *   lifetime in the same way then it is required in C++. For details on
 *   how object ownership is handled see \ref Ownership "Object ownership".
 * - The package uses itself for its internal logging similar to Log4j 1.3.
 *   For details see \ref LogLog "Logging within the package".
 * - The configuration using system properties was replaced with a combination
 *   of environment variables and application settings. For details see
 *   \ref Env "Environment Variables".
 * - Custom levels are not supported.
 * - Multiple Logger Repositories are not supported
 *
 * The following classes have been changed:
 *
 * - \ref Log4Qt::AppenderSkeleton "AppenderSkeleton"
 *   - The procedure of checking, if logging is possible, originally used by
 *     \ref Log4Qt::WriterAppender "WriterAppender" was generalised and is used
 *     in \ref Log4Qt::AppenderSkeleton "AppenderSkeleton" and derived classes
 *     (\ref Log4Qt::AppenderSkeleton::checkEntryConditions() "checkEntryConditions()").
 *   - The \ref Log4Qt::AppenderSkeleton::doAppend() "doAppend()" member function will
 *     check the entry conditions by calling the sub-class specific
 *     \ref Log4Qt::AppenderSkeleton::checkEntryConditions() "checkEntryConditions()".
 *     If successful the sub-class specific
 *     \ref Log4Qt::AppenderSkeleton::append() "append()" function is called.
 *
 * - Configurator
 *   - Configure functions return a boolean indicating, if the configuration
 *     was successful.
 *   - Configure errors are accessible over
 *     \ref Log4Qt::ConfiguratorHelper::configureError()
 *     "ConfiguratorHelper::configureError()".
 *   - Watching for configuration file changes is a function performed
 *     centrally by the \ref Log4Qt::ConfiguratorHelper "ConfiguratorHelper".
 *     The class provides signals to notify on configuration change and errors.
 *   - The class \ref Log4Qt::PropertyConfigurator "PropertyConfigurator" was
 *     extended to be able to read configuration data from a QSettings object.
 *
 * - \ref Log4Qt::Level "Level"
 *   - A new value \ref Log4Qt::Level::NULL_INT "Level::NULL_INT" was
 *     introduced to indicate there is no level set.
 *
 * - \ref Log4Qt::Logger "Logger"
 *   - The method \ref Log4Qt::Logger::isEnabledFor() "isEnabledFor()"
 *     does also take the repository threshold into account.
 *   - Several overloaded convenience member function are available to log
 *     messages with arguments of different types.
 *   - Two macros, \ref Log4Qt::LOG4QT_DECLARE_STATIC_LOGGER "LOG4QT_DECLARE_STATIC_LOGGER"
 *     and \ref Log4Qt::LOG4QT_DECLARE_QCLASS_LOGGER "LOG4QT_DECLARE_QCLASS_LOGGER",
 *     allows retrieving and caching of a pointer to a logger object.
 *
 * - \ref Log4Qt::LogManager "LogManager"
 *   - A QtMessage handler can be installed via
 *     \ref Log4Qt::LogManager::setHandleQtMessages() "setHandleQtMessages()",
 *     to redirect all messages created by calls to qDebug(), qWarning(),
 *     qCritical() and qFatal() to a logger. The logger is named Qt and can be
 *     accessed using \ref Log4Qt::LogManager::qtLogger() "qtLogger()".
 *   - The initialisation procedure is available over a public method
 *     (\ref Log4Qt::LogManager::startup() "startup()").
 *   - The LogManager provides access to the logger used internally by the
 *     package (\ref Log4Qt::LogManager::logLogger() "logLogger()") and to
 *     its default initialisation procedure
 *     (\ref Log4Qt::LogManager::configureLogLogger() "configureLogLogger()").
 *
 * - \ref Log4Qt::WriterAppender "WriterAppender"
 *   - The class will call  \ref Log4Qt::WriterAppender::handleIoErrors()
 *     "handleIoErrors()" after all I/O operations. Sub-classes should
 *     re-implement the function to handle errors.
 *
 * The following classes have been added:
 *
 * - An additional appender class, \ref Log4Qt::DebugAppender "DebugAppender",
 *   was added. The class appends logging events to the platform specific debug
 *   output.
 * - Various helper class have been introduced:
 *   - \ref Log4Qt::ClassLogger "ClassLogger": The class ClassLogger provides
 *     logging for a QObject derived class.
 *   - \ref Log4Qt::ConfiguratorHelper "ConfiguratorHelper": The class
 *     ConfiguratorHelper provides a configuration file watch and last error
 *     for configurator classes.
 *   - \ref Log4Qt::DateTime "DateTime": The class DateTime provides extended
 *     functionality for QDateTime.
 *   - \ref Log4Qt::LogError "LogError": The class LogError represents an error.
 *   - \ref Log4Qt::Factory "Factory": The class Factory provides factories
 *     for Appender, Filter and Layout objects.
 *   - \ref Log4Qt::InitialisationHelper "InitialisationHelper": The class
 *     InitialisationHelper performs static initialisation tasks.
 *   - \ref Log4Qt::LogObject "LogObject": The class LogObject is the common
 *     base class for many classes in the package.
 *   - \ref Log4Qt::LogObjectPtr "LogObjectPtr": The class LogObjectPtr
 *     implements automatic reference counting for LogObject objects.
 *   - \ref Log4Qt::PatternFormatter "PatternFormatter": The class
 *     PatternFormatter formats a logging event based on a pattern string.
 *   - \ref Log4Qt::Properties "Properties": The class Properties implements a
 *     JAVA property hash.
 */

/*!
 * \page Ownership Object ownership
 *
 * In difference to the JAVA Log4j package %Log4Qt must manage ownership and
 * lifetime of the objects used. This is non trivial as objects are created
 * and used in different ways.
 *
 * In general an object can be created explicitly for example an application
 * may create Loggers, Appenders and Layouts during creation of a QApplication
 * object. But they can also be automatically created by the package on
 * startup using a \ref Log4Qt::PropertyConfigurator "PropertyConfigurator"
 * configuration file. Objects may also be created the one way and then used
 * the other. Object may be used by multiple other objects. A Layout for example
 * may be used by multiple Appenders. Objects are also created from multiple
 * threads. The creation may happen during static initialisation and the
 * deletion during static de-initialization.
 *
 * The parent child model used by QObject cannot be used to handle this. It
 * cannot automatically delete an object that is used by multiple others as
 * for example an Appender used by multiple Loggers. In addition to this
 * QObjects and their children must reside in the same thread. This would
 * either mean to impose restriction on how objects can be created or to move
 * objects to a specific thread.
 *
 * To allow an automatic deletion of not required objects the package
 * implements reference counting for Appenders, Layouts and Filters. The
 * reference counting is implemented in \ref Log4Qt::LogObject "LogObject",
 * which is used as a common base class. The reference count can be explicitly
 * changed using the methods \ref Log4Qt::LogObject::retain() "retain()" and
 * \ref Log4Qt::LogObject::release() "release()". Alternatively an auto pointer
 * is available \ref Log4Qt::LogObjectPtr "LogObjectPtr", which is used
 * throughout the package.
 *
 * The reference counting mechanism will test, if an object has a QObject
 * parent object set. If a parent is set, the object will not be deleted, if
 * the reference count reaches 0. This allows to mix the reference counted
 * paradigm with the QObject parent child one.
 *
 * The following example configures a logger and uses reference counting to
 * manage the ownership of objects.
 *
 * \code
 * // Create layout
 * TTCCLayout *p_layout = new TTCCLayout();
 *
 * // Create appender
 * ConsoleAppender *p_appender = new ConsoleAppender(p_layout, ConsoleAppender::STDOUT_TARGET);
 * p_appender->activateOptions();
 *
 * // Get logger
 * Logger *p_logger = Logger::logger("MyClass");
 * p_logger->addAppender(p_appender);
 *
 * // ...
 *
 * // Remove appender from Logger
 * p_logger->removeAllAppenders(); // p_appender and p_layout are deleted here
 * \endcode
 *
 * The following example configures a logger and uses QObject ownership of
 * objects.
 *
 * \code
 * QObject *p_parent = new MyObject;
 *
 * // Create objects
 * ConsoleAppender *p_appender = new ConsoleAppender(p_parent);
 * TTCCLayout *p_layout = new TTCCLayout(p_appender);
 *
 * // Configure appender
 * p_appender->setTarget(ConsoleAppender::STDOUT_TARGET);
 * p_appender->setLayout(p_layout);
 * p_appender->activateOptions();
 *
 * // Get logger
 * Logger *p_logger = Logger::logger("MyClass");
 * p_logger->addAppender(p_appender);
 *
 * // ...
 *
 * // Remove appender from Logger
 * p_logger->removeAllAppenders();
 *
 * delete p_parent; // p_appender and p_layout are deleted here
 * \endcode
 *
 * The following example shows how to use objects created on the stack.
 *
 * \code
 * {
 *     // Create layout
 *     TTCCLayout layout;
 *     layout.retain();
 *
 *     // Create appender
 *     ConsoleAppender appender(&layout, ConsoleAppender::STDOUT_TARGET);
 *     appender.retain();
 *     appender.activateOptions();
 *
 *     // Get logger
 *     Logger *p_logger = Logger::logger("MyClass");
 *     p_logger->addAppender(&appender);
 *
 *     // ...
 *
 *     // Remove appender from Logger
 *     p_logger->removeAllAppenders(); // Without retain() program crashes here
 *
 * } // p_appender and p_layout are deleted here
 * \endcode
 */

/*!
 * \page LogLog Logging within the package
 *
 * The package uses itself for logging similar to Log4j 1.3. This brings much
 * more flexibility over logging to stdout, stderr like in Log4j 1.2 using
 * logLog. It also enables the program to capture and handle errors raised by
 * the package.
 *
 * Using this approach introduces the issue of recursion. The following example
 * explains a situation where this happens. Let's say all logger are configured
 * to be additive and only the root logger has an appender set. The appender
 * is a \ref Log4Qt::FileAppender "FileAppender". During the logging of an
 * event an I/O error occurs. The \ref Log4Qt::FileAppender "FileAppender" logs
 * an event by itself using the logger %Log4Qt::FileAppender. The event is
 * passed to the root logger, which calls then the \ref Log4Qt::FileAppender
 * "FileAppender". This causes another I/O error, which is logged by
 * the \ref Log4Qt::FileAppender "FileAppender".
 *
 * To avoid an endless loop the appender will drop the event on a recursive
 * invocation. This check is done by \ref Log4Qt::AppenderSkeleton
 * "AppenderSkeleton" in \ref Log4Qt::AppenderSkeleton::doAppend()
 * "doAppend()".
 *
 * The problem only occurs, if a logger, appender, layout or filter log an
 * event while an event is appended. Neither the logger class nor any of the
 * layout or filter classes log events during appending of an event. Most of
 * the appender classes may log errors during appending. Only the
 * \ref Log4Qt::ListAppender "ListAppender" and
 * \ref Log4Qt::ListAppender "ConsoleAppender" are not logging events.
 *
 * The default configuration uses two \ref Log4Qt::ListAppender
 * "ConsoleAppender", one for stderr and one for stdout. No event will be
 * dropped, because no recursive invocations can occur.
 */

/*!
 * \page Init Initialization procedure
 *
 * The package is initialised in two stages. The first stage takes place during
 * static initialization. The second stage takes place when the
 * \ref Log4Qt::LogManager "LogManager" singleton is created.
 *
 * During static initialisation the \ref Log4Qt::InitialisationHelper
 * "InitialisationHelper" singleton is created . On construction it captures
 * the program startup time, reads the required values from the system
 * environment and registers the package types with the Qt type system.
 *
 * The \ref Log4Qt::LogManager "LogManager" singleton is created on first use.
 * The creation is usually triggered by the request for a \ref Log4Qt::Logger
 * "Logger" object. The call to \ref Log4Qt::Logger::logger()
 * "Logger::logger()" is passed through to \ref Log4Qt::LogManager::logger()
 * "LogManager::logger()". On creation the \ref Log4Qt::LogManager "LogManager"
 * creates a \ref Log4Qt::Hierarchy "Hierarchy" object as logger repository.
 *
 * After the singleton is created the logging of the package is configured to
 * its default by a call to \ref Log4Qt::LogManager::configureLogLogger()
 * "LogManager::configureLogLogger()". The logger
 * \ref Log4Qt::LogManager::logLogger() "logLogger()" is configured to be not
 * additive. Messages with the level \ref Log4Qt::Level::ERROR_INT
 * "Level::ERROR_INT" and \ref Log4Qt::Level::FATAL_INT "Level::FATAL_INT" are
 * written to \c stderr using a ConsoleAppender. The remaining messages are
 * written to \c stdout using a second ConsoleAppender. The level is read from
 * the system environment or application settings using
 * \ref Log4Qt::InitialisationHelper::setting()
 * "InitialisationHelper::setting()" with the key \c Debug. If a level value
 * is found, but it is not a valid Level string,
 * \ref Log4Qt::Level::DEBUG_INT "Level::DEBUG_INT" is used. If no level string
 * is found \ref Log4Qt::Level::ERROR_INT "Level::ERROR_INT" is used.
 *
 * Once the logging is configured the package is initialised by a call to
 * \ref Log4Qt::LogManager::startup() "LogManager::startup()". The function
 * will test for the setting \c DefaultInitOverride in the system environment
 * and application settings using \ref Log4Qt::InitialisationHelper::setting()
 * "InitialisationHelper::setting()". If the value is present and set to
 * anything else then \c false, the initialisation is aborted.<br>
 * The system environment and application settings are tested for the setting
 * \c Configuration. If it is found and it is a valid path to a file, the
 * package is configured with the file using
 * \ref Log4Qt::PropertyConfigurator::doConfigure(const QString &, LoggerRepository *)
 * "PropertyConfigurator::doConfigure()". If the setting \c Configuration is
 * not available and a QCoreApplication object is present, the application
 * settings are tested for a group \c Log4Qt/Properties. If the group exists,
 * the package is configured with the setting using the
 * \ref Log4Qt::PropertyConfigurator::doConfigure(const QSettings &r, LoggerRepository *)
 * "PropertyConfiguratordoConfigure()". If neither a configuration file nor
 * configuration settings could be found, the current working directory is
 * searched for the file \c "log4qt.properties". If it is found, the package
 * is configured with the file using
 * \ref Log4Qt::PropertyConfigurator::doConfigure(const QString &, LoggerRepository *)
 * "PropertyConfigurator::doConfigure()".
 *
 * The following example shows how to use application settings to initialise the
 * package.
 *
 * \code
 * # file: myapplication.h
 *
 * #include qapplication.h
 *
 * class MyApplication : public QApplication
 * {
 *     Q_OBJECT
 *
 * public:
 *     MyApplication();
 *     ~MyApplication();
 *     void setupLog4Qt();
 * }
 * \endcode
 * \code
 * # file: myapplication.cpp
 *
 * #include myapplication.h
 *
 * MyApplication::MyApplication(
 * {
 *     // Set Application data to allow Log4Qt initialisation to read the
 *     // correct values
 *     setApplicationName("MyApplication");
 *     setOrganisationName("MyOrganisation");
 *     setOrganizationDomain("www.myorganisation.com");
 *
 *     // Log first message, which initialises Log4Qt
 *     Log4Qt::Logger::logger("MyApplication")->info("Hello World");
 * }
 *
 * MyApplication::~MyApplication()
 * {
 * }
 *
 * void MyApplication::setupLog4Qt()
 * {
 *     QSettings s;
 *
 *     // Set logging level for Log4Qt to TRACE
 *     s.beginGroup("Log4Qt");
 *     s.setValue("Debug", "TRACE");
 *
 *     // Configure logging to log to the file C:/myapp.log using the level TRACE
 *     s.beginGroup("Properties");
 *     s.setValue("log4j.appender.A1", "org.apache.log4j.FileAppender");
 *     s.setValue("log4j.appender.A1.file", "C:/myapp.log");
 *     s.setValue("log4j.appender.A1.layout", "org.apache.log4j.TTCCLayout");
 *     s.setValue("log4j.appender.A1.layout.DateFormat", "ISO8601");
 *     s.setValue("log4j.rootLogger", "TRACE, A1");
 *
 *     // Settings will become active on next application startup
 * }
 * \endcode
 */

/*!
 * \page Env Environment Variables
 *
 * The package uses environment variables to control the initialization
 * procedure. The environment variables replace the system property entries
 * used by Log4j.
 *
 * For compability reasons the Log4j entry is recognised. Alternatively a
 * environment variable style Log4Qt form can be used. The following entries
 * are used:
 *
 * - LOG4QT_DEBUG<br>
 *   The variable controls the \ref Log4Qt::Level "Level" value for the
 *   logger \ref Log4Qt::LogManager::logLogger() "LogManager::logLogger()".
 *   If the value is a valid \ref Log4Qt::Level "Level" string, the level for
 *   the is set to the level. If the value is not a valid
 *   \ref Log4Qt::Level "Level" string, \ref Log4Qt::Level::DEBUG_INT
 *   "DEBUG_INT" is used. Otherwise \ref Log4Qt::Level::ERROR_INT "ERROR_INT"
 *   is used.
 *   - \ref Log4Qt::LogManager::configureLogLogger()
 *     "LogManager::configureLogLogger()"
 *
 * - LOG4QT_DEFAULTINITOVERRIDE<br>
 *   The variable controls the \ref Init "initialization procedure" performed
 *   by the \ref Log4Qt::LogManager "LogManager" on startup. If it is set to
 *   any other value then \c false the \ref Init "initialization procedure"
 *   is skipped.
 *   - \ref Log4Qt::LogManager::startup() "LogManager::startup()"
 *
 * - LOG4QT_CONFIGURATION<br>
 *   The variable specifies the configuration file used for initialising the
 *   package.
 *   - \ref Log4Qt::LogManager::startup() "LogManager::startup()"
 *
 * - LOG4QT_CONFIGURATORCLASS<br>
 *   The variable specifies the configurator class used for initialising the
 *   package.
 *
 * Environment variables are read during static initialisation on creation of
 * the \ref Log4Qt::InitialisationHelper "InitialisationHelper". They can be
 * accessed by calling \ref Log4Qt::InitialisationHelper::environmentSettings()
 * "InitialisationHelper::environmentSettings()".
 *
 * All settings can also be made in the application settings under the group
 * \c %Log4Qt. For example the environment variable \c LOG4QT_DEBUG is
 * equivalent to the setting \c Log4Qt/Debug. If an environment variable is
 * set it takes precedence over the application setting. Settings are only
 * used, if an QApplication object is available, when the
 * \ref Log4Qt::LogManager "LogManager" is
 * initialised (see \ref Log4Qt::InitialisationHelper::setting()
 * "InitialisationHelper::setting()" for details).
 */

/*!
 * \page Undocumented Undocumented functions
 *
 * In general it was tried to avoid the usage of undocumented features of Qt.
 * Nice to have features like for example Q_DECLARE_PRIVATE are not used. Only
 * features that would have been resulted in re-coding the same functionality
 * are used.
 *
 * - QT_WA: The macro is used to call Windows A/W functions
 *    - \ref Log4Qt::DebugAppender "DebugAppender"
 * - QBasicAtomicPointer: The class is used instead of QAtomicPointer, because
 *   it allows the initialisation as plain old data type.
 *    - \ref Log4Qt::LOG4QT_GLOBAL_STATIC "LOG4QT_GLOBAL_STATIC"
 *    - \ref Log4Qt::LOG4QT_IMPLEMENT_INSTANCE "LOG4QT_IMPLEMENT_INSTANCE"
 *    - \ref Log4Qt::LOG4QT_DECLARE_STATIC_LOGGER "LOG4QT_DECLARE_STATIC_LOGGER"
 * - Q_BASIC_ATOMIC_INITIALIZER: The macro is used to initialise QAtomicPointer
 *   objects as plain old data type.
 *    - \ref Log4Qt::LOG4QT_GLOBAL_STATIC "LOG4QT_GLOBAL_STATIC"
 *    - \ref Log4Qt::LOG4QT_IMPLEMENT_INSTANCE "LOG4QT_IMPLEMENT_INSTANCE"
 *    - \ref Log4Qt::LOG4QT_DECLARE_STATIC_LOGGER "LOG4QT_DECLARE_STATIC_LOGGER"
 */

/*!
 * \page Assumptions Assumptions
 *
 * The following assumptions are used throughout the package:
 *
 * - Reading / writing of bool or int is thread-safe, if declared volatile
 *   - \ref Log4Qt::ListAppender "ListAppender"
 *   - \ref Log4Qt::AppenderSkeleton "AppenderSkeleton"
 *   - \ref Log4Qt::ConsoleAppender "ConsoleAppender"
 *   - \ref Log4Qt::FileAppender "FileAppender"
 *   - \ref Log4Qt::Hierarchy "Hierarchy"
 *   - \ref Log4Qt::Level "Level"
 *   - \ref Log4Qt::Logger "Logger"
 *   - \ref Log4Qt::WriterAppender "WriterAppender"
 * - \ref Log4Qt::Layout::format() "Layout::format()" is implemented reentrant
 *   in all sub-classes.
 *   - \ref Log4Qt::AppenderSkeleton "AppenderSkeleton"
 * - Being able to use singleton objects during static de-initialization without
 *   order issues is more valuable then their destruction.
 *   - \ref Log4Qt::LogManager "LogManager"
 *   - \ref Log4Qt::LOG4QT_GLOBAL_STATIC "LOG4QT_GLOBAL_STATIC"
 *   - \ref Log4Qt::LOG4QT_IMPLEMENT_INSTANCE "LOG4QT_IMPLEMENT_INSTANCE"
 */

/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(4, 3, 0)
#	error "Log4Qt requires Qt version 4.3.0 or higher"
#endif


/******************************************************************************
 * Declarations
 ******************************************************************************/

/*!
 * \brief The namespace %Log4Qt encloses all parts of the package.
 */
namespace Log4Qt
{
    /*!
     * This macro expands a numeric value of the form 0xMMmmPP (MM = major,
     * mm = minor, PP = patch) that specifies Log4Qt's version number.
     * This is the version against which the application is compiled.
     *
     * \sa \ref Log4Qt::LOG4QT_VERSION_STR "LOG4QT_VERSION_STR",
     *     \ref Log4Qt::LogManager::version() "LogManager::version()"
     */
    #define LOG4QT_VERSION 0x000200

    /*!
     * The macro expands to a string that specifies the Log4Qt's version
     * number. This is the version against which the application is compiled.
     *
     * \sa \ref Log4Qt::LOG4QT_VERSION "LOG4QT_VERSION",
     *     \ref Log4Qt::LogManager::version() "LogManager::version()"
     */
    #define LOG4QT_VERSION_STR "0.3.0"

	enum ErrorCode
	{
		OK = 0,
	    // AppenderSkeleton, FileAppender, WriterAppender
	    APPENDER_ACTIVATE_MISSING_LAYOUT_ERROR,
        APPENDER_ACTIVATE_MISSING_WRITER_ERROR,
        APPENDER_ACTIVATE_MISSING_FILE_ERROR,
	    APPENDER_CLOSED_ERROR,
	    APPENDER_INVALID_PATTERN_ERROR,
	    APPENDER_NO_OPEN_FILE_ERROR,
	    APPENDER_NOT_ACTIVATED_ERROR,
	    APPENDER_OPENING_FILE_ERROR,
	    APPENDER_RENAMING_FILE_ERROR,
	    APPENDER_REMOVE_FILE_ERROR,
	    APPENDER_USE_INVALID_PATTERN_ERROR,
	    APPENDER_USE_MISSING_LAYOUT_ERROR,
	    APPENDER_USE_MISSING_WRITER_ERROR,
	    APPENDER_WRITING_FILE_ERROR,
        // Level
        LEVEL_INVALID_LEVEL_STRING,
        // Layouts, PatternFormatter
	    LAYOUT_EXPECTED_DIGIT_ERROR,
	    LAYOUT_OPTION_IS_NOT_INTEGER_ERROR,
	    LAYOUT_INTEGER_IS_NOT_POSITIVE_ERROR,
        // Logger
        LOGGER_INVALID_LEVEL_FOR_ROOT,
	    // PropertyConfigurator, OptionHandler
        CONFIGURATOR_OPENING_FILE_ERROR,
        CONFIGURATOR_READING_FILE_ERROR,
        CONFIGURATOR_INVALID_SUBSTITUTION_ERROR,
        CONFIGURATOR_INVALID_OPTION_ERROR,
        CONFIGURATOR_MISSING_APPENDER_ERROR,
        CONFIGURATOR_UNKNOWN_APPENDER_CLASS_ERROR,
        CONFIGURATOR_MISSING_LAYOUT_ERROR,
        CONFIGURATOR_UNKNOWN_LAYOUT_CLASS_ERROR,
        CONFIGURATOR_PROPERTY_ERROR,
        CONFIGURATOR_UNKNOWN_TYPE_ERROR
	};


	/******************************************************************************
	 * Operators, Helpers
	 ******************************************************************************/


	/******************************************************************************
	 * Inline
	 ******************************************************************************/


} // namespace Log4Qt


#endif // LOG4QT_H
