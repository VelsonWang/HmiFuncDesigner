/******************************************************************************
 *
 * package:     Log4Qt
 * file:        ttcclayout.h
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

#ifndef LOG4QT_TTCCLAYOUT_H
#define LOG4QT_TTCCLAYOUT_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/layout.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	class LoggingEvent;
	class PatternFormatter;
	
	/*!
	 * \brief The class TTCCLayout outputs the time, thread, logger and nested 
	 *        diagnostic context information of a logging event.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class TTCCLayout : public Layout
	{
	    Q_OBJECT
	
	    /*!
	     * The property holds if the logger name is part of the formatted output.
	     *
	     * The default value is true for including the logger name.
	     *
	     * \sa categoryPrefixing(), setCategoryPrefixing()
	     */
	    Q_PROPERTY(bool categoryPrefixing READ categoryPrefixing WRITE setCategoryPrefixing)
	    
	    /*!
	     * The property holds if the nested context information is part of the 
	     * formatted output.
	     *
	     * The default value it true for including the nested context information.
	     *
	     * \sa contextPrinting(), setContextPrinting()
	     */
	    Q_PROPERTY(bool contextPrinting READ contextPrinting WRITE setContextPrinting)
	
	    /*!
	     * The property holds the date format used by the layout.
	     *
         * The default date format is "TIME_RELATIVE".
	     *
	     * \sa dateFormat(), setDateFormat()
	     */
	    Q_PROPERTY(QString dateFormat READ dateFormat WRITE setDateFormat)
	
		/*!
		 * The property holds if the thread name is part of the formatted output.
		 *
		 * The default value it true for including the thread name.
		 *
		 * \sa threadPrinting(), setThreadPrinting()
		 */
		Q_PROPERTY(bool threadPrinting READ threadPrinting WRITE setThreadPrinting)
		
	public:
	    /*!
	     * The enum DateFormat defines constants for date formats.
	     * 
	     * \sa setDateFormat(DateFormat), DateTime::toString()
	     */
	    enum DateFormat
	    {
	    	/*! The none date format string is "NONE".  */
	        NONE,
	    	/*! 
	    	 * The iso8601 date format string is "ISO8601". The date will be 
	    	 * formatted as yyyy-MM-dd hh:mm:ss.zzz. 
	    	 */
	        ISO8601,
	    	/*!
             * The absolute date format string is "TIME_ABSOLUTE". The date will be
	    	 * formatted as HH:mm:ss.zzz.  
	    	 */
            TIME_ABSOLUTE,
	    	/*!
	    	 * The date date format string is "DATE". The date will be formatted 
	    	 * as MMM YYYY HH:mm:ss.zzzz.
	    	 */
	        DATE,
	    	/*!
             * The relative date format string is "TIME_RELATIVE". The date will be
	    	 * formatted as milliseconds since start of the program.
	    	 */
            TIME_RELATIVE
	    };
	    Q_ENUMS(DateFormat)
	        
	    TTCCLayout(QObject *pParent = 0);
	    TTCCLayout(const QString &rDateFormat,
	               QObject *pParent = 0);
	    
		/*!
		 * Creates a TTCCLayout with the date formar value specified by 
		 * the \a dateFormat constant and the parent \a pParent.
		 */
	    TTCCLayout(DateFormat dateFormat,
	               QObject *pParent = 0);
	    
	    virtual ~TTCCLayout();
	private:
	    TTCCLayout(const TTCCLayout &rOther); // Not implemented
	    TTCCLayout &operator=(const TTCCLayout &rOther); // Not implemented
	
	public:
		bool categoryPrefixing() const;
		bool contextPrinting() const;
	    QString dateFormat() const;
		// JAVA: bool ignoresThrowable() const;
		bool threadPrinting() const;	
	    void setCategoryPrefixing(bool categoryPrefixing);
	    void setContextPrinting(bool contextPrinting);
	    void setDateFormat(const QString &rDateFormat);
	
	    /*!
		 * Sets the date format to the value specified by the \a dateFormat
		 * constant.
		 */
	    void setDateFormat(DateFormat dateFormat);
	    
		// JAVA: setIgnoresThrowable(bool ignoresThrowable);
		void setThreadPrinting(bool threadPrinting);
	    virtual QString format(const LoggingEvent &rEvent);
	
	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %TTCCLayout(name:"TTCC" categoryprefixing:true 
	     *             contextprinting:true dateformat:"ISO8601" 
	     *             referencecount:1 threadprinting:true)
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	    virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	
	private:
		void updatePatternFormatter();
		
	private:
		bool mCategoryPrefixing;
		bool mContextPrinting;
	    QString mDateFormat;
	    bool mThreadPrinting;
	    PatternFormatter *mpPatternFormatter;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline bool TTCCLayout::categoryPrefixing() const
	{	return mCategoryPrefixing;	}
	
	inline bool TTCCLayout::contextPrinting() const
	{	return mContextPrinting;	}
	
	inline QString TTCCLayout::dateFormat() const
	{   return mDateFormat; }
	
	inline bool TTCCLayout::threadPrinting() const
	{	return mThreadPrinting;	}
	
	inline void TTCCLayout::setCategoryPrefixing(bool categoryPrefixing)
	{	mCategoryPrefixing = categoryPrefixing;
		updatePatternFormatter();	}
	
	inline void TTCCLayout::setContextPrinting(bool contextPrinting)
	{	mContextPrinting = contextPrinting;	
		updatePatternFormatter();	}
	
	inline void TTCCLayout::setDateFormat(const QString &rDateFormat)
	{	mDateFormat = rDateFormat;	
		updatePatternFormatter();	}
	
	inline void TTCCLayout::setThreadPrinting(bool threadPrinting)
	{	mThreadPrinting = threadPrinting;
		updatePatternFormatter();	}
	
	
} // namespace Log4Qt
	
	
// Q_DECLARE_TYPEINFO(Log4Qt::TTCCLayout, Q_COMPLEX_TYPE); // Use default
	
	
#endif // LOG4QT_TTCCLAYOUT_H
