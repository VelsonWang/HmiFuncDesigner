/******************************************************************************
 *
 * package:     Log4Qt
 * file:        patternformatter.h
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

#ifndef LOG4QT_PATTERNFORMATTER_H
#define LOG4QT_PATTERNFORMATTER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QList>
#include <QtCore/QString>


/******************************************************************************
 * Declarations
 ******************************************************************************/


namespace Log4Qt
{
	
	class FormattingInfo;
	class PatternConverter;
	class LoggingEvent;
	
	/*!
	 * \brief The class PatternFormatter formats a logging event based on a 
	 *        pattern string.
	 * 
	 * The class PatternFormatter formats a LoggingEvent base on a pattern 
	 * string. It is used by the patternLayout and TTCCLayout class to 
	 * implement the formatting.
	 * 
	 * On object construction the provided patterns tring is parsed. Based on 
	 * the information found a chain of PatternConverter is created. Each 
	 * PatternConverter handles a certain member of a LoggingEvent.
	 * 
	 * \sa PatternLayout::format()
	 * \sa TTCCLayout::format()
	 */
	class PatternFormatter
	{
	public:
		/*!
		 * Creates a PatternFormatter using a the specified \a rPattern.
		 */
		PatternFormatter(const QString &rPattern);
		
		/*!
		 * Destroys the PatternFormatter and all PatternConverter.
		 */
		virtual ~PatternFormatter();
	
	private:
		PatternFormatter(const PatternFormatter &rOther); // Not implemented
		PatternFormatter &operator=(const PatternFormatter &rOther); // Not implemented
	
	public:
		/*!
		 * Formats the given \a rLoggingEvent using the chain of 
		 * PatternConverter created during construction from the specified 
		 * pattern.
		 */
		QString format(const LoggingEvent &rLoggingEvent) const;
		
	private:
		/*!
		 * If the character \a rDigit is a digit the digit is added to the 
		 * integer \a rValue and the function returns true. Otherwise the 
		 * function returns false.
		 * 
		 * The function adds the digit by multiplying the existing value 
		 * with ten and adding the numerical value of the digit. If the 
		 * maximum integer value would be exceeded by the operation 
		 * \a rValue is set to INT_MAX.
		 */
		bool addDigit(const QChar &rDigit, 
	                  int &rValue);
		
		/*!
		 * Creates a PatternConverter based on the specified conversion 
		 * character \a rChar, the formatting information 
		 * \a rFormattingInfo and the option \a rOption. 
		 * 
		 * The PatternConverter converter is appended to the list of 
		 * PatternConverters.
		 */
		void createConverter(const QChar &rChar, 
	                         const FormattingInfo &rFormattingInfo,  
	                         const QString &rOption = QString());
		
		/*!
		 * Creates a LiteralPatternConverter with the string literal 
		 * \a rLiteral.
		 * 
		 * The PatternConverter converter is appended to the list of 
		 * PatternConverters.
		 */
		void createLiteralConverter(const QString &rLiteral);
		
		/*!
		 * Parses the pattern string specified on construction and creates 
		 * PatternConverter according to it.
		 */
		void parse();	
	
		/*! 
		 * Parses an integer option from an option string. If the string is 
		 * not a valid integer or the integer value is less then zero, zero 
		 * is returned. Returns the end of line seperator for the operating 
		 * system.
		 */
		int parseIntegerOption(const QString &rOption);
		
	private:
		const QString mIgnoreCharacters;
		const QString mConversionCharacters;
		const QString mOptionCharacters;
		QString mPattern;
		QList<PatternConverter *> mPatternConverters;
		
	    // Needs to be friend to access internal data
		friend QDebug operator<<(QDebug, const PatternFormatter &rPatternFormatter);
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates PatternFormatter
	 * 
	 * Writes all object member variables to the given debug stream \a rDebug and
	 * returns the stream.
	 *
	 * <tt>
	 * %PatternFormatter(pattern:"%r [%t] %p %c %x - %m%n" 
	 *                   converters:(
     *                   DatePatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) format: "TIME_RELATIVE" )  ,
	 *                   LiteralPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) literal: " [" )  , 
	 *                   BasicPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) type: "THREAD_CONVERTER" )  ,
	 *                   LiteralPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) literal: "] " )  ,
	 *                   BasicPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) type: "LEVEL_CONVERTER" )  ,
	 *                   LiteralPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) literal: " " )  ,
	 *                   LoggerPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) precision: 0 )  , 
	 *                   LiteralPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) literal: " " )  ,
	 *                   BasicPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) type: "NDC_CONVERTER" )  ,
	 *                   LiteralPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) literal: " - " ) ,
	 *                   BasicPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) type: "MESSAGE_CONVERTER" )  ,
	 *                   LiteralPatternConverter(FormattingInfo(min:"0" max:"INT_MAX" left:false) literal: "" ) ) )
	 * </tt>
	 * \sa QDebug
	 */
	QDebug operator<<(QDebug debug, 
                      const PatternFormatter &rPatternFormatter);
#endif // QT_NO_DEBUG_STREAM
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	
} // namespace Log4Qt


Q_DECLARE_TYPEINFO(Log4Qt::PatternFormatter, Q_MOVABLE_TYPE);


#endif // LOG4QT_PATTERNFORMATTER_H
