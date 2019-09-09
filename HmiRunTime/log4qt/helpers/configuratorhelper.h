/******************************************************************************
 *
 * package:     Log4Qt
 * file:        configuratorhelper.h
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

#ifndef LOG4QT_HELPERS_CONFIGURATORHELPER_H
#define LOG4QT_HELPERS_CONFIGURATORHELPER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QObject>

#include <QtCore/QList>
#include <QtCore/QMutex>
#include "log4qt/loggingevent.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

class QFileSystemWatcher;


namespace Log4Qt
{
	    
	/*!
	 * \brief The class ConfiguratorHelper provides a confiuration file watch 
	 *        and last error for configurator classes.
	 * 
	 * A configuration file can be set using setConfigurationFile(). The file
	 * is watched for changes. If a change occurs the configuration is reloaded 
	 * and the ConfigurationFileChanged() signal is emitted. Error information 
	 * for the last call to a configure function or the last configuration file 
	 * change can be accessed using configureError().
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 */
	class ConfiguratorHelper : public QObject
	{
		Q_OBJECT
	
	public:
		/*!
	     * Prototype for a configure callback function. The function is called 
	     * when then configuration file is changed and takes the
	     * configuration file as a parameter.
	     * 
	     * \sa setConfigurationFile(),
	     *     PropertyConfigurator::configure(const QString &)
		 */
		typedef bool (*ConfigureFunc)(const QString &rFileName);
		
	private:
	    ConfiguratorHelper();
	    ConfiguratorHelper(const ConfiguratorHelper &rOther); // Not implemented
	    virtual ~ConfiguratorHelper();
	    ConfiguratorHelper &operator=(const ConfiguratorHelper &rOther); // Not implemented
	
	public:

		/*!
		 * Returns the error information for the last configuration operation 
		 * that took place. The configuration operation could be the result of 
		 * a call to one of the configure methods or through a change
		 * to the configuration file.
		 * 
		 * \sa setConfigureError(), PropertyConfigurator::configure(),
		 *     setConfigurationFile()
		 */
		static QList<LoggingEvent> configureError();
		
	    /*!
	     * Returns the current configuration file.
	     * 
	     * \sa setConfigurationFile()
	     */
	    static QString configurationFile();

		/*!
	     * Returns the ConfiguratorHelper instance.
	     */
	    static ConfiguratorHelper *instance();

		/*!
		 * Sets the configuration error information for the last configuration
		 * operation.
		 * 
		 * \sa configureError()
		 */
		static void setConfigureError(const QList<LoggingEvent> &rConfigureError);
		
	    /*!
	     * Sets the configuration file to \a rFileName. The file is watched for 
	     * changes. On a file change the function \a pConfigureFunc will be called 
	     * and the signal configurationFileChange() will be emitted.
	     * 
	     * Setting the configuration file to an empty string stops the file watch.
	     * 
	     * \sa configurationFile(), PropertyConfigurator::configureAndWatch(),
	     *     configureError()
	     */
	    static void setConfigurationFile(const QString &rFileName = QString(),
                                         ConfigureFunc pConfigureFunc = 0);

	signals:
		/*!
		 * The signal is emitted after a change to the file \a rFileName
		 * was processed. If an error occured during the configuration, the 
		 * flag \a error will be true and error information is available 
		 * over configureError().
		 */
		void configurationFileChanged(const QString &rFileName,
				                      bool error);
	
	private slots:
		void doConfigurationFileChanged(const QString &rFileName);
		
	private:
	    void doSetConfigurationFile(const QString &rFileName,
                                    ConfigureFunc pConfigureFunc);
	    
	private:
		mutable QMutex mObjectGuard;
	    QString mConfigurationFile;
	    ConfigureFunc mpConfigureFunc;
	    QFileSystemWatcher *mpConfigurationFileWatch;
	    QList<LoggingEvent> mConfigureError;
	    
#ifndef QT_NO_DEBUG_STREAM
	    // Needs to be friend to access details
	    friend QDebug operator<<(QDebug debug, 
                                 const ConfiguratorHelper &rConfiguratorHelper);
#endif // QT_NO_DEBUG_STREAM
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/

#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates ConfiguratorHelper
	 * 
	 * Writes all object member variables to the given debug stream \a rDebug and
	 * returns the stream.
	 *
	 * <tt>
	 * %ConfiguratorHelper(configurationfile: "" configurefunc: false 
	 *                     filesystemwatcher: QObject(0x0) )  
	 * </tt>
	 * \sa QDebug, ConfiguratorHelper::logManager()
	 */
	QDebug operator<<(QDebug debug, 
	                  const ConfiguratorHelper &rConfiguratorHelper);
#endif // QT_NO_DEBUG_STREAM

	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline QList<LoggingEvent> ConfiguratorHelper::configureError()
	{   QMutexLocker locker(&instance()->mObjectGuard);
		return instance()->mConfigureError;	}

	inline QString ConfiguratorHelper::configurationFile()
	{   QMutexLocker locker(&instance()->mObjectGuard);
		return instance()->mConfigurationFile;	}

	inline void ConfiguratorHelper::setConfigureError(const QList<LoggingEvent> &rConfigureError)
	{   QMutexLocker locker(&instance()->mObjectGuard);
		instance()->mConfigureError = rConfigureError;	}

	inline void ConfiguratorHelper::setConfigurationFile(const QString &rFileName,
                                                       ConfigureFunc pConfigureFunc)
	{   instance()->doSetConfigurationFile(rFileName, pConfigureFunc);	}
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::ConfiguratorHelper, Q_COMPLEX_TYPE); // use default


#endif // LOG4QT_HELPERS_CONFIGURATORHELPER_H
