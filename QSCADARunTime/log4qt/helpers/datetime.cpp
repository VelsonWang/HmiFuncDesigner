/******************************************************************************
 *
 * package:     Log4Qt
 * file:        datetime.cpp
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


#include "log4qt/helpers/datetime.h"

#include <QtCore/QDebug>
#include "log4qt/helpers/initialisationhelper.h"



namespace Log4Qt
{
	
	    
	/**************************************************************************
	 *Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * Class implementation: DateTime
	 **************************************************************************/
	
	
	QString DateTime::toString(const QString &rFormat) const
	{
	    QString format(rFormat);
	    
	    if (format.isEmpty())
	        return QString();
	    if (!isValid())
	        return QString();
	    if (format == QLatin1String("NONE"))
	        return QString();
	
        if (format == QLatin1String("TIME_RELATIVE"))
	        return QString::number(toMilliSeconds() - InitialisationHelper::startTime());
	    
	    if (format == QLatin1String("ISO8601"))
	        format = QLatin1String("yyyy-MM-dd hh:mm:ss.zzz");
        if (format == QLatin1String("TIME_ABSOLUTE"))
	        format = QLatin1String("HH:mm:ss.zzz");
	    if (format == QLatin1String("DATE"))
	        format = QLatin1String("dd MMM YYYY HH:mm:ss.zzzz");
	    
	    return formatDateTime(format);
	}
		
    
	QString DateTime::formatDateTime(const QString &rFormat) const
	{
	    if (rFormat.isEmpty())
	        return QString();
	    if (!isValid())
	        return QString();
	    
	    const QLatin1Char null('0');
	    const QLatin1Char quote('\'');
	    const QString tokens = QLatin1String("\'dMyhHmszAPapw");
	    const bool am_pm = hasAMPM(rFormat);
	    
	    QString result;
	    QString token;
	    QChar expected = null;
	    
	    QChar c;
	    int i;
	    for (i = 0; i < rFormat.length(); i++)
	    {
	        c = rFormat.at(i);
	        
	        // Handle literal text
	        if (expected == quote)
	        {
	            if (c == quote)
	            {
	                Q_ASSERT_X(i > 0, "DateTime::toString()", "Found quote with status quote at i = 0");
	                if (i > 0 && rFormat.at(i - 1) == quote)
	                    // Second of two quotes
	                    result += quote;
	                expected = null;
	            }
	            else
	                // Next literal character
	                result += c;
	        }
	        else if (c == expected)
	        {
	            // Extend token
	            token += c;
	        }
	        else
	        {
	            // Close last token
	            result += formatToken(token, am_pm);
	            token.clear();
	            expected = null;
	            
	            // Test for valid character
	            if (tokens.indexOf(c) >= 0)
	            {
	                if (c == QLatin1Char('a'))
	                    expected = QLatin1Char('p');
	                else if (c == QLatin1Char('A'))
	                    expected = QLatin1Char('P');
	                else if (c.toLower() == QLatin1Char('p'))
	                    expected = null;
	                else
	                    expected = c;
	                if (c != quote)
	                    token += c;
	            } else
	                result += c;
	        }
	    }
	    
	    result += formatToken(token, am_pm);
	    return result;
	}
	
	
	QString DateTime::formatToken(const QString &rToken, bool am_pm) const
	{
	    if (rToken.isEmpty())
	        return QString();
	    
	    const QChar c = rToken.at(0);
	    QString result;
	    int used;
	    
	    // Qt data format strings 
	    if (rToken.startsWith(QLatin1String("dddd")))
	    {
	        result = QDate::longDayName(date().dayOfWeek());
	        used = 4;
	    } 
	    else if (rToken.startsWith(QLatin1String("ddd")))
	    {
	        result = QDate::shortDayName(date().dayOfWeek());
	        used = 3;
	    } 
	    else if (rToken.startsWith(QLatin1String("dd")))
	    {
	        result = QString::number(date().day()).rightJustified(2, QLatin1Char('0'), true);
	        used = 2;
	    }
	    else if (c == QLatin1Char('d')) 
	    {
	        result = QString::number(date().day());
	        used = 1;
	    }
	    else if (rToken.startsWith(QLatin1String("MMMM"))) 
	    {
	        result = QDate::longMonthName(date().month());
	        used = 4;
	    } 
	    else if (rToken.startsWith(QLatin1String("MMM"))) 
	    {
	        result = QDate::shortMonthName(date().month());
	        used = 3;
	    } 
	    else if (rToken.startsWith(QLatin1String("MM"))) 
	    {
	        result = QString::number(date().month()).rightJustified(2, QLatin1Char('0'), true);
	        used = 2;
	    } 
	    else if (c == QLatin1Char('M')) 
	    {
	        result = QString::number(date().month());
	        used = 1;
	    } 
	    else if (rToken.startsWith(QLatin1String("yyyy"))) 
	    {
	        result = QString::number(date().year());
	        used = 4;
	    } 
	    else if (rToken.startsWith(QLatin1String("yy"))) 
	    {
	        result = QString::number(date().year() % 100).rightJustified(2, QLatin1Char('0'), true);
	        used = 2;
	    }
	
	    // Qt time format strings 
	    else if (rToken.startsWith(QLatin1String("hh")) || rToken.startsWith(QLatin1String("HH"))) 
	    {
	        int hour = time().hour();
	        if (am_pm && c == QLatin1Char('h') && hour > 12)
	            hour -= 12;
	        result = QString::number(hour).rightJustified(2, QLatin1Char('0'), true);
	        used = 2;
	    } 
	    else if (c == QLatin1Char('h') || c == QLatin1Char('H')) 
	    {
	        int hour = time().hour();
	        if (am_pm && c == QLatin1Char('h') && hour > 12)
	            hour -= 12;
	        result = QString::number(hour);
	        used = 2;
	    } 
	    else if (rToken.startsWith(QLatin1String("mm")))
	    {
	        result = QString::number(time().minute()).rightJustified(2, QLatin1Char('0'), true);
	        used = 2;
	    } 
	    else if (c == (QLatin1Char('m'))) 
	    {
	        result = QString::number(time().minute());
	        used = 1;
	    } 
	    else if (rToken.startsWith(QLatin1String("ss"))) 
	    {
	        result = QString::number(time().second()).rightJustified(2, QLatin1Char('0'), true);
	        used = 2;
	    }
	    else if (c == QLatin1Char('s')) 
	    {
	        result = QString::number(time().second());
	        used = 1;
	    } 
	    else if (rToken.startsWith(QLatin1String("zzz"))) 
	    {
	        result = QString::number(time().msec()).rightJustified(3, QLatin1Char('0'), true);
	        used = 3;
	    } 
	    else if (c == QLatin1Char('z')) 
	    {
	        result = QString::number(time().msec());
	        used = 1;
	    }
	    else if (c.toLower() == QLatin1Char('a'))
	    {
	        bool is_lower = c == QLatin1Char('a');
	        if (time().hour() < 12)
	            result = QLatin1String("AM");
	        else 
	            result = QLatin1String("PM");
	        if (is_lower)
	            result = result.toLower();
	        if (rToken.size() > 1 && 
	            ((is_lower && rToken.at(1) == QLatin1Char('p')) ||
	             (!is_lower && rToken.at(1) == QLatin1Char('P')))
	            )
	            used = 2;
	        else
	            used = 1;
	    }
	
	    // Extension for week number
	    else if (rToken.startsWith(QLatin1String("ww"))) 
	    {
	        result = QString::number(date().weekNumber()).rightJustified(2, QLatin1Char('0'), true);
	        used = 2;
	    }
	    else if (c == QLatin1Char('w')) 
	    {
	        result = QString::number(date().weekNumber());
	        used = 1;
	    }
	
	    if (used)
	        return result + formatToken(rToken.mid(used), am_pm);
	    else
	        return result;
	}
	
	
	bool DateTime::hasAMPM(const QString &rToken)
	{
	    bool in_literal = false;
	    QChar c;
	    int i;
	    for (i = 0; i < rToken.length(); i++)
	    {
	        c = rToken.at(i);
	        if (c == QLatin1Char('\''))
	            in_literal = !in_literal;
	        else if (!in_literal && c.toLower() == QLatin1Char('a'))
	            return true;
	    }
	    return false;
	}
	
	
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
	
} // namespace Log4Qt
