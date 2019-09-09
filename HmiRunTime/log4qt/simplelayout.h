/******************************************************************************
 *
 * package:     Log4Qt
 * file:        simplelayout.h
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

#ifndef LOG4QT_SIMPLELAYOUT_H
#define LOG4QT_SIMPLELAYOUT_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/layout.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/


namespace Log4Qt
{
	
	/*!
	 * \brief The class SimpleLayout outputs the level and message of a logging
	 *        event.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	 class SimpleLayout : public Layout
	{
	    Q_OBJECT
	
	public:
	    SimpleLayout(QObject *pParent = 0);
	    // virtual ~SimpleLayout(); // Use compiler default
	private:
	    SimpleLayout(const SimpleLayout &rOther); // Not implemented
	    SimpleLayout &operator=(const SimpleLayout &rOther); // Not implemented
	        
	public:
	    virtual QString format(const LoggingEvent &rEvent);
	    
	protected:
	
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %SimpleLayout(name:"SL" referencecount:1) 
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	    virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline SimpleLayout::SimpleLayout(QObject *pParent) :
	    Layout(pParent)
	{}
	
	
} // namespace Log4Qt
	
	
// Q_DECLARE_TYPEINFO(Log4Qt::SimpleLayout, Q_COMPLEX_TYPE); // Use default
	
	
#endif // LOG4QT_SIMPLELAYOUT_H
