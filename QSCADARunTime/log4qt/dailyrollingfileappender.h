/******************************************************************************
 *
 * package:     Log4Qt
 * file:        dailyrollingfileappender.h
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

#ifndef LOG4QT_DAILYROLLINGFILEAPPENDER_H
#define LOG4QT_DAILYROLLINGFILEAPPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/fileappender.h"

#include <QtCore/QDateTime>


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	/*!
	 * \brief The class DailyRollingFileAppender extends FileAppender so that the 
	 *        underlying file is rolled over at a specified frequency.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. See
	 *       \ref Ownership "Object ownership" for more details.
	 */
	class DailyRollingFileAppender : public FileAppender
	{
	    Q_OBJECT
	    
	    /*!
	     * The property holds the date pattern used by the appender.
	     *
	     * The default is DAILY_ROLLOVER for rollover at midnight each day.
	     *
	     * \sa datePattern(), setDatePattern()
	     */
	    Q_PROPERTY(QString datePattern READ datePattern WRITE setDatePattern)
	 
	public:
	    /*!
	     * The enum DatePattern defines constants for date patterns.
	     * 
	     * \sa setDatePattern(DatePattern)
	     */
	    enum DatePattern
	    {
	    	/*! The minutely date pattern string is "'.'yyyy-MM-dd-hh-mm". */
	        MINUTELY_ROLLOVER = 0,
	    	/*! The hourly date pattern string is "'.'yyyy-MM-dd-hh". */
	        HOURLY_ROLLOVER,
	    	/*! The half-daily date pattern string is "'.'yyyy-MM-dd-a". */
	        HALFDAILY_ROLLOVER,
	    	/*! The daily date pattern string is "'.'yyyy-MM-dd". */
	        DAILY_ROLLOVER,
	    	/*! The weekly date pattern string is "'.'yyyy-ww". */
	        WEEKLY_ROLLOVER,
	    	/*! The monthly date pattern string is "'.'yyyy-MM". */
	        MONTHLY_ROLLOVER
	    };
	    Q_ENUMS(DatePattern)
	
	    DailyRollingFileAppender(QObject *pParent = 0);
	    DailyRollingFileAppender(Layout *pLayout, 
	                             const QString &rFileName, 
	                             const QString &rDatePattern, 
	                             QObject *pParent = 0);
	    virtual ~DailyRollingFileAppender();
	private:
	    DailyRollingFileAppender(const DailyRollingFileAppender &rOther); // Not implemented
	    DailyRollingFileAppender &operator=(const DailyRollingFileAppender &rOther); // Not implemented
	        
	public:
	    QString datePattern() const;
	
	    /*!
		 * Sets the datePattern to the value specified by the \a datePattern
		 * constant.
		 */
	    void setDatePattern(DatePattern datePattern);
	    
	    void setDatePattern(const QString &rDatePattern);
	
	    virtual void activateOptions();
	    
	protected:
	    virtual void append(const LoggingEvent &rEvent);
	    
	    /*!
	     * Tests if all entry conditions for using append() in this class are 
	     * met.
	     * 
	     * If a conditions is not met, an error is logged and the function 
	     * returns false. Otherwise the result of 
	     * FileAppender::checkEntryConditions() is returned.
	     * 
	     * The checked conditions are:
	     * - A valid pattern has been set (APPENDER_USE_INVALID_PATTERN_ERROR)
	     * 
	     * The function is called as part of the checkEntryConditions() chain 
	     * started by AppenderSkeleton::doAppend().
	     * 
	     * \sa AppenderSkeleton::doAppend(), 
	     *     AppenderSkeleton::checkEntryConditions()
	     */
	    virtual bool checkEntryConditions() const;

	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %DailyRollingFileAppender(name:"DRFA" activedatepattern:"'.'yyyy-MM-dd-hh-mm" 
	     *                           appendfile:false bufferedio:true 
	     *                           datepattern:"'.'yyyy-MM-dd-hh-mm" 
	     *                           encoding:"" frequency:"MINUTELY_ROLLOVER" 
	     *                           file:"/log.txt" filter:0x0 immediateflush:true 
	     *                           isactive:true isclosed:false layout:"TTCC" 
	     *                           referencecount:1 
	     *                           rollovertime:QDateTime("Mon Oct 22 05:23:00 2007") 
	     *                           threshold: "NULL" writer: 0x0 )  
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	    virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	
	private:
	    void computeFrequency();
	    void computeRollOverTime();
	    QString frequencyToString() const;
	    void rollOver();
	    
	private:
	    QString mDatePattern;
	    DatePattern mFrequency;
	    QString mActiveDatePattern;
	    QDateTime mRollOverTime;
	    QString mRollOverSuffix;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline QString DailyRollingFileAppender::datePattern() const
	{   QMutexLocker locker(&mObjectGuard);
	    return mDatePattern;   }
	
	inline void DailyRollingFileAppender::setDatePattern(const QString &rDatePattern)
	{   QMutexLocker locker(&mObjectGuard);
	    mDatePattern = rDatePattern;    }
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::DailyRollingFileAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_DAILYROLLINGFILEAPPENDER_H
