/******************************************************************************
 *
 * package:     Log4Qt
 * file:        optionconverter.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * changes      Feb 2009, Martin Heinrich
 *              - Fixed a problem were OptionConverter::toBoolean would not
 *                return the default value, if the conversion fails.
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


#include "log4qt/helpers/optionconverter.h"

#include <QtCore/QDebug>
#include "log4qt/helpers/logerror.h"
#include "log4qt/helpers/properties.h"
#include "log4qt/logger.h"
#include "log4qt/consoleappender.h"



namespace Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
    LOG4QT_DECLARE_STATIC_LOGGER(logger, Log4Qt::OptionConverter)

    
    
	/**************************************************************************
	 * Class implementation: OptionConverter
	 **************************************************************************/
	
	
    QString OptionConverter::findAndSubst(const Properties &rProperties,
                                          const QString &rKey)
    {
        QString value = rProperties.property(rKey);
        if (value.isNull())
            return value;
        
        const QString begin_subst = QLatin1String("${");
        const QString end_subst = QLatin1String("}");
        const int begin_length = begin_subst.length();
        const int end_length = end_subst.length();
        
        // Don't return a null string, the null string indicates that the 
        // property key does not exist.
        QString result = QLatin1String("");
        
        int i = 0;
        int begin;
        int end;
        while (i < value.length())
        {
            begin = value.indexOf(begin_subst, i);
            if (begin == -1)
            {
                result += value.mid(i);
                i = value.length();
            }
            else
            {
                result += value.mid(i, begin - i);
                end = value.indexOf(end_subst, i + begin_length);
                if (end == -1)
                {
                    LogError e = LOG4QT_ERROR(QT_TR_NOOP("Missing closing bracket for opening bracket at %1. Invalid subsitution in value %2."), 
                                              CONFIGURATOR_INVALID_SUBSTITUTION_ERROR,
                                              "Log4Qt::OptionConverter");
                    e << begin << value;
                    logger()->error(e);
                    return result;
                }
                else
                {
                    result += findAndSubst(rProperties, value.mid(begin + begin_length, end - begin - end_length - 1));
                    i = end + end_length;
                }
            }
        }
        return result;
    }
    
    
    QString OptionConverter::classNameJavaToCpp(const QString &rClassName)
    {
        const QLatin1String java_class_delimiter(".");
        const QLatin1String cpp_class_delimiter("::");
        
        QString result = rClassName;
        return result.replace(java_class_delimiter, cpp_class_delimiter);
    }
    
    
    bool OptionConverter::toBoolean(const QString &rOption, 
                                    bool *p_ok)
    {
        const QLatin1String str_true("true");
        const QLatin1String str_enabled("enabled");
        const QLatin1String str_one("1");
        const QLatin1String str_false("false");
        const QLatin1String str_disabled("disabled");
        const QLatin1String str_zero("0");
        
        if (p_ok)
            *p_ok = true;
        QString s = rOption.trimmed().toLower();
        if (s == str_true || s == str_enabled || s == str_one)
            return true;
        if (s == str_false || s == str_disabled || s == str_zero)
            return false;
        
        if (p_ok)
            *p_ok = false;
        LogError e = LOG4QT_ERROR(QT_TR_NOOP("Invalid option string '%1' for a boolean"), 
                                  CONFIGURATOR_INVALID_OPTION_ERROR,
                                  "Log4Qt::OptionConverter");
        e << rOption;
        logger()->error(e);
        return false;
    }

    
    bool OptionConverter::toBoolean(const QString &rOption, 
                                    bool default_value)
    {
        bool ok;
        bool result = toBoolean(rOption, &ok);
        if (ok)
            return result;
        else
            return default_value;
    }
    
    qint64 OptionConverter::toFileSize(const QString &rOption, 
                                       bool *p_ok)
    {
        // - Search for unit
        // - Convert characters befor unit to int
        // - Error, if
        //   - the conversion failed
        //   - the value < 0
        //   - there is text after the unit characters
        
        if (p_ok)
            *p_ok = false;
        QString s = rOption.trimmed().toLower();
        qint64 f = 1;
        int i;
        i = s.indexOf(QLatin1String("kb"));
        if (i >= 0)
            f = 1024;
        else 
        {
            i = s.indexOf(QLatin1String("mb"));
            if (i >= 0)
                f = 1024 * 1024;
            else 
            {
                i = s.indexOf(QLatin1String("gb"));
                if (i >= 0)
                    f = 1024 * 1024 * 1024;
            }
        }
        if (i < 0)
            i = s.length();
        bool ok;
        qint64 value = s.left(i).toLongLong(&ok);
        if (!ok || value < 0 || s.length() > i + 2)
        {
            LogError e = LOG4QT_ERROR(QT_TR_NOOP("Invalid option string '%1' for a file size"), 
                                      CONFIGURATOR_INVALID_OPTION_ERROR,
                                      "Log4Qt::OptionConverter");
            e << rOption;
            logger()->error(e);
            return 0;
        }
        if (p_ok)
            *p_ok = true;
        return value * f;
    }

    
    int OptionConverter::toInt(const QString &rOption, 
                               bool *p_ok)
    {
        int value = rOption.trimmed().toInt(p_ok);
        if (*p_ok)
            return value;
        
        LogError e = LOG4QT_ERROR(QT_TR_NOOP("Invalid option string '%1' for an integer"), 
                                  CONFIGURATOR_INVALID_OPTION_ERROR,
                                  "Log4Qt::OptionConverter");
        e << rOption;
        logger()->error(e);
        return 0;
    }
    
    qint64 OptionConverter::toQInt64(const QString &rOption, 
      bool *p_ok)
    {
      int value = rOption.trimmed().toLongLong(p_ok);
      if (*p_ok)
        return value;

      LogError e = LOG4QT_ERROR(QT_TR_NOOP("Invalid option string '%1' for an qint64"), 
        CONFIGURATOR_INVALID_OPTION_ERROR,
        "Log4Qt::OptionConverter");
      e << rOption;
      logger()->error(e);
      return 0;
    }
    
    Level OptionConverter::toLevel(const QString &rOption, 
                                   bool *p_ok)
    {
        bool ok;
        Level level = Level::fromString(rOption.toUpper().trimmed(), &ok);
        if (p_ok)
            *p_ok = ok;
        if (ok)
            return level;

        LogError e = LOG4QT_ERROR(QT_TR_NOOP("Invalid option string '%1' for a level"), 
                                  CONFIGURATOR_INVALID_OPTION_ERROR,
                                  "Log4Qt::OptionConverter");
        e << rOption;
        logger()->error(e);
        return level;
    }

    
    Level OptionConverter::toLevel(const QString &rOption, 
                                   const Level &rDefaultValue)
    {
        bool ok;
        Level result = toLevel(rOption, &ok);
        if (ok)
            return result;
        else
            return rDefaultValue;
    }

    
    int OptionConverter::toTarget(const QString &rOption, 
                                  bool *p_ok)
    {
        const QLatin1String java_stdout("system.out"); 
        const QLatin1String cpp_stdout("stdout_target"); 
        const QLatin1String java_stderr("system.err"); 
        const QLatin1String cpp_stderr("stderr_target");
        
        if (p_ok)
            *p_ok = true;
        QString s = rOption.trimmed().toLower();
        if (s == java_stdout || s == cpp_stdout)
            return ConsoleAppender::STDOUT_TARGET;
        if (s == java_stderr || s == cpp_stderr)
            return ConsoleAppender::STDERR_TARGET;

        if (p_ok)
            *p_ok = false;
        LogError e = LOG4QT_ERROR(QT_TR_NOOP("Invalid option string '%1' for a target"), 
                                  CONFIGURATOR_INVALID_OPTION_ERROR,
                                  "Log4Qt::OptionConverter");
        e << rOption;
        logger()->error(e);
        return ConsoleAppender::STDOUT_TARGET;
    }
    
    
    
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/


	
} // namespace Log4Qt
