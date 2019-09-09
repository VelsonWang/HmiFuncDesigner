/******************************************************************************
 *
 * package:     Log4Qt
 * file:        factory.h
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

#ifndef LOG4QT_HELPERS_FACTORY_H
#define LOG4QT_HELPERS_FACTORY_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QStringList>

QT_BEGIN_NAMESPACE
class QMetaProperty;
class QObject;
QT_END_NAMESPACE

/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	class Appender;
	class Filter;
	class Layout;
	    
	/*!
	 * \brief The class Factory provides factories for Appender, Filter and
	 *        Layout objects.
	 * 
	 * The functions createAppender(), createFilter() and createLayout() 
	 * allow to create objects by specifying their class names. By default
	 * all classes of the package are recognised with their Log4j and Log4Qt
	 * classanmes. For example an object of the class FileAppender can be
	 * craeted using "org.apache.log4j.FileAppender" or "Log4Qt::FileAppender".
	 * Additional classes can be registered using registerAppender(), 
	 * registerFilter() and registerLayout().
	 * 
	 * An QObject property can be set from a string value with
	 * setObjectProperty(). The function handles the required error checking
	 * and type conversion.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \sa PropertyConfigurator
	 */
	class Factory
	{
	public:
		/*!
	     * Prototype for an Appender factory function. The function creates 
	     * an Appender object on the heap and returns a pointer to it.
	     * 
	     * \sa registerAppender(), createAppender()
		 */
		typedef Appender *(*AppenderFactoryFunc)();
		
		/*!
	     * Prototype for a Filter factory function. The function creates 
	     * a Filter object on the heap and returns a pointer to it.
	     * 
	     * \sa registerFilter(), createFilter()
		 */
		typedef Filter *(*FilterFactoryFunc)();
		
		/*!
	     * Prototype for a Layout factory function. The function creates 
	     * a Layout object on the heap and returns a pointer to it.
	     * 
	     * \sa registerLayout(), createLayout()
		 */
		typedef Layout *(*LayoutFactoryFunc)();
		
	private:
	    Factory();
        Q_DISABLE_COPY(Factory)
	
	public:
		/*!
		 * Creates an object for the class \a rAppenderClassName on the heap 
		 * and returns a pointer to it. If the class has no registered factory 
		 * function a null pointer is returned.
		 * 
		 * \sa registerAppender(), unregisterAppender(), registeredAppenders()  
		 */
	    static Appender *createAppender(const QString &rAppenderClassName);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static Appender *createAppender(const char *pAppenderClassName);
	
		/*!
		 * Creates an object for the class \a rFilterClassName on the heap 
		 * and returns a pointer to it. If the class has no registered factory 
		 * function a null pointer is returned.
		 * 
		 * \sa registerFilter(), unregisterFilter(), registeredFilters()  
		 */
	    static Filter *createFilter(const QString &rFilterClassName);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static Filter *createFilter(const char *pFilterClassName);
	    
		/*!
		 * Creates an object for the class \a rLayoutClassName on the heap 
		 * and returns a pointer to it. If the class has no registered factory 
		 * function a null pointer is returned.
		 * 
		 * \sa registerLayout(), unregisterLayout(), registeredLayouts()  
		 */
	    static Layout *createLayout(const QString &rLayoutClassName);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static Layout *createLayout(const char *pLayoutClassName);
	    
        /*!
         * Returns the Factory instance.
         */
        static Factory *instance();
        
	    /*!
		 * Registers the Appender factory function \a pAppenderFactoryFunc
		 * for the class \a rAppenderClassName. If a registered factory 
		 * function exists for the class, it is replaced with 
		 * \a pAppenderFactoryFunc.
		 * 
		 * \sa unregisterAppender(), registeredAppenders(), createAppender()  
		 */
	    static void registerAppender(const QString &rAppenderClassName,
	    		                     AppenderFactoryFunc pAppenderFactoryFunc);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static void registerAppender(const char *pAppenderClassName,
	    		                     AppenderFactoryFunc pAppenderFactoryFunc);
	    
		/*!
		 * Registers the Filter factory function \a pFilterFactoryFunc
		 * for the class \a rFilterClassName. If a registered factory 
		 * function exists for the class, it is replaced with 
		 * \a pFilterFactoryFunc.
		 * 
		 * \sa unregisterFilter(), registeredFilters(), createFilter()
		 */
	    static void registerFilter(const QString &rFilterClassName,
	    		                   FilterFactoryFunc pFilterFactoryFunc);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static void registerFilter(const char *pFilterClassName,
	    		                   FilterFactoryFunc pFilterFactoryFunc);
	    
		/*!
		 * Registers the Layout factory function \a pLayoutFactoryFunc
		 * for the class \a rLayoutClassName. If a registered factory 
		 * function exists for the class, it is replaced with 
		 * \a pLayoutFactoryFunc.
		 * 
		 * \sa unregisterLayout(), registeredLayout(), createLayout() 
		 */
	    static void registerLayout(const QString &rLayoutClassName,
	    		                   LayoutFactoryFunc pLayoutFactoryFunc);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static void registerLayout(const char *pLayoutClassName,
	    		                   LayoutFactoryFunc pLayoutFactoryFunc);
	    
        /*! 
	     * Returns a list of the class names for registered Appender factory
	     * functions.
	     * 
	     * \sa registerAppender(), unregisterAppender()
	     */
	    static QStringList registeredAppenders();
	    
	    /*! 
	     * Returns a list of the class names for registered Filter factory
	     * functions.
	     * 
	     * \sa registerFilter(), unregisterFilter()
	     */
	    static QStringList registeredFilters();
	    
	    /*! 
	     * Returns a list of the class names for registered Layout factory
	     * functions.
	     * 
	     * \sa registerLayout(), unregisterLayout()
	     */
	    static QStringList registeredLayouts();
	    
        /*! 
         * Sets the property \a rProperty of the object \a pObject to the 
         * value \a rValue. The function will test that the property 
         * \a rProperty is writeable and of a type the function can convert to.
         * The types bool, int, Level and QString are supported.
         * 
         * \sa OptionConverter
         */
        static void setObjectProperty(QObject *pObject, 
                                      const QString &rProperty,
                                      const QString &rValue);
        
        /*!
         * This is an overloaded member function, provided for convenience.
         */
        static void setObjectProperty(QObject *pObject, 
                                      const char *pProperty, 
                                      const QString &rValue);

        /*!
		 * Unregisters the Appender factory function for the class 
		 * \a rAppenderClassName.
		 * 
		 * \sa registerAppender(), registeredAppenders() 
		 */
	    static void unregisterAppender(const QString &rAppenderClassName);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static void unregisterAppender(const char *pAppenderClassName);
	    
		/*!
		 * Unregisters the Filter factory function for the class 
		 * \a rFilterClassName.
		 * 
		 * \sa registerFilter(), registeredFilters() 
		 */
	    static void unregisterFilter(const QString &rFilterClassName);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static void unregisterFilter(const char *pFilterClassName);
	    
		/*!
		 * Unregisters the Layout factory function for the class 
		 * \a rLayoutClassName.
		 * 
		 * \sa registerLayout(), registeredLayouts() 
		 */
	    static void unregisterLayout(const QString &rLayoutClassName);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    static void unregisterLayout(const char *pLayoutClassName);

	private:
	    Appender *doCreateAppender(const QString &rAppenderClassName);
	    Filter *doCreateFilter(const QString &rFilterClassName);
	    Layout *doCreateLayout(const QString &rLayoutClassName);
	    void doRegisterAppender(const QString &rAppenderClassName,
	                            AppenderFactoryFunc pAppenderFactoryFunc);
	    void doRegisterFilter(const QString &rFilterClassName,
	                          FilterFactoryFunc pFilterFactoryFunc);
	    void doRegisterLayout(const QString &rLayoutClassName,
	                          LayoutFactoryFunc pLayoutFactoryFunc);
        void doSetObjectProperty(QObject *pObject, 
                                 const QString &rProperty, 
                                 const QString &rValue);
	    void doUnregisterAppender(const QString &rAppenderClassName);
	    void doUnregisterFilter(const QString &rFilterClassName);
        void doUnregisterLayout(const QString &rLayoutClassName);
	    void registerDefaultAppenders();
	    void registerDefaultFilters();
	    void registerDefaultLayouts();
	    bool validateObjectProperty(QMetaProperty &rMetaProperty, 
	                                const QString &rProperty,
	                                QObject *pObject);
	    
	private:
		mutable QMutex mObjectGuard;
	    QHash<QString, AppenderFactoryFunc> mAppenderRegistry;
	    QHash<QString, FilterFactoryFunc> mFilterRegistry;
        QHash<QString, LayoutFactoryFunc> mLayoutRegistry;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/

#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates Factory
	 * 
	 * Writes all object member variables to the given debug stream \a rDebug and
	 * returns the stream.
	 *
	 * <tt>
	 * %Factory(appenderfactories:("Log4Qt::DebugAppender", "Log4Qt::NullAppender", 
	 *          "Log4Qt::ConsoleAppender", "org.apache.log4j.varia.DebugAppender", 
	 *          "org.apache.log4j.FileAppender", "org.apache.log4j.RollingFileAppender", 
	 *          "org.apache.log4j.DailyRollingFileAppender", 
	 *          "org.apache.log4j.varia.ListAppender", 
	 *          "org.apache.log4j.varia.NullAppender", 
	 *          "Log4Qt::FileAppender", "org.apache.log4j.ConsoleAppender", 
	 *          "Log4Qt::DailyRollingFileAppender", "Log4Qt::ListAppender", 
	 *          "Log4Qt::RollingFileAppender") filterfactories: 
	 *          ("Log4Qt::DenyAllFilter", "Log4Qt::StringMatchFilter", 
	 *          "Log4Qt::LevelRangeFilter", "org.apache.log4j.varia.DenyAllFilter", 
	 *          "org.apache.log4j.varia.LevelRangeFilter", 
	 *          "org.apache.log4j.varia.StringMatchFilter", "Log4Qt::LevelMatchFilter", 
	 *          "org.apache.log4j.varia.LevelMatchFilter") layoutfactories: 
	 *          ("org.apache.log4j.SimpleLayout", "Log4Qt::PatternLayout", 
	 *          "Log4Qt::SimpleLayout", "org.apache.log4j.TTCCLayout", 
	 *          "Log4Qt::TTCCLayout", "org.apache.log4j.PatternLayout") )  
	 * </tt>
	 * \sa QDebug, Factory::logManager()
	 */
	QDebug operator<<(QDebug debug, 
	                  const Factory &rFactory);
#endif // QT_NO_DEBUG_STREAM

	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/

    inline Appender *Factory::createAppender(const QString &rAppenderClassName)
    {
        return instance()->doCreateAppender(rAppenderClassName);
    }
    
	inline Appender *Factory::createAppender(const char *pAppenderClassName)
    {
        return instance()->doCreateAppender(QLatin1String(pAppenderClassName));
    }

    inline Filter *Factory::createFilter(const QString &rFilterClassName)
    {
        return instance()->doCreateFilter(rFilterClassName);
    }

    inline Filter *Factory::createFilter(const char *pFilterClassName)
    {
        return instance()->doCreateFilter(QLatin1String(pFilterClassName));
    }
    
    inline Layout *Factory::createLayout(const QString &rLayoutClassName)
    {
        return instance()->doCreateLayout(rLayoutClassName);
    }
    
    inline Layout *Factory::createLayout(const char *pLayoutClassName)
    {
        return instance()->doCreateLayout(QLatin1String(pLayoutClassName));
    }
   
    inline void Factory::registerAppender(const QString &rAppenderClassName,
                                          AppenderFactoryFunc pAppenderFactoryFunc)
    {
        instance()->doRegisterAppender(rAppenderClassName, pAppenderFactoryFunc);
    }

    inline void Factory::registerAppender(const char *pAppenderClassName,
                                          AppenderFactoryFunc pAppenderFactoryFunc)
    {
        instance()->doRegisterAppender(QLatin1String(pAppenderClassName), pAppenderFactoryFunc);
    }
    
    inline void Factory::registerFilter(const QString &rFilterClassName,
                                 FilterFactoryFunc pFilterFactoryFunc)
    {
        instance()->doRegisterFilter(rFilterClassName, pFilterFactoryFunc);
    }
    
    inline void Factory::registerFilter(const char *pFilterClassName,
    		                            FilterFactoryFunc pFilterFactoryFunc)
    {
        instance()->doRegisterFilter(QLatin1String(pFilterClassName), pFilterFactoryFunc);
    }
    
    inline void Factory::registerLayout(const QString &rLayoutClassName,
                                 LayoutFactoryFunc pLayoutFactoryFunc)
    {
        instance()->doRegisterLayout(rLayoutClassName, pLayoutFactoryFunc);
    }
    
    inline void Factory::registerLayout(const char *pLayoutClassName,
    		                            LayoutFactoryFunc pLayoutFactoryFunc)
    {
        instance()->doRegisterLayout(QLatin1String(pLayoutClassName), pLayoutFactoryFunc);
    }

    inline QStringList Factory::registeredAppenders()
    {
        QMutexLocker locker(&instance()->mObjectGuard);
        return instance()->mAppenderRegistry.keys();
    }
    
    inline QStringList Factory::registeredFilters()
    {
        QMutexLocker locker(&instance()->mObjectGuard);
        return instance()->mFilterRegistry.keys();
    }
    
    inline QStringList Factory::registeredLayouts()
    {
        QMutexLocker locker(&instance()->mObjectGuard);
        return instance()->mLayoutRegistry.keys();
    }
    
    inline void Factory::setObjectProperty(QObject *pObject, 
                                           const QString &rProperty, 
                                           const QString &rValue)
    {
        instance()->doSetObjectProperty(pObject, rProperty, rValue);
    }
    
    inline void Factory::setObjectProperty(QObject *pObject, 
                                           const char *pProperty, 
                                           const QString &rValue)
    {
        instance()->doSetObjectProperty(pObject, QLatin1String(pProperty), rValue);
    }
    
    inline void Factory::unregisterAppender(const QString &rAppenderClassName)
    {
        instance()->doUnregisterAppender(rAppenderClassName);
    }
    
    inline void Factory::unregisterAppender(const char *pAppenderClassName)
    {
        instance()->doUnregisterAppender(QLatin1String(pAppenderClassName));
    }
    
    inline void Factory::unregisterFilter(const QString &rFilterClassName)
    {
        instance()->doUnregisterFilter(rFilterClassName);
    }
    
    inline void Factory::unregisterFilter(const char *pFilterClassName)
    {
        instance()->doUnregisterFilter(QLatin1String(pFilterClassName));
    }
    
    inline void Factory::unregisterLayout(const QString &rLayoutClassName)
    {
        instance()->doUnregisterLayout(rLayoutClassName);
    }
    
    inline void Factory::unregisterLayout(const char *pLayoutClassName)
    {
        instance()->doUnregisterLayout(QLatin1String(pLayoutClassName));
    }
    
} // namespace Log4Qt   


// Q_DECLARE_TYPEINFO(Log4Qt::Factory, Q_COMPLEX_TYPE); // use default


#endif // LOG4QT_HELPERS_FACTORY_H
