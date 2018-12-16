/******************************************************************************
 *
 * package:     Log4Qt
 * file:        levelmatchfilter.h
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

#ifndef LOG4QT_LEVELMATCHFILTER_H
#define LOG4QT_LEVELMATCHFILTER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/spi/filter.h"

#include "log4qt/level.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	/*!
	 * \brief The class LevelMatchFilter allows logging events with a specified 
	 *        level.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class LevelMatchFilter : public Filter
	{
		Q_OBJECT
		
	    /*!
	     * The property holds if an event is accpeted on a match.
	     *
	     * The default is true.
	     * 
	     * \sa acceptOnMatch(), setAcceptOnMatch()
	     */
	    Q_PROPERTY(bool acceptOnMatch READ acceptOnMatch WRITE setAcceptOnMatch)
	
		/*!
	     * The property holds the level to match for this filter.
	     *
	     * The default is Level::NULL_INT.
	     * 
	     * \sa levelToMatch(), setLevelToMatch()
	     */
	    Q_PROPERTY(Level levelToMatch READ levelToMatch WRITE setLevelToMatch)
		
	public:
		LevelMatchFilter(QObject *pParent = 0);
	    // LevelMatchFilter(const LevelMatchFilter &rOther); // Use compiler default
	    // virtual ~LevelMatchFilter(); // Use compiler default
	    // LevelMatchFilter &operator=(const LevelMatchFilter &rOther); // Use compiler default
	
	    bool acceptOnMatch() const;
	    Level levelToMatch() const;
	    void setAcceptOnMatch(bool accept);
	    void setLevelToMatch(Level level);
	    
	    virtual Decision decide(const LoggingEvent &rEvent) const;
	
	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream \a rDebug 
	     * and returns the stream.
	     *
	     * <tt>
	     * %LevelMatchFilter(acceptonmatch:true leveltomatch:"WARN" 
	     *                   next:Log4Qt::DenyAllFilter(0x3bce3a8) 
	     * 					 referencecount:1 )  
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
		virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	
	private:
		bool mAcceptOnMatch;
		Level mLevelToMatch;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline bool LevelMatchFilter::acceptOnMatch() const
	{	return mAcceptOnMatch;	}
	
	inline Level LevelMatchFilter::levelToMatch() const
	{	return mLevelToMatch;	}
	
	inline void LevelMatchFilter::setAcceptOnMatch(bool accept)
	{	mAcceptOnMatch = accept;	}
	
	inline void LevelMatchFilter::setLevelToMatch(Level level)
	{	mLevelToMatch = level;	}
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::LevelMatchFilter, Q_MOVABLE_TYPE); // Use default


#endif // LOG4QT_LEVELMATCHFILTER_H
