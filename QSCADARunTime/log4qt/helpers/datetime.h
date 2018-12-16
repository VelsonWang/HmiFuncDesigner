/******************************************************************************
 *
 * package:     Log4Qt
 * file:        datetime.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * changes:		Sep 2008, Martin Heinrich:
 * 				- Resolved compilation problem with Microsoft Visual Studio 2005
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

#ifndef LOG4QT_HELPERS_DATETIME_H
#define LOG4QT_HELPERS_DATETIME_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QDateTime>


/******************************************************************************
 * Declarations
 ******************************************************************************/


namespace Log4Qt
{
	
	/*!
	 * \brief The class DateTime provides extended functionality for QDateTime.
	 *
	 * The class DateTime implements additional formatting options for 
	 * toString() and provides conversion functions from and to milliseconds.
	 */
	class DateTime : public QDateTime
	{
	public:	
	    /*!
	     * Constructs a null date time.
	     *
	     * \sa QDateTime::QDateTime()
	     */
	    DateTime();
	    
	    // DateTime(const DateTime &rOther); // Use compiler default
	    
	    /*!
	     * Constructs a copy of another QDateTime.
	     *
	     * \sa QDateTime::QDateTime(const QDateTime &rOther)
	     */
	    DateTime(const QDateTime &rOther);
	    
	    /*!
	     * Constructs a datetime with the given \a rDate and \a rTime, using 
	     * the time specification defined by \a timeSpec.
	     *
	     * \sa QDateTime::QDateTime(const QDate &rDate, const QTime &rTime, 
	     *     Qt::TimeSpec timeSpec = Qt::LocalTime)
	     */
	    DateTime(const QDate &rDate, 
	             const QTime &rTime, 
	             Qt::TimeSpec timeSpec = Qt::LocalTime);
	    
	    // virtual ~DateTime(); // Use compiler default
	    
	    /*!
	     * Assigns \a rOther to this DateTime and returns a reference to it.
	     */
	    DateTime &operator=(const DateTime &rOther);
	  
	    /*!
	     * Returns the datetime as the number of milliseconds that have passed
	     * since 1970-01-01T00:00:00,000, Coordinated Universal Time (Qt::UTC).
	     *
	     * \sa QDateTime::toTime_t() 
	     */
	    qint64 toMilliSeconds() const;
	    
	    /*!
	     * Returns the datetime as a string. The \a rFormat parameter 
	     * determines the format of the result string.
	     *
	     * In addition to the expressions of QDateTime::toString(const QString 
	     * &rFormat) the following expression can be used.
	     *
	     * <table align="center" border="1" cellpadding="2" cellspacing="0" bordercolor="#84b0c7">
	     * <tr bgcolor="#d5e1e8">
	     * <th width="20%"> Expression </th>
	     * <th> Output </th>
	     * </tr><tr>
	     * <td> w </td>
	     * <td> the week of the year as number without a leading zero (1 to 53) </td>
	     * </tr><tr bgcolor="#ffffff">
	     * <td> ww </td>
	     * <td> the week of the year as number with a leading zero (01 to 53) </td>
	     * </tr>
	     * </table>
	     *
	     * Alternatively the \a rFormat parameter can specify one of the 
	     * following strings.
	     *
	     * <table align="center" border="1" cellpadding="2" cellspacing="0" bordercolor="#84b0c7">
	     * <tr bgcolor="#d5e1e8">
	     * <th width="20%"> String </th>
	     * <th> Format </th>
	     * </tr><tr>
         * <td> TIME_ABSOLUTE </td>
	     * <td> uses the format HH:mm:ss.zzz </td>
	     * </tr><tr bgcolor="#ffffff">
	     * <td> DATE </td>     
	     * <td> uses the format dd MMM YYYY HH:mm:ss.zzzz </td>
	     * </tr><tr>
	     * <td> ISO8601 </td>
	     * <td> uses the format yyyy-MM-dd hh:mm:ss.zzz </td>
	     * </tr><tr bgcolor="#ffffff">
	     * <td> NONE </td>
	     * <td> uses an empty string as format </td>
	     * </tr><tr bgcolor="#ffffff">
         * <td> TIME_RELATIVE </td>
	     * <td> returns the milliseconds since start of the program</td>
	     * </tr>
	     * </table>
	     *
	     * \sa QDateTime::toString(const QString &rFormat)
	     */
	    QString toString(const QString &rFormat) const;
	
	    /*!
	     * Returns the current datetime, as reported by the system clock, in 
	     * the local time zone.
	     *
	     * \sa QDateTime::currentDateTime()
	     */
	    static DateTime currentDateTime();
	    
	    /*!
	     * Returns a datetime whose date and time are the number of 
	     * milliseconds that have passed since 1970-01-01T00:00:00, 
	     * Coordinated Universal Time (Qt::UTC).
	     * 
	     * \sa QDateTime::fromTime_t(uint seconds)
	     */
	    static DateTime fromMilliSeconds(qint64 milliSeconds);
	
	private:
	    QString formatDateTime(const QString &rFormat) const;
	    QString formatToken(const QString &rToken, bool am_pm) const;
	    static bool hasAMPM(const QString &rFormat);
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline DateTime::DateTime() : QDateTime()
	{}
	
	inline DateTime::DateTime(const QDateTime &rOther) : QDateTime(rOther)
	{}
	
	inline DateTime::DateTime(const QDate &rDate, 
	                          const QTime &rTime, 
	                          Qt::TimeSpec timeSpec) :
	    QDateTime(rDate, rTime, timeSpec)
	{}
	
	inline DateTime &DateTime::operator=(const DateTime &rOther)
	{   QDateTime::operator=(rOther); return *this; }
	
	inline qint64 DateTime::toMilliSeconds() const
	{   return (qint64)1000 * toTime_t() + time().msec();   }
	
	inline DateTime DateTime::currentDateTime()
	{   return DateTime(QDateTime::currentDateTime());    }
	
	inline DateTime DateTime::fromMilliSeconds(qint64 milliSeconds)
	{   return DateTime(QDateTime::fromTime_t(milliSeconds / 1000).addMSecs(milliSeconds % 1000));    }
	
	
} // namespace Log4Qt


Q_DECLARE_TYPEINFO(Log4Qt::DateTime, Q_MOVABLE_TYPE);


#endif // LOG4QT_HELPERS_DATETIME_H
