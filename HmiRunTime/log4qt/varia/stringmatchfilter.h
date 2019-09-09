/******************************************************************************
 *
 * package:     Log4Qt
 * file:        stringmatchfilter.h
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

#ifndef LOG4QT_STRINGMATCHFILTER_H
#define LOG4QT_STRINGMATCHFILTER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/spi/filter.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	/*!
	 * \brief The class StringMatchFilter allows logging events with a 
	 *        specified level.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed.
	 *       See  \ref Ownership "Object ownership" for more details.
	 */
	class StringMatchFilter : public Filter
	{
		Q_OBJECT
		
	    /*!
	     * The property holds if an event is accpeted on a match.
	     *
	     * The default is true.
	     * 
	     * \sa acceptOnMatch(), acceptOnMatch()
	     */
	    Q_PROPERTY(bool acceptOnMatch READ acceptOnMatch WRITE setAcceptOnMatch)
		
	    /*!
	     * The property holds the string to match for this filter.
	     *
	     * \sa stringToMatch(), setStringToMatch()
	     */
	    Q_PROPERTY(QString stringToMatch READ stringToMatch WRITE setStringToMatch)
		
	public:
		StringMatchFilter(QObject *pParent = 0);
	    // StringMatchFilter(const StringMatchFilter &rOther); // Use compiler default
	    // virtual ~StringMatchFilter(); // Use compiler default
	    // StringMatchFilter &operator=(const StringMatchFilter &rOther); // Use compiler default
	
	    bool acceptOnMatch() const;
	    QString stringToMatch() const;
	    void setAcceptOnMatch(bool accept);
	    void setStringToMatch(const QString &rString);
	    
	    virtual Decision decide(const LoggingEvent &rEvent) const;
	
	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %StringMatchFilter(acceptonmatch:true referencecount:1 
	     *                    stringtomatch:"LDAP_STRONG_AUTH_REQUIRED" 
	     *                    next:Log4Qt::LevelMatchFilter(0x3bdd960) )
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
		virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	
	private:
		bool mAcceptOnMatch;
		QString mStringToMatch;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline bool StringMatchFilter::acceptOnMatch() const
	{	return mAcceptOnMatch;	}
	
	inline QString StringMatchFilter::stringToMatch() const
	{	return mStringToMatch;	}
	
	inline void StringMatchFilter::setAcceptOnMatch(bool accept)
	{	mAcceptOnMatch = accept;	}
	
	inline void StringMatchFilter::setStringToMatch(const QString &rString)
	{	mStringToMatch = rString;	}
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::StringMatchFilter, Q_MOVABLE_TYPE); // Use default


#endif // LOG4QT_STRINGMATCHFILTER_H
