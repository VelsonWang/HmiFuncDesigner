/******************************************************************************
 *
 * package:     Logging
 * file:        propertyconfigurator.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * changes      Feb 2009, Martin Heinrich
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


#include "log4qt/propertyconfigurator.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include "log4qt/helpers/configuratorhelper.h"
#include "log4qt/helpers/factory.h"
#include "log4qt/helpers/optionconverter.h"
#include "log4qt/helpers/properties.h"
#include "log4qt/appender.h"
#include "log4qt/layout.h"
#include "log4qt/logger.h"
#include "log4qt/logmanager.h"
#include "log4qt/loggerrepository.h"
#include "log4qt/varia/listappender.h"



namespace Log4Qt
{
	    
	    
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
    LOG4QT_DECLARE_STATIC_LOGGER(logger, Log4Qt::PropertyConfigurator)
    
    
	
	/**************************************************************************
	 * Class implementation: PropertyConfigurator
	 **************************************************************************/
	

	bool PropertyConfigurator::doConfigure(const Properties &rProperties, 
                                           LoggerRepository *pLoggerRepository) 
	{
        startCaptureErrors();
		configureFromProperties(rProperties, pLoggerRepository);
		return stopCaptureErrors();
	}
	
	
	bool PropertyConfigurator::doConfigure(const QString &rConfigFileName, 
                                           LoggerRepository *pLoggerRepository)
	{
        startCaptureErrors();
		configureFromFile(rConfigFileName, pLoggerRepository);
        return stopCaptureErrors();
	}
	
	
    bool PropertyConfigurator::doConfigure(const QSettings &rSettings, 
                                           LoggerRepository *pLoggerRepository)
    {
        startCaptureErrors();
        configureFromSettings(rSettings, pLoggerRepository);
        return stopCaptureErrors();
    }

    
    bool PropertyConfigurator::configure(const Properties &rProperties)
	{
		PropertyConfigurator configurator;
		return configurator.doConfigure(rProperties);
	}
	
	
    bool PropertyConfigurator::configure(const QString &rConfigFilename)
    {
        PropertyConfigurator configurator;
        return configurator.doConfigure(rConfigFilename);
    }
    
    
    bool PropertyConfigurator::configure(const QSettings &rSettings)
    {
        PropertyConfigurator configurator;
        return configurator.doConfigure(rSettings);
    }
    
    
	bool PropertyConfigurator::configureAndWatch(const QString &rConfigFileName)
	{
		// Stop an existing watch to avoid a possible concurrent configuration
	    ConfiguratorHelper::setConfigurationFile();
		if (rConfigFileName.isEmpty())
			return true;

		PropertyConfigurator configurator;
		bool result = configurator.doConfigure(rConfigFileName);
		ConfiguratorHelper::setConfigurationFile(rConfigFileName, configure);
		return result;
	}
	
	
	void PropertyConfigurator::configureFromFile(const QString &rConfigFileName,
                                                 LoggerRepository *pLoggerRepository)
	{
	    QFile file(rConfigFileName);
	    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	    {
	        LogError e = LOG4QT_ERROR(QT_TR_NOOP("Unable to open property file '%1'"),
                                      CONFIGURATOR_OPENING_FILE_ERROR,
                                      "Log4Qt::PropertyConfigurator");
	        e << rConfigFileName;
	        e.addCausingError(LogError(file.errorString(), file.error()));
	        logger()->error(e);
	        return;
	    }
		Properties properties;
		properties.load(&file);
        if (file.error())
        {
            LogError e = LOG4QT_ERROR(QT_TR_NOOP("Unable to read property file '%1'"),
                                      CONFIGURATOR_READING_FILE_ERROR,
                                      "Log4Qt::PropertyConfigurator");
            e << rConfigFileName;
            e.addCausingError(LogError(file.errorString(), file.error()));
            logger()->error(e);
            return;
        }
		configureFromProperties(properties, pLoggerRepository);
	}
	
	
	void PropertyConfigurator::configureFromProperties(const Properties &rProperties, 
                                                       LoggerRepository *pLoggerRepository)
	{
		if (!pLoggerRepository)
			pLoggerRepository = LogManager::loggerRepository();
		
        configureGlobalSettings(rProperties, pLoggerRepository);
        configureRootLogger(rProperties, pLoggerRepository);
        configureNonRootElements(rProperties, pLoggerRepository);
        mAppenderRegistry.clear();
	}
	
	
    void PropertyConfigurator::configureFromSettings(const QSettings &rSettings,
                                                     LoggerRepository *pLoggerRepository)
    {
        Properties properties;
        properties.load(rSettings);
        configureFromProperties(properties, pLoggerRepository);
    }

    
    void PropertyConfigurator::configureGlobalSettings(const Properties &rProperties,
                                                       LoggerRepository *pLoggerRepository) const
    {
        Q_ASSERT_X(pLoggerRepository, "PropertyConfigurator::configureGlobalSettings()", "pLoggerRepository must not be null.");

        const QLatin1String key_reset("log4j.reset");
        const QLatin1String key_debug("log4j.Debug");
        const QLatin1String key_config_debug("log4j.configDebug");
        const QLatin1String key_threshold("log4j.threshold");
        const QLatin1String key_handle_qt_messages("log4j.handleQtMessages");

        // Test each global setting and set it
        // - Reset: log4j.reset
        // - Debug: log4j.Debug, log4j.configDebug
        // - Threshold: log4j.threshold
        // - Handle Qt Messages: log4j.handleQtMessages
        
        // Reset
        QString value = rProperties.property(key_reset);
        if (!value.isEmpty() && OptionConverter::toBoolean(value, false))
        {
            // Use LogManager and not pLoggerRepository to reset internal
            // logging.
            LogManager::resetConfiguration();
            logger()->debug("Reset configuration");
        }
            
        // Debug
        value = rProperties.property(key_debug);
        if (value.isNull())
        {
            value = rProperties.property(key_config_debug);
            if (!value.isNull())
                logger()->warn("[%1] is deprecated. Use [%2] instead.", key_config_debug, key_debug);
        }
        if (!value.isNull())
        {
            // Don't use OptionConverter::toLevel(). Invalid level string is a valid setting
            bool ok;
            Level level = Level::fromString(value, &ok);
            if (!ok)
                level = Level::DEBUG_INT;
            LogManager::logLogger()->setLevel(level);
            logger()->debug("Set level for Log4Qt logging to %1", 
                            LogManager::logLogger()->level().toString());
        }
        
        // Threshold
        value = rProperties.property(key_threshold);
        if (!value.isNull())
        {
            pLoggerRepository->setThreshold(OptionConverter::toLevel(value, Level::ALL_INT));
            logger()->debug("Set threshold for LoggerRepository to %1", 
                            pLoggerRepository->threshold().toString());
        }

        // Handle Qt messages
        value = rProperties.property(key_handle_qt_messages);
        if (!value.isNull())
        {
            LogManager::setHandleQtMessages(OptionConverter::toBoolean(value, false));
            logger()->debug("Set handling of Qt messages LoggerRepository to %1", 
                            QVariant(LogManager::handleQtMessages()).toString());
        }
    }

    
    void PropertyConfigurator::configureNonRootElements(const Properties &rProperties, 
                                                        LoggerRepository *pLoggerRepository)
    {
        Q_ASSERT_X(pLoggerRepository, "PropertyConfigurator::configureNonRootElements()", "pLoggerRepository must not be null.");
    
        const QString logger_prefix = QLatin1String("log4j.logger.");
        const QString category_prefix = QLatin1String("log4j.category.");

        // Iterate through all entries:
        // - Test for the logger/category prefix
        // - Convert JAVA class names to C++ ones
        // - Parse logger data (Level, Appender)
        // - Parse logger additivity
        
        QStringList keys = rProperties.propertyNames();
        QString key;
        Q_FOREACH(key, keys)
        {
            QString java_name;
            if (key.startsWith(logger_prefix))
                java_name = key.mid(logger_prefix.length());
            else if (key.startsWith(category_prefix))
                java_name = key.mid(category_prefix.length());
            QString cpp_name = OptionConverter::classNameJavaToCpp(java_name);
            if (!java_name.isEmpty())
            {
                Logger *p_logger = pLoggerRepository->logger(cpp_name);
                QString value = OptionConverter::findAndSubst(rProperties, key);
                parseLogger(rProperties, p_logger, key, value);
                parseAdditivityForLogger(rProperties, p_logger, java_name);
            }
        }
    }

    
    void PropertyConfigurator::configureRootLogger(const Properties &rProperties, 
                                                   LoggerRepository *pLoggerRepository)
    {
        Q_ASSERT_X(pLoggerRepository, "PropertyConfigurator::configureRootLogger()", "pLoggerRepository must not be null.");
        
        const QLatin1String key_root_logger("log4j.rootLogger");
        const QLatin1String key_root_category("log4j.rootCategory");
        
        // - Test for the logger/category prefix
        // - Parse logger data for root logger
        
        QString key = key_root_logger;
        QString value = OptionConverter::findAndSubst(rProperties, key);
        if (value.isNull())
        {
            key = key_root_category;
            value = OptionConverter::findAndSubst(rProperties, key);
            if (!value.isNull())
                logger()->warn("[%1] is deprecated. Use [%2] instead.", key_root_category, key_root_logger);
        }

        if (value.isNull())
            logger()->debug("Could not find root logger information. Is this correct?");
        else 
            parseLogger(rProperties, pLoggerRepository->rootLogger(), key, value);
    }

    
    void PropertyConfigurator::parseAdditivityForLogger(const Properties &rProperties,
                                                        Logger *pLogger, 
                                                        const QString &rLog4jName) const
    {
        Q_ASSERT_X(pLogger, "parseAdditivityForLogger()", "pLogger must not be null.");

        const QLatin1String additivity_prefix("log4j.additivity.");
        
        // - Lookup additivity key for logger
        // - Set additivity, if specified
        
        QString key = additivity_prefix + rLog4jName;
        QString value = OptionConverter::findAndSubst(rProperties, key);
        logger()->debug("Parsing additivity for logger: key '%1', value '%2'", key, value);
        if (!value.isEmpty())
        {
            bool additivity = OptionConverter::toBoolean(value, true);
            logger()->debug("Setting additivity for logger '%1' to '%2'", pLogger->name(), QVariant(value).toString());
            pLogger->setAdditivity(additivity);
        }
    }
    

    LogObjectPtr<Appender> PropertyConfigurator::parseAppender(const Properties &rProperties,
                                                               const QString &rName)
    {
        // - Test if appender has been parsed before
        // - Find appender key
        // - Create appender object
        // - Set layout, if required by appender
        // - Set properties
        // - Activate options
        // - Add appender to registry
        
        const QLatin1String appender_prefix("log4j.appender.");
        
        logger()->debug("Parsing appender named '%1'", rName);

        if (mAppenderRegistry.contains(rName))
        {
            logger()->debug("Appender '%1' was already parsed.", rName);
            return mAppenderRegistry.value(rName);
        }

        QString key = appender_prefix + rName;
        QString value = OptionConverter::findAndSubst(rProperties, key);
        if (value.isNull())
        {
            LogError e = LOG4QT_ERROR(QT_TR_NOOP("Missing appender definition for appender named '%1'"),
                                      CONFIGURATOR_MISSING_APPENDER_ERROR,
                                      "Log4Qt::PropertyConfigurator");
            e << rName;
            logger()->error(e);
            return 0;
        }
        LogObjectPtr<Appender> p_appender = Factory::createAppender(value);
        if (!p_appender)
        {
            LogError e = LOG4QT_ERROR(QT_TR_NOOP("Unable to create appender of class '%1' namd '%2'"),
                                      CONFIGURATOR_UNKNOWN_APPENDER_CLASS_ERROR,
                                      "Log4Qt::PropertyConfigurator");
            e << value << rName;
            logger()->error(e);
            return 0;
        }
        p_appender->setName(rName);
        
        if (p_appender->requiresLayout())
        {
            LogObjectPtr<Layout> p_layout = parseLayout(rProperties, key);
            if (p_layout)
                p_appender->setLayout(p_layout);
            else
                return 0;
        }
        
        QStringList exclusions;
        exclusions << QLatin1String("layout");
        setProperties(rProperties, key + QLatin1String("."), exclusions, p_appender);
        AppenderSkeleton *p_appenderskeleton = qobject_cast<AppenderSkeleton *>(p_appender);
        if (p_appenderskeleton) 
            p_appenderskeleton->activateOptions();
        
        mAppenderRegistry.insert(rName, p_appender);
        return p_appender;
    }
    
    
    LogObjectPtr<Layout> PropertyConfigurator::parseLayout(const Properties &rProperties,
                                                           const QString &rAppenderKey)
    {
        Q_ASSERT_X(!rAppenderKey.isEmpty(), "PropertyConfigurator::parseLayout()", "rAppenderKey must not be empty.");

        // - Find layout key
        // - Create layput object
        // - Set properties
        // - Activate options

        const QLatin1String layout_suffix(".layout");
        
        logger()->debug("Parsing layout for appender named '%1'", rAppenderKey);

        QString key = rAppenderKey + layout_suffix;
        QString value = OptionConverter::findAndSubst(rProperties, key);
        if (value.isNull())
        {
            LogError e = LOG4QT_ERROR(QT_TR_NOOP("Missing layout definition for appender '%1'"),
                                      CONFIGURATOR_MISSING_LAYOUT_ERROR,
                                      "Log4Qt::PropertyConfigurator");
            e << rAppenderKey;
            logger()->error(e);
            return 0;
        }
        LogObjectPtr<Layout> p_layout = Factory::createLayout(value);
        if (!p_layout)
        {
            LogError e = LOG4QT_ERROR(QT_TR_NOOP("Unable to create layoput of class '%1' requested by appender '%2'"),
                                      CONFIGURATOR_UNKNOWN_LAYOUT_CLASS_ERROR,
                                      "Log4Qt::PropertyConfigurator");
            e << value << rAppenderKey;
            logger()->error(e);
            return 0;
        }
        
        QStringList exclusions;
        setProperties(rProperties, key + QLatin1String("."), QStringList(), p_layout);
        p_layout->activateOptions();
        
        return p_layout;
    }
    
    
    void PropertyConfigurator::parseLogger(const Properties &rProperties, 
                                           Logger *pLogger,
                                           const QString &rKey,
                                           const QString &rValue)
    {
        Q_ASSERT_X(pLogger, "PropertyConfigurator::parseLogger()", "pLogger must not be null.");
        Q_ASSERT_X(!rKey.isEmpty(), "PropertyConfigurator::parseLogger()", "rKey must not be empty.");
        
        const QLatin1String keyword_inherited("INHERITED");
        
        // - Split value on comma
        // - If level value, is specified
        //   - Test for NULL and INHERITED
        //   - Ensure root logger is not set to NULL
        //   - Set level
        // - For each entry
        //   - Create Appender
        
        logger()->debug("Parsing logger: key '%1', value '%2'", rKey, rValue);
        QStringList appenders = rValue.split(QLatin1Char(','));
        QStringListIterator i (appenders);
        
        // First entry is the level. There will be always one entry, even if the rValue is 
        // empty or does not contain a comma.
        QString value = i.next().trimmed();
        if (!value.isEmpty())
        {
            Level level;
            if (value.compare(keyword_inherited,Qt::CaseInsensitive) == 0)
                level = Level::NULL_INT;
            else
                level = OptionConverter::toLevel(value, Level::DEBUG_INT);
            if (level == Level::NULL_INT && pLogger->name() == QString())
                logger()->warn("The root logger level cannot be set to NULL.");
            else
            {
                pLogger->setLevel(level);
                logger()->debug("Set level for logger '%1' to '%2'", 
                                pLogger->name(), pLogger->level().toString());
            }
        }
        
        pLogger->removeAllAppenders();
        while(i.hasNext())
        {
            value = i.next().trimmed();
            if(value.isEmpty())
                continue;
            LogObjectPtr<Appender> p_appender = parseAppender(rProperties, value);
            if (p_appender)
                pLogger->addAppender(p_appender);
        }
    }
    
    
    void PropertyConfigurator::setProperties(const Properties &rProperties,
                                             const QString &rPrefix,
                                             const QStringList &rExclusions,
                                             QObject *pObject)
    {
        Q_ASSERT_X(!rPrefix.isEmpty(), "PropertyConfigurator::setProperties()", "rPrefix must not be empty.");
        Q_ASSERT_X(pObject, "PropertyConfigurator::setProperties()", "pObject must not be null.");
        
        // Iterate through all entries:
        // - Test for prefix to determine, if setting is for object
        // - Skip empty property name
        // - Skip property names in exclusion list
        // - Set property on object
        
        logger()->debug("Setting properties for object of class '%1' from keys starting with '%2'",
                        QLatin1String(pObject->metaObject()->className()),
                        rPrefix);
        
        QStringList keys = rProperties.propertyNames();
        QString key;
        Q_FOREACH(key, keys)
        {
            if (!key.startsWith(rPrefix))
                continue;
            QString property = key.mid(rPrefix.length());
            if (property.isEmpty())
                continue;
            QStringList split_property = property.split(QLatin1Char('.'));
            if (rExclusions.contains(split_property.at(0), Qt::CaseInsensitive))
                continue;
            QString value = OptionConverter::findAndSubst(rProperties, key);
            Factory::setObjectProperty(pObject, property, value);
        }
    }
    
    
    void PropertyConfigurator::startCaptureErrors()
    {
        Q_ASSERT_X(!mpConfigureErrors, "PropertyConfigurator::startCaptureErrors()", "mpConfigureErrors must be empty.");
        
        mpConfigureErrors = new ListAppender;
        mpConfigureErrors->setName(QLatin1String("PropertyConfigurator"));
        mpConfigureErrors->setConfiguratorList(true);
        mpConfigureErrors->setThreshold(Level::ERROR_INT);
        LogManager::logLogger()->addAppender(mpConfigureErrors);
    }
    
    
    bool PropertyConfigurator::stopCaptureErrors()
    {
        Q_ASSERT_X(mpConfigureErrors, "PropertyConfigurator::stopCaptureErrors()", "mpConfigureErrors must not be empty.");
        
        LogManager::logLogger()->removeAppender(mpConfigureErrors);
        ConfiguratorHelper::setConfigureError(mpConfigureErrors->list());
        bool result = (mpConfigureErrors->list().count() == 0);
        mpConfigureErrors = 0;
        return result;
    }
    
    
    
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug,
	                  const PropertyConfigurator &rPropertyConfigurator)
	{
		Q_UNUSED(rPropertyConfigurator);
		debug.nospace() << "PropertyConfigurator(" 
		    << ")";
		return debug.space();    
	}
#endif
	
	
	
} // namespace Logging
