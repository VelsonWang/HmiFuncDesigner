/******************************************************************************
 *
 * package:     Log4Qt
 * file:        dailyrollingfileappender.cpp
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


#include "log4qt/dailyrollingfileappender.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QMetaEnum>
#include <QtCore/QTextCodec>
#include "log4qt/helpers/datetime.h"
#include "log4qt/layout.h"
#include "log4qt/loggingevent.h"



namespace Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * Class implementation: DailyRollingFileAppender
	 **************************************************************************/
	
	
	DailyRollingFileAppender::DailyRollingFileAppender(QObject *pParent) :
	    FileAppender(pParent),
	    mDatePattern()
	{
	    setDatePattern(DAILY_ROLLOVER);
	}
	
	
	DailyRollingFileAppender::DailyRollingFileAppender(Layout *pLayout, 
	                                                   const QString &rFileName, 
	                                                   const QString &rDatePattern, 
	                                                   QObject *pParent) :
	    FileAppender(pLayout, rFileName, pParent),
	    mDatePattern()
	{
	    setDatePattern(rDatePattern);
	}
	              	   
	
	DailyRollingFileAppender::~DailyRollingFileAppender()
	{
	    close();
	}
	
	
	void DailyRollingFileAppender::setDatePattern(DatePattern datePattern)
	{
	    switch (datePattern)
	    {
	        case MINUTELY_ROLLOVER:
	            setDatePattern(QLatin1String("'.'yyyy-MM-dd-hh-mm"));
	            break;
	        case HOURLY_ROLLOVER:
	            setDatePattern(QLatin1String("'.'yyyy-MM-dd-hh"));
	            break;
	        case HALFDAILY_ROLLOVER:
	            setDatePattern(QLatin1String("'.'yyyy-MM-dd-a"));
	            break;
	        case DAILY_ROLLOVER:
	            setDatePattern(QLatin1String("'.'yyyy-MM-dd"));
	            break;
	        case WEEKLY_ROLLOVER:
	            setDatePattern(QLatin1String("'.'yyyy-ww"));
	            break;            
	        case MONTHLY_ROLLOVER:
	            setDatePattern(QLatin1String("'.'yyyy-MM"));
	            break;
	        default:
	            Q_ASSERT_X(false, "DailyRollingFileAppender::setDatePattern()", "Invalid datePattern constant");
	            setDatePattern(DAILY_ROLLOVER);
	    };
	}
	
	
	void DailyRollingFileAppender::activateOptions()
	{
	    QMutexLocker locker(&mObjectGuard);
	
	    computeFrequency();
	    if (!mActiveDatePattern.isEmpty())
	    {
	        computeRollOverTime();
	        FileAppender::activateOptions();
	    }
	}
	
	
	void DailyRollingFileAppender::append(const LoggingEvent &rEvent)
	{
	    // Q_ASSERT_X(, "DailyRollingFileAppender::append()", "Lock must be held by caller")
	    
	    if (QDateTime::currentDateTime() > mRollOverTime)
	        rollOver();
	    FileAppender::append(rEvent);
	}
	    
	    
	bool DailyRollingFileAppender::checkEntryConditions() const
	{    
	    // Q_ASSERT_X(, "DailyRollingFileAppender::checkEntryConditions()", "Lock must be held by caller")
	    
	    if (mActiveDatePattern.isEmpty())
	    {
	        LogError e = LOG4QT_QCLASS_ERROR(QT_TR_NOOP("Use of appender '%1' without having a valid date pattern set"),
                                             APPENDER_USE_INVALID_PATTERN_ERROR);
	        e << name();
	        logger()->error(e);
	        return false;
	    }
	    
	    return FileAppender::checkEntryConditions();
	}
	
	
#ifndef QT_NO_DEBUG_STREAM
	QDebug DailyRollingFileAppender::debug(QDebug &rDebug) const
	{
	    QString layout_name;
	    if (layout())
	        layout_name = layout()->name();
	    QString codec_name;
	    if (encoding())
	        codec_name = QLatin1String(encoding()->name());
	    
	    rDebug.nospace() << "DailyRollingFileAppender(" 
	        << "name:" << name() << " "
	        << "activedatepattern:" << mActiveDatePattern << " "
	        << "appendfile:" << appendFile() << " " 
	        << "bufferedio:" << bufferedIo() << " "
	        << "datepattern:" << datePattern() << " "
	        << "encoding:" << codec_name << " "
	        << "frequency:" << frequencyToString() << " "
	        << "file:" << file() << " "
	        << "filter:" << firstFilter() << " "        
	        << "immediateflush:" << immediateFlush() << " "
	        << "isactive:" << isActive() << " "
	        << "isclosed:" << isClosed() << " "
	        << "layout:" << layout_name << " "
	        << "referencecount:" << referenceCount() << " "
	        << "rollovertime:" << mRollOverTime
	        << "threshold:" << threshold().toString()
	        << "writer:" << writer()
	        << ")";
	    return rDebug.space();    
	}
