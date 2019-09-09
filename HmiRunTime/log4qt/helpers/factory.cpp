/******************************************************************************
 *
 * package:     Log4Qt
 * file:        factory.cpp
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



/******************************************************************************
 * Dependencies
 ******************************************************************************/


#include "log4qt/helpers/factory.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include "log4qt/consoleappender.h"
#include "log4qt/dailyrollingfileappender.h"
#include "log4qt/fileappender.h"
#include "log4qt/helpers/logerror.h"
#include "log4qt/helpers/initialisationhelper.h"
#include "log4qt/helpers/optionconverter.h"
#include "log4qt/patternlayout.h"
#include "log4qt/rollingfileappender.h"
#include "log4qt/simplelayout.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/varia/debugappender.h"
#include "log4qt/varia/denyallfilter.h"
#include "log4qt/varia/levelmatchfilter.h"
#include "log4qt/varia/levelrangefilter.h"
#include "log4qt/varia/listappender.h"
#include "log4qt/varia/nullappender.h"
#include "log4qt/varia/stringmatchfilter.h"



namespace Log4Qt
{
	
	    
	/**************************************************************************
	 * Declarations
	 **************************************************************************/	
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/


    LOG4QT_DECLARE_STATIC_LOGGER(logger, Log4Qt::Factory)
    
    
    // Appenders

	Appender *console_file_appender()
	{	return new ConsoleAppender;	}

	Appender *create_daily_rolling_file_appender()
	{	return new DailyRollingFileAppender;	}

	Appender *create_debug_appender()
	{	return new DebugAppender;	}

	Appender *create_file_appender()
	{	return new FileAppender;	}

	Appender *create_list_appender()
	{	return new ListAppender;	}

	Appender *create_null_appender()
	{	return new NullAppender;	}

	Appender *create_rolling_file_appender()
	{	return new RollingFileAppender;	}
	
	
	// Filters
	
	Filter *create_deny_all_filter()
	{	return new DenyAllFilter;	}

	Filter *create_level_match_filter()
	{	return new LevelMatchFilter;	}

	Filter *create_level_range_filter()
	{	return new LevelRangeFilter;	}

	Filter *create_string_match_filter()
	{	return new StringMatchFilter;	}

	
	// Layouts
	
	Layout *create_pattern_layout()
	{	return new PatternLayout;	}

	Layout *create_simple_layout()
	{	return new SimpleLayout;	}

	Layout *create_ttcc_layout()
	{	return new TTCCLayout;	}

    
	
	/**************************************************************************
	 * Class implementation: Factory
	 **************************************************************************/
	
	    
	Factory::Factory() :
		mObjectGuard(),
		mAppenderRegistry(),
		mFilterRegistry(),
        mLayoutRegistry()
	{
		registerDefaultAppenders();
		registerDefaultFilters();
        registerDefaultLayouts();
	}
	    
	    
    LOG4QT_IMPLEMENT_INSTANCE(Factory)
    
    
	Appender *Factory::doCreateAppender(const QString &rAppenderClassName)
    {	
    	QMutexLocker locker(&mObjectGuard);    

    	if (!mAppenderRegistry.contains(rAppenderClassName))
	    {
	        logger()->warn("Request for the creation of Appender with class '%1', which is not registered", rAppenderClassName);
	    	return 0;
	    }
	    return mAppenderRegistry.value(rAppenderClassName)();
	}


	Filter *Factory::doCreateFilter(const QString &rFilterClassName)
    {	
    	QMutexLocker locker(&mObjectGuard);    

    	if (!mFilterRegistry.contains(rFilterClassName))
	    {
	        logger()->warn("Request for the creation of Filter with class '%1', which is not registered", rFilterClassName);
	    	return 0;
	    }
	    return mFilterRegistry.value(rFilterClassName)();
    }


	Layout *Factory::doCreateLayout(const QString &rLayoutClassName)
    {	
    	QMutexLocker locker(&mObjectGuard);
    
    	if (!mLayoutRegistry.contains(rLayoutClassName))
	    {
	        logger()->warn("Request for the creation of Layout with class '%1', which is not registered", rLayoutClassName);
	    	return 0;
	    }
	    return mLayoutRegistry.value(rLayoutClassName)();
    }

	
	void Factory::doRegisterAppender(const QString &rAppenderClassName,
    		                         AppenderFactoryFunc pAppenderFactoryFunc)
    {	
    	QMutexLocker locker(&mObjectGuard);
    	
        if(rAppenderClassName.isEmpty())
        {
            logger()->warn("Registering Appender factory function with empty class name");
            return;
        }
    	mAppenderRegistry.insert(rAppenderClassName, pAppenderFactoryFunc);
    }


