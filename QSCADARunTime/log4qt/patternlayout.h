/******************************************************************************
 *
 * package:     Log4Qt
 * file:        patternlayout.h
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

#ifndef LOG4QT_PATTERNLAYOUT_H
#define LOG4QT_PATTERNLAYOUT_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/layout.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/


namespace Log4Qt
{
	
	class PatternFormatter;
	
	/*!
	 * \brief The class PatternLayout outputs a logging event based on a 
	 *        pattern string. 
	 * 
	 * \note The ownership and lifetime of objects of this class are managed.
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	 class PatternLayout : public Layout
	{
		Q_OBJECT
	
	    /*!
	     * The property holds the conversion pattern used by the appender.
	     *
	     * The default is "%m%n".
	     *
	     * \sa conversionPattern(), setConversionPattern()
	     */
	    Q_PROPERTY(QString conversionPattern READ conversionPattern WRITE setConversionPattern)
	
	public:
	    /*!
	     * The enum ConversionPattern defines constants for pattern strings.
	     * 
	     * \sa setConversionPattern(ConversionPattern);
	     */
	    enum ConversionPattern
	    {
	    	/*! The default conversion pattern string is "%m,%n". */
	    	DEFAULT_CONVERSION_PATTERN,
	    	/*!
	    	 * The ttcc conversion pattern string is 
	    	 * "%r [%t] %p %c %x - %m%n".
	    	 */
	    	TTCC_CONVERSION_PATTERN,
	    };
	    Q_ENUMS(ConversionPattern)
	    
	    PatternLayout(QObject *pParent = 0);
	    PatternLayout(const QString &rPattern, 
	                  QObject *pParent = 0);
	    
		/*!
		 * Creates a PatternLayout with the conversion pattern value specified
		 * by the \a conversionPattern constant.
		 */
	    PatternLayout(ConversionPattern conversionPattern, 
	                  QObject *pParent = 0);
	    
	    virtual ~PatternLayout();
	private:
	    PatternLayout(const PatternLayout &rOther); // Not implemented
	    PatternLayout &operator=(const PatternLayout &rOther); // Not implemented
	        
	public:
		QString conversionPattern() const;
		void setConversionPattern(const QString &rPattern);
		
		/*!
		 * Sets the conversion pattern to the value specified by the 
		 * \a conversionPattern constant.
		 */
		void setConversionPattern(ConversionPattern conversionPattern);
		
	    virtual QString format(const LoggingEvent &rEvent);
	    
	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %PatternLayout(name:"PL" pattern:"%r [%t] %p %c %x - %m%n"
	     *                "referencecount:3") 
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	
	private:
		void updatePatternFormatter();
		
	private:
		QString mPattern;
		PatternFormatter *mpPatternFormatter;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline QString PatternLayout::conversionPattern() const
	{	return PatternLayout::mPattern;	}
	
	inline void PatternLayout::setConversionPattern(const QString &rPattern)
	{	mPattern = rPattern;	
		updatePatternFormatter();	}
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::PatternLayout, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_PATTERNLAYOUT_H
