/******************************************************************************
 *
 * package:     Log4Qt
 * file:        denyallfilter.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * changes      Feb 2009, Martin Heinrich
 *              - Fixed a compile error on VS 2008 by using Q_UNUSED(&rEvent) 
 *                instead of Q_UNUSED(rEvent)
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

#ifndef LOG4QT_DENYALLFILTER_H
#define LOG4QT_DENYALLFILTER_H


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
	 * \brief The class DenyAllFilter drops all logging events
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class DenyAllFilter : public Filter
	{
		Q_OBJECT
		
	public:
	    DenyAllFilter(QObject *pParent = 0);
	    // DenyAllFilter(const DenyAllFilter &rOther); // Use compiler default
	    // virtual ~DenyAllFilter(); // Use compiler default
	    // DenyAllFilter &operator=(const DenyAllFilter &rOther); // Use compiler default
	
	    virtual Decision decide(const LoggingEvent &rEvent) const;
	
	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream \a rDebug 
	     * and returns the stream.
	     *
	     * <tt>
	     * %DenyAllFilter(next:QObject(0x0) referencecount:1 ) 
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
		virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	};
	
	
	/*************************************************************************
	 * Operators, Helper
	 *************************************************************************/
	
	
	/*************************************************************************
	 * Inline
	 *************************************************************************/
	
	inline DenyAllFilter::DenyAllFilter(QObject *pParent) :
	    Filter(pParent)
	{}
	
	inline Filter::Decision DenyAllFilter::decide(const LoggingEvent &rEvent) const
	{	Q_UNUSED(&rEvent); return Filter::DENY;	}
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::DenyAllFilter, Q_MOVABLE_TYPE); // Use default


#endif // LOG4QT_DENYALLFILTER_H