#endif // QT_NO_DEBUG_STREAM
	
	
	void DailyRollingFileAppender::computeFrequency()
	{
	    // Q_ASSERT_X(, "DailyRollingFileAppender::computeFrequency()", "Lock must be held by caller")
	
	    const DateTime start_time(QDate(1999, 1, 1), QTime(0, 0));
	    const QString start_string = start_time.toString(mDatePattern);
	    mActiveDatePattern.clear();
	    
	    if (start_string != static_cast<DateTime>(start_time.addSecs(60)).toString(mDatePattern))
	        mFrequency = MINUTELY_ROLLOVER;
	    else if (start_string != static_cast<DateTime>(start_time.addSecs(60 * 60)).toString(mDatePattern))
	        mFrequency = HOURLY_ROLLOVER;
	    else if (start_string != static_cast<DateTime>(start_time.addSecs(60 * 60 * 12)).toString(mDatePattern))
	        mFrequency = HALFDAILY_ROLLOVER;
	    else if (start_string != static_cast<DateTime>(start_time.addDays(1)).toString(mDatePattern))
	        mFrequency = DAILY_ROLLOVER;
	    else if (start_string != static_cast<DateTime>(start_time.addDays(7)).toString(mDatePattern))
	        mFrequency = WEEKLY_ROLLOVER;
	    else if (start_string != static_cast<DateTime>(start_time.addMonths(1)).toString(mDatePattern))
	        mFrequency = MONTHLY_ROLLOVER;
	    else
	    {
	        LogError e = LOG4QT_QCLASS_ERROR(QT_TR_NOOP("The pattern '%1' does not specify a frequency for appender '%2'"),
                                             APPENDER_INVALID_PATTERN_ERROR);
	        e << mDatePattern << name();
	        logger()->error(e);
	        return;
	    }
	    
	    mActiveDatePattern = mDatePattern;
	    logger()->trace("Frequency set to %2 using date pattern %1",
	                    mActiveDatePattern,
	                    frequencyToString());
	}
	
	
	void DailyRollingFileAppender::computeRollOverTime()
	{
	    // Q_ASSERT_X(, "DailyRollingFileAppender::computeRollOverTime()", "Lock must be held by caller")
	    Q_ASSERT_X(!mActiveDatePattern.isEmpty(), "DailyRollingFileAppender::computeRollOverTime()", "No active date pattern");
	
	    QDateTime now = QDateTime::currentDateTime();
	    QDate now_date = now.date();
	    QTime now_time = now.time();
	    QDateTime start;
	    
	    switch (mFrequency)
	    {
	        case MINUTELY_ROLLOVER:
	            {
	                start = QDateTime(now_date,
	                                  QTime(now_time.hour(),
	                                        now_time.minute(),
	                                        0, 0));
	                mRollOverTime = start.addSecs(60);
	            } 
	            break;
	        case HOURLY_ROLLOVER:
	            {
	                start = QDateTime(now_date,
	                                  QTime(now_time.hour(),
	                                        0, 0, 0));
	                mRollOverTime = start.addSecs(60*60);
	            }
	            break;
	        case HALFDAILY_ROLLOVER:
	            {
	                int hour = now_time.hour();
	                if (hour >=  12)
	                    hour = 12;
	                else
	                    hour = 0;
	                start = QDateTime(now_date,
	                                  QTime(hour, 0, 0, 0));
	                mRollOverTime = start.addSecs(60*60*12);
	            }
	            break;
	        case DAILY_ROLLOVER:
	            {
	                start = QDateTime(now_date,
	                                  QTime(0, 0, 0, 0));
	                mRollOverTime = start.addDays(1);
	            }
	            break;
	        case WEEKLY_ROLLOVER:
	            { 
	                // QT numbers the week days 1..7. The week starts on Monday.
	                // Change it to being numbered 0..6, starting with Sunday.
	                int day = now_date.dayOfWeek();
	                if (day == Qt::Sunday)
	                    day = 0;
	                start = QDateTime(now_date,
	                                  QTime(0, 0, 0, 0)).addDays(-1 * day);
	                mRollOverTime = start.addDays(7);
	            }
	            break;
	        case MONTHLY_ROLLOVER:
	            {
	                start = QDateTime(QDate(now_date.year(),
	                                        now_date.month(),
	                                        1),
	                                  QTime(0, 0, 0, 0));
	                mRollOverTime = start.addMonths(1);
	            }
	            break;
	        default:
	            Q_ASSERT_X(false, "DailyRollingFileAppender::computeInterval()", "Invalid datePattern constant");
	            mRollOverTime = QDateTime::fromTime_t(0);
	    }
	    
	    mRollOverSuffix = static_cast<DateTime>(start).toString(mActiveDatePattern);
	    Q_ASSERT_X(static_cast<DateTime>(now).toString(mActiveDatePattern) == mRollOverSuffix, 
	               "DailyRollingFileAppender::computeRollOverTime()", "File name changes within interval");
	    Q_ASSERT_X(mRollOverSuffix != static_cast<DateTime>(mRollOverTime).toString(mActiveDatePattern),
	               "DailyRollingFileAppender::computeRollOverTime()", "File name does not change with rollover");
	    
	    logger()->trace("Computing roll over time from %1: The interval start time is %2. The roll over time is %3",
	                    now,
	                    start,
	                    mRollOverTime);
	}
	
	
	QString DailyRollingFileAppender::frequencyToString() const
	{
	    QMetaEnum meta_enum = metaObject()->enumerator(metaObject()->indexOfEnumerator("DatePattern"));
	    return QLatin1String(meta_enum.valueToKey(mFrequency));
	}
	
	
	void DailyRollingFileAppender::rollOver()
	{
	    // Q_ASSERT_X(, "DailyRollingFileAppender::rollOver()", "Lock must be held by caller")
	    Q_ASSERT_X(!mActiveDatePattern.isEmpty(), "DailyRollingFileAppender::rollOver()", "No active date pattern");
	
	    QString roll_over_suffix = mRollOverSuffix;
	    computeRollOverTime();
	    if (roll_over_suffix == mRollOverSuffix)
	        return;
	
	    closeFile();
	
	    QString target_file_name = file() + roll_over_suffix;
	    QFile f(target_file_name);
	    if (f.exists() && !removeFile(f))
	        return;
	    f.setFileName(file());
	    if (!renameFile(f, target_file_name))
            return;
	    openFile();
	}
	
		
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
} // namespace Log4Qt
