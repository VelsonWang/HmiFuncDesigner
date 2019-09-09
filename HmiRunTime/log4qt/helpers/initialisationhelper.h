/******************************************************************************
 *
 * package:     Log4Qt
 * file:        initialisationhelper.h
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

#ifndef LOG4QT_HELPERS_INITIALISATIONHELPER_H
#define LOG4QT_HELPERS_INITIALISATIONHELPER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QAtomicPointer>
#include <QtCore/QHash>
#include <QtCore/QString>

#if QT_VERSION >= QT_VERSION_CHECK(4, 4, 0)
#	ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
#		warning "QAtomicPointer test and set is not native. The macros Log4Qt::LOG4QT_GLOBAL_STATIC and Log4Qt::LOG4QT_IMPLEMENT_INSTANCE are not thread-safe."
#	endif
#endif


/******************************************************************************
 * Declarations
 ******************************************************************************/

class QMutex;

namespace Log4Qt
{
    /*!
     * LOG4QT_GLOBAL_STATIC declares a static function \a FUNCTION that
     * returns a pointer to a singleton object of the type \a TYPE.
     *
     * The macro uses a static variable to store a pointer to the singleton
     * object. On the first invocation an object of the type \a TYPE is created
     * on the heap and the pointer is set. Any further invocations will return
     * the stored pointer. If multiple threads are accessing the function
     * without the pointer being set, each thread will create an object of the
     * type \a TYPE. The threads that find the pointer already been set will
     * delete their object. The singleton object will not be deleted during static
     * de-initialisation.
     *
     * The following example uses a global global mutex object to synchronise
     * access to a static member variable.
     *
     * \code
     * #file: myclass.h
     *
     * class MyClass
     * {
     * public:
     *     MyClass();
     *     ~MyClass();
     * private:
     *     static qint64 msObjectCount;
     * }
     * \endcode
     * \code
     * #file: myclass.cpp
     *
     * #include myclass.h
     *
     * LOG4QT_GLOBAL_STATIC(QMutex, class_guard)
     *
     * MyClass::MyClass()
     * {
     *     QMutexLocker(class_guard());
     *     msObjectCount++;
     * }
     *
     * MyClass::~MyClass()
     * {
     *     QMutexLocker(class_guard());
     *     msObjectCount--;
     * }
     *
     * qint64 MyClass::msObjectCount = 0;
     * \endcode
     *
     * \note The function created by the macro is thread-safe.
     *
     * \sa \ref Log4Qt::LOG4QT_IMPLEMENT_INSTANCE "LOG4QT_IMPLEMENT_INSTANCE",
     *     \ref Log4Qt::InitialisationHelper "InitialisationHelper"
     */
#if QT_VERSION < QT_VERSION_CHECK(4, 4, 0)
	#define LOG4QT_GLOBAL_STATIC(TYPE, FUNCTION)                              \
		static volatile TYPE *sp_global_static_##FUNCTION = 0;                \
		TYPE *FUNCTION()                                                      \
		{                                                                     \
			if (!sp_global_static_##FUNCTION)                                 \
			{                                                                 \
				TYPE *p_temp = new TYPE;                                      \
				if (!q_atomic_test_and_set_ptr(&sp_global_static_##FUNCTION,  \
											   0, p_temp))                    \
					delete p_temp;                                            \
			}                                                                 \
			return const_cast<TYPE *>(sp_global_static_##FUNCTION);           \
		}
#elif QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #define LOG4QT_GLOBAL_STATIC(TYPE, FUNCTION)                              \
        static QBasicAtomicPointer<TYPE > sp_global_static_##FUNCTION =       \
            Q_BASIC_ATOMIC_INITIALIZER(0);                                    \
        TYPE *FUNCTION()                                                      \
        {                                                                     \
            if (!sp_global_static_##FUNCTION)                                 \
            {                                                                 \
                TYPE *p_temp = new TYPE;                                      \
                if (!sp_global_static_##FUNCTION.testAndSetOrdered(0,         \
                                                                   p_temp))   \
                    delete p_temp;                                            \
            }                                                                 \
            return sp_global_static_##FUNCTION;                               \
        }
#else
    #define LOG4QT_GLOBAL_STATIC(TYPE, FUNCTION)                              \
		static QBasicAtomicPointer<TYPE > sp_global_static_##FUNCTION =       \
			Q_BASIC_ATOMIC_INITIALIZER(0);                                    \
        TYPE *FUNCTION()                                                      \
        {                                                                     \
            if (!sp_global_static_##FUNCTION.loadAcquire())                   \
            {                                                                 \
                TYPE *p_temp = new TYPE;                                      \
                if (!sp_global_static_##FUNCTION.testAndSetOrdered(0,         \
																   p_temp))   \
                    delete p_temp;                                            \
            }                                                                 \
            return sp_global_static_##FUNCTION.loadAcquire();                 \
        }
#endif

    /*!
     * LOG4QT_IMPLEMENT_INSTANCE implements an instance function for a
     * singleton class \a TYPE.
     *
     * The function works like the one created by
     * \ref Log4Qt::LOG4QT_GLOBAL_STATIC "LOG4QT_GLOBAL_STATIC".
     *
     * The following example illustrates how to use the macro to create a
     * singleton class:
     *
     * \code
     * #file: mysingleton.h
     *
     * class MySingleton
     * {
     * private:
     *     MySingleton();
     *     ~MySingleton();
     * public:
     *     MySingleton *instance();
     * }
     * \endcode
     * \code
     * #file: mysingleton.cpp
     *
     * #include mysingleton.h
     *
     * MySingleton::MySingleton()
     * {}
     *
     * MySingleton::~MySingleton()
     * {}
     *
     * LOG4QT_IMPLEMENT_INSTANCE(MySingleton)
     *
     * \endcode
     *
     * \note The function created by the macro is thread-safe.
     *
     * \sa \ref Log4Qt::LOG4QT_GLOBAL_STATIC "LOG4QT_GLOBAL_STATIC",
     *     \ref Log4Qt::InitialisationHelper "InitialisationHelper"
     */
#if QT_VERSION < QT_VERSION_CHECK(4, 4, 0)
	#define LOG4QT_IMPLEMENT_INSTANCE(TYPE)                                   \
		static TYPE *sp_singleton_##TYPE = 0;                                 \
		TYPE *TYPE::instance()                                                \
		{                                                                     \
			if (!sp_singleton_##TYPE)                                         \
			{                                                                 \
				TYPE *p_temp = new TYPE;                                      \
				if (!q_atomic_test_and_set_ptr(&sp_singleton_##TYPE,          \
											   0, p_temp))                    \
					 delete p_temp;                                           \
			}                                                                 \
			return sp_singleton_##TYPE;                                       \
		}
#elif QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #define LOG4QT_IMPLEMENT_INSTANCE(TYPE)                                   \
        static QBasicAtomicPointer<TYPE > sp_singleton_##TYPE =               \
            Q_BASIC_ATOMIC_INITIALIZER(0);                                    \
        TYPE *TYPE::instance()                                                \
        {                                                                     \
            if (!sp_singleton_##TYPE)                                         \
            {                                                                 \
                TYPE *p_temp = new TYPE;                                      \
                if (!sp_singleton_##TYPE.testAndSetOrdered(0, p_temp))        \
                    delete p_temp;                                            \
            }                                                                 \
            return sp_singleton_##TYPE;                                       \
        }
#else
    #define LOG4QT_IMPLEMENT_INSTANCE(TYPE)                                   \
		static QBasicAtomicPointer<TYPE > sp_singleton_##TYPE =               \
			Q_BASIC_ATOMIC_INITIALIZER(0);                                    \
        TYPE *TYPE::instance()                                                \
        {                                                                     \
            if (!sp_singleton_##TYPE.loadAcquire())                           \
            {                                                                 \
                TYPE *p_temp = new TYPE;                                      \
                if (!sp_singleton_##TYPE.testAndSetOrdered(0, p_temp))        \
                    delete p_temp;                                            \
            }                                                                 \
            return sp_singleton_##TYPE.loadAcquire();                         \
        }
#endif

	/*!
	 * \brief The class InitialisationHelper performs static initialisation
	 *        tasks.
	 *
	 * The InitialisationHelper is either created on the first call or through
	 * static initialisation. It will capture the programs startup time,
	 * which can be retrieved using startTime(). The system environment
	 * is analysed for package related definitions. The result is available
	 * over environmentSettings(). The packages custom types are registered with
	 * the Qt type system.
	 *
	 * Settings for the package can be retrieved using setting(). Two macros
	 * are available to help with the creation of singletons / global static
	 * objects (\ref Log4Qt::LOG4QT_GLOBAL_STATIC "LOG4QT_GLOBAL_STATIC" and
     * \ref Log4Qt::LOG4QT_IMPLEMENT_INSTANCE "LOG4QT_IMPLEMENT_INSTANCE").
	 *
	 * \note All the functions declared in this class are thread-safe.
	 *
	 * \sa \ref Init "Initialization procedure",
	 */
	class InitialisationHelper
	{
	private:
	    InitialisationHelper();
	    InitialisationHelper(const InitialisationHelper &rOther); // Not implemented
	    virtual ~InitialisationHelper();
	    InitialisationHelper &operator=(const InitialisationHelper &rOther); // Not implemented

	public:

        /*!
         * Returns a hash with the settings retrieved from the system
         * environment on startup.
         *
         * The following table shows the environment variables taken into
         * account and the setting key used for them.
         *
         * <table align="center" border="1" cellpadding="2" cellspacing="0" bordercolor="#84b0c7">
         * <tr bgcolor="#d5e1e8">
         * <th width="25%"> Environment variable </th>
         * <th width="25%"> Setting key </th>
         * </tr><tr>
         * <td> LOG4QT_DEBUG </td>
         * <td> Debug </td>
         * </tr><tr bgcolor="#ffffff">
         * <td> LOG4QT_DEFAULTINITOVERRIDE </td>
         * <td> DefaultInitOverride </td>
         * </tr><tr>
         * <td> LOG4QT_CONFIGURATION </td>
         * <td> Configuration </td>
         * </tr><tr bgcolor="#ffffff">
         * <td> LOG4QT_CONFIGURATORCLASS </td>
         * <td> ConfiguratorClass </td>
         * </tr>
         * </table>
         *
         * \sa \ref Env "Environment Variables",
         *     setting()
         */
        static QHash<QString, QString> environmentSettings();

        /*!
	     * Returns the InitialisationHelper instance.
	     */
	    static InitialisationHelper *instance();

        /*!
         * Returns the value for the setting \a rKey or \a rDefault, if it is
         * not defined.
         *
         * A setting can be either defined by an environment variable or by a
         * key in the application setting. The function will first test the
         * settings made by environment variables for the key \a rKey using
         * environmentSettings(). If the key is not present and a
         * QCoreApplication exists, the application settings are tested for
         * the key \a rKey in the group \c %Log4Qt.
         *
         * The following setting exists:
         *
         * <table align="center" border="1" cellpadding="2" cellspacing="0" bordercolor="#84b0c7">
         * <tr bgcolor="#d5e1e8">
         * <th width="25%"> Setting key </th>
         * <th> Description </th>
         * </tr><tr>
         * <td> Debug </td>
         * <td> The variable controls the Level value for the logger
         * LogManager::logLogger(). If the value is a valid Level string,
         * the level for the logger is set to the level. If the value is not
         * a valid Level string,  \ref Level::DEBUG_INT "DEBUG_INT" is used.
         * Otherwise \ref Level::ERROR_INT "ERROR_INT" is used. </td>
         * </tr><tr bgcolor="#ffffff">
         * <td> DefaultInitOverride </td>
         * <td> The variable controls the \ref Init "initialization procedure"
         * performed by the \ref LogManager "LogManager" on startup.
         * If it is set to any other value then \c false the \ref Init
         * "initialization procedure" is skipped.</td>
         * </tr><tr>
         * <td> Configuration </td>
         * <td> Specifies the configuration file used for initialising the package.</td>
         * </tr><tr bgcolor="#ffffff">
         * <td> ConfiguratorClass </td>
         * <td> Specifies the configurator class used for initialising the package.</td>
         * </tr>
         * </table>
         *
         * \sa environmentSettings(), \ref Env "Environment Variables",
         *     \ref Init "Initialization procedure",
         *     LogManager::configureLogLogger(), LogManager::startup()
         */
	    static QString setting(const QString &rKey,
                               const QString &rDefault = QString());

	    /*!
	     * Returns the start time of the program as the number of milliseconds
	     * that have passed since 1970-01-01T00:00:00,000, Coordinated
	     * Universal Time (Qt::UTC).
	     *
	     * \sa DateTime::fromMilliSeconds(),
	     *     DateTime::toMilliSeconds()
	     */
	    static qint64 startTime();

	private:
	    void doInitialiseEnvironmentSettings();
		void doRegisterTypes();
        QString doSetting(const QString &rKey,
                          const QString &rDefault) const;
        static bool shutdown();
        static bool staticInitialisation();

	private:
	    // QMutex mObjectGuard;
	    const qint64 mStartTime;
	    QHash <QString, QString> mEnvironmentSettings;
	    static bool msStaticInitialisation;

#ifndef QT_NO_DEBUG_STREAM
	    // Needs to be friend to access details
	    friend QDebug operator<<(QDebug debug,
                                 const InitialisationHelper &rInitialisationHelper);
#endif // QT_NO_DEBUG_STREAM
	};


	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/

#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates InitialisationHelper
	 *
	 * Writes all object member variables to the given debug stream \a rDebug and
	 * returns the stream.
	 *
	 * <tt>
	 * %InitialisationHelper(InitialisationHelper(starttime:1193883677438(
	 *                   QDateTime("Wed Oct 31 21:21:17 2007") )
	 *                   environmentsettings: QHash(("configuration", "\myapp.log4j")
	 *                   ("Debug", "DEBUG")) )  )
	 * </tt>
	 * \sa QDebug, InitialisationHelper::logManager()
	 */
	QDebug operator<<(QDebug debug,
	                  const InitialisationHelper &rInitialisationHelper);
#endif // QT_NO_DEBUG_STREAM


	/**************************************************************************
	 * Inline
	 **************************************************************************/

    inline QHash<QString, QString> InitialisationHelper::environmentSettings()
    {   // QMutexLocker locker(&instance()->mObjectGuard); // Constant for object lifetime
        return instance()->mEnvironmentSettings;  }

	inline QString InitialisationHelper::setting(const QString &rKey,
	                                             const QString &rDefault)
    {   // QMutexLocker locker(&instance()->mObjectGuard); // Reentrant and const
        return instance()->doSetting(rKey, rDefault);  }

	inline qint64 InitialisationHelper::startTime()
	{   // QMutexLocker locker(&instance()->mObjectGuard); // Constant for object lifetime
		return instance()->mStartTime;  }

} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::InitialisationHelper, Q_COMPLEX_TYPE); // use default


#endif // LOG4QT_HELPERS_INITIALISATIONHELPER_H
