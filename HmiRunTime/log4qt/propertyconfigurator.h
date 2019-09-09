/******************************************************************************
 * 
 * package:     Logging
 * file:        propertyconfigurator.h
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

#ifndef LOG4QT_PROPERTYCONFIGURATOR_H
#define LOG4QT_PROPERTYCONFIGURATOR_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/


#include <QtCore/QHash>
#include "log4qt/helpers/logobjectptr.h"
#include "log4qt/log4qt.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

class QSettings;

namespace Log4Qt
{

    class Appender;
    class Layout;
    class ListAppender;
    class Logger;
    class Properties;
	class LoggerRepository;
	
	/*!
	 * \brief The class PropertyConfigurator allows the configuration of the 
	 *        package from a JAVA properties file.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 */
	class PropertyConfigurator
	{
	public:
	    PropertyConfigurator();
	    // virtual ~PropertyConfigurator(); // Use compiler default
	private:
	    PropertyConfigurator(const PropertyConfigurator &rOther); // Not implemented
	    PropertyConfigurator &operator=(const PropertyConfigurator &rOther); // Not implemented
	    
	public:
		/*!
		 * \sa ConfiguratorHelper::configureError()
		 */
		bool doConfigure(const Properties &rProperties, 
                         LoggerRepository *pLoggerRepository = 0); 
		
		/*!
		 * \sa ConfiguratorHelper::configureError()
		 */
		bool doConfigure(const QString &rConfigFileName, 
                         LoggerRepository *pLoggerRepository = 0);
		
        /*!
         * Reads the configuration data from the QSettings object 
         * \a rSettings.
         * 
         * \sa \ref Properties::load(const QSettings &) "Properties::load()",
         *     ConfiguratorHelper::configureError()
         */
        bool doConfigure(const QSettings &rSettings,
                         LoggerRepository *pLoggerRepository = 0);
		
	    // JAVA: void doConfigure(const QUrl &rUrl, LoggerRepository *pLoggerRepository);
		
		/*!
		 * \sa ConfiguratorHelper::configureError()
		 */
		static bool configure(const Properties &rProperties);
		
        /*!
         * \sa ConfiguratorHelper::configureError()
         */
        static bool configure(const QString &rConfigFilename);
        
        /*!
         * Reads the configuration data from the QSettings object 
         * \a rSettings.
         * 
         * \sa \ref doConfigure(const QSettings &, LoggerRepository *) "doConfigure()",
         *     \ref Properties::load(const QSettings &) "Properties::load()",
         *     ConfiguratorHelper::configureError()
         */
        static bool configure(const QSettings &rSettings);
        
	    // JAVA: static void configure(const QUrl &rUrl);
		 
		/*!
		 * \sa ConfiguratorHelper::configureError(),
		 *     ConfiguratorHelper::configurationFile()
		 */
	    static bool configureAndWatch(const QString &rConfigFilename);
	    
	private:
		void configureFromFile(const QString &rConfigFileName,
                               LoggerRepository *pLoggerRepository);
        void configureFromProperties(const Properties &rProperties, 
                                     LoggerRepository *pLoggerRepository);
        void configureFromSettings(const QSettings &rSettings, 
                                   LoggerRepository *pLoggerRepository);
        void configureGlobalSettings(const Properties &rProperties,
                                     LoggerRepository *pLoggerRepository) const;
        void configureNonRootElements(const Properties &rProperties, 
                                      LoggerRepository *pLoggerRepository);
        void configureRootLogger(const Properties &rProperties, 
                                 LoggerRepository *pLoggerRepository);
        void parseAdditivityForLogger(const Properties &rProperties,
                                      Logger *pLogger, 
                                      const QString &rLog4jName) const;
        LogObjectPtr<Appender> parseAppender(const Properties &rProperties,
                                             const QString &rName);
        LogObjectPtr<Layout> parseLayout(const Properties &rProperties,
                                         const QString &rAppenderName);
        void parseLogger(const Properties &rProperties, 
                         Logger *pLogger, 
                         const QString &rKey,
                         const QString &rValue);
        void setProperties(const Properties &rProperties,
                           const QString &rPrefix,
                           const QStringList &rExclusions,
                           QObject *pObject);
        void startCaptureErrors();
        bool stopCaptureErrors();
        
	private:
	    LogObjectPtr<ListAppender> mpConfigureErrors;
	    QHash< QString, LogObjectPtr<Appender> > mAppenderRegistry;
	};
	    
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates PropertyConfigurator
	 * 
	 * Writes all object member variables to the given debug stream \a debug 
	 * and returns the stream.
	 *
	 * <tt>
	 * %PropertyConfigurator() 
	 * </tt>
	 * \sa QDebug
	 */
	QDebug operator<<(QDebug debug,
	                  const PropertyConfigurator &rPropertyConfigurator);
#endif
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline PropertyConfigurator::PropertyConfigurator()
	{}
	
	
} // namspace Logging


// Q_DECLARE_TYPEINFO(Log4Qt::PropertyConfigurator, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_PROPERTYCONFIGURATOR_H