    void Factory::doRegisterFilter(const QString &rFilterClassName,
    		                       FilterFactoryFunc pFilterFactoryFunc)
    {	
    	QMutexLocker locker(&mObjectGuard);
    	
        if(rFilterClassName.isEmpty())
        {
            logger()->warn("Registering Filter factory function with empty class name");
            return;
        }
    	mFilterRegistry.insert(rFilterClassName, pFilterFactoryFunc);
    }

    
    void Factory::doRegisterLayout(const QString &rLayoutClassName,
    		                       LayoutFactoryFunc pLayoutFactoryFunc)
    {	
    	QMutexLocker locker(&mObjectGuard);
    	
        if(rLayoutClassName.isEmpty())
        {
            logger()->warn("Registering Layout factory function with empty class name");
            return;
        }
        mLayoutRegistry.insert(rLayoutClassName, pLayoutFactoryFunc);
    }
    
    
    void Factory::doSetObjectProperty(QObject *pObject, 
                                      const QString &rProperty, 
                                      const QString &rValue)
    {
        // - Validate property
        // - Get correct property name from meta object
        // - Find specific property setter
        // - If no specfifc propery setter can be found,
        //   find general property setter
        // - Call property setter

        QMetaProperty meta_property;
        if (!validateObjectProperty(meta_property, rProperty, pObject))
            return;
        
        QString property = QLatin1String(meta_property.name());
        QString type = QLatin1String(meta_property.typeName());
        logger()->debug("Setting property '%1' on object of class '%2' to value '%3'",
                        property,
                        QLatin1String(pObject->metaObject()->className()),
                        rValue);
        
        QVariant value;
        bool ok = true;
        if (type == QLatin1String("bool"))
            value = OptionConverter::toBoolean(rValue, &ok);
        else if (type == QLatin1String("int"))
            value = OptionConverter::toInt(rValue, &ok);
        else if (type == QLatin1String("qint64") || type == QLatin1String("qlonglong"))
            value = OptionConverter::toQInt64(rValue, &ok);
        else if (type == QLatin1String("Log4Qt::Level"))
            value = QVariant::fromValue(OptionConverter::toLevel(rValue, &ok));
        else if (type == QLatin1String("QString"))
            value = rValue;
        else
        {
            LogError e = LOG4QT_ERROR(QT_TR_NOOP("Cannot convert to type '%1' for property '%2' on object of class '%3'"),
                                      CONFIGURATOR_UNKNOWN_TYPE_ERROR,
                                      "Log4Qt::Factory");
            e << type
              << property
              << QString::fromLatin1(pObject->metaObject()->className());
            logger()->error(e);
            return;
        }        
        if (!ok)
            return;
        
        // Everything is checked and the type is the one of the property. 
        // Write should never return false 
        if (!meta_property.write(pObject, value))
            logger()->warn("Unxpected error result from QMetaProperty.write()");
    }
    
    
    void Factory::doUnregisterAppender(const QString &rAppenderClassName)
    {
    	QMutexLocker locker(&mObjectGuard);
    	
    	if (!mAppenderRegistry.contains(rAppenderClassName))
    	{
    		logger()->warn("Request to unregister not registered Appender factory function for class '%1'", rAppenderClassName);
    		return;
    	}
    	mAppenderRegistry.remove(rAppenderClassName);
    }
    
    
    void Factory::doUnregisterFilter(const QString &rFilterClassName)
    {
    	QMutexLocker locker(&mObjectGuard);

    	if (!mFilterRegistry.contains(rFilterClassName))
    	{
    		logger()->warn("Request to unregister not registered Filter factory function for class '%1'", rFilterClassName);
    		return;
    	}
    	mFilterRegistry.remove(rFilterClassName);
    }
    
    
    void Factory::doUnregisterLayout(const QString &rLayoutClassName)
    {
        QMutexLocker locker(&mObjectGuard);
        
        if (!mLayoutRegistry.contains(rLayoutClassName))
        {
            logger()->warn("Request to unregister not registered Layout factory function for class '%1'", rLayoutClassName);
            return;
        }
        mLayoutRegistry.remove(rLayoutClassName);
    }

	    
    void Factory::registerDefaultAppenders()
    {
    	mAppenderRegistry.insert(QLatin1String("org.apache.log4j.ConsoleAppender"), console_file_appender);
    	mAppenderRegistry.insert(QLatin1String("Log4Qt::ConsoleAppender"), console_file_appender);
    	mAppenderRegistry.insert(QLatin1String("org.apache.log4j.DailyRollingFileAppender"), create_daily_rolling_file_appender);
    	mAppenderRegistry.insert(QLatin1String("Log4Qt::DailyRollingFileAppender"), create_daily_rolling_file_appender);
    	mAppenderRegistry.insert(QLatin1String("org.apache.log4j.varia.DebugAppender"), create_debug_appender);
    	mAppenderRegistry.insert(QLatin1String("Log4Qt::DebugAppender"), create_debug_appender);
    	mAppenderRegistry.insert(QLatin1String("org.apache.log4j.FileAppender"), create_file_appender);
    	mAppenderRegistry.insert(QLatin1String("Log4Qt::FileAppender"), create_file_appender);
    	mAppenderRegistry.insert(QLatin1String("org.apache.log4j.varia.ListAppender"), create_list_appender);
    	mAppenderRegistry.insert(QLatin1String("Log4Qt::ListAppender"), create_list_appender);
    	mAppenderRegistry.insert(QLatin1String("org.apache.log4j.varia.NullAppender"), create_null_appender);
    	mAppenderRegistry.insert(QLatin1String("Log4Qt::NullAppender"), create_null_appender);
    	mAppenderRegistry.insert(QLatin1String("org.apache.log4j.RollingFileAppender"), create_rolling_file_appender);
    	mAppenderRegistry.insert(QLatin1String("Log4Qt::RollingFileAppender"), create_rolling_file_appender);
    }
    
    
    void Factory::registerDefaultFilters()
    {
    	mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.DenyAllFilter"), create_deny_all_filter);
    	mFilterRegistry.insert(QLatin1String("Log4Qt::DenyAllFilter"), create_deny_all_filter);
    	mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.LevelMatchFilter"), create_level_match_filter);
    	mFilterRegistry.insert(QLatin1String("Log4Qt::LevelMatchFilter"), create_level_match_filter);
    	mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.LevelRangeFilter"), create_level_range_filter);
    	mFilterRegistry.insert(QLatin1String("Log4Qt::LevelRangeFilter"), create_level_range_filter);
    	mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.StringMatchFilter"), create_string_match_filter);
    	mFilterRegistry.insert(QLatin1String("Log4Qt::StringMatchFilter"), create_string_match_filter);
    }
    
    
    void Factory::registerDefaultLayouts()
    {
    	mLayoutRegistry.insert(QLatin1String("org.apache.log4j.PatternLayout"), create_pattern_layout);
    	mLayoutRegistry.insert(QLatin1String("Log4Qt::PatternLayout"), create_pattern_layout);
    	mLayoutRegistry.insert(QLatin1String("org.apache.log4j.SimpleLayout"), create_simple_layout);
    	mLayoutRegistry.insert(QLatin1String("Log4Qt::SimpleLayout"), create_simple_layout);
    	mLayoutRegistry.insert(QLatin1String("org.apache.log4j.TTCCLayout"), create_ttcc_layout);
    	mLayoutRegistry.insert(QLatin1String("Log4Qt::TTCCLayout"), create_ttcc_layout);
    }

    
    bool Factory::validateObjectProperty(QMetaProperty &rMetaProperty, 
                                         const QString &rProperty,
                                         QObject *pObject)
    {
        // Validate:
        // - No null object pointer
        // - No empty property name
        // - Property exists on the object (QT or Java name)
        // - Property is readable
        // - Property is writable 

        const char *p_context = "Log4Qt::Factory";
        LogError e = LOG4QT_ERROR(QT_TR_NOOP("Unable to set property value on object"),
                                  CONFIGURATOR_PROPERTY_ERROR,
                                  p_context);
        
        if (!pObject)
        {
            LogError ce = LOG4QT_ERROR(QT_TR_NOOP("Invalid null object pointer"), 
                                       0,
                                       p_context);
            e.addCausingError(ce);
            logger()->error(e);
            return false;
        }
        if (rProperty.isEmpty())
        {
            LogError ce = LOG4QT_ERROR(QT_TR_NOOP("Invalid empty property name"), 
                                       0, 
                                       p_context);
            e.addCausingError(ce);
            logger()->error(e);
            return false;
        }
        const QMetaObject *p_meta_object = pObject->metaObject();
        QString property = rProperty;
        int i = p_meta_object->indexOfProperty(property.toLatin1());
        if (i < 0)
        {
            // Try name with lower case first character. Java properties names
            // start upper case
            property[0] = property[0].toLower(); 
            i = p_meta_object->indexOfProperty(property.toLatin1());
            if (i < 0)
            {
                LogError ce = LOG4QT_ERROR(QT_TR_NOOP("Property '%1' does not exist in class '%2'"), 
                                           0, 
                                           p_context);
                ce << property
                   << QString::fromLatin1(pObject->metaObject()->className());
                e.addCausingError(ce);
                logger()->error(e);
                return false;
            }
        }
        rMetaProperty = p_meta_object->property(i);
        if (!rMetaProperty.isWritable())
        {
            LogError ce = LOG4QT_ERROR(QT_TR_NOOP("Property '%1' is not writable in class '%2'"), 
                                       0, 
                                       p_context);
            ce << property
               << QString::fromLatin1(pObject->metaObject()->className());
            e.addCausingError(ce);
            logger()->error(e);
            return false;
        }
        
        return true;
    }
    
    
    
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/

	
#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug, 
	                  const Factory &rFactory)
	{
		debug.nospace() << "Factory("
			<< "appenderfactories:" << rFactory.registeredAppenders()
			<< "filterfactories:" << rFactory.registeredFilters()
			<< "layoutfactories:" << rFactory.registeredLayouts()
	        << ")";
		return debug.space();
	}
#endif // QT_NO_DEBUG_STREAM

	
	
} // namespace Log4Qt

