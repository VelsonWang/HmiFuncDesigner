/******************************************************************************
 *
 * package:     Log4Qt
 * file:        debugappender.h
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

#ifndef LOG4QT_DEBUGAPPENDER_H
#define LOG4QT_DEBUGAPPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/appenderskeleton.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	    
	/*!
	 * \brief The class DebugAppender appends logging events to the platform 
	 *        specific debug output.
	 * 
	 * A DebugAppender appends to the Debugger on Windows and to stderr on all 
	 * other systems.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class DebugAppender : public AppenderSkeleton
	{
	    Q_OBJECT
	
	public:
		/*!
		 * Creates a DebugAppender.
		 */
	    DebugAppender(QObject *pParent = 0);
	    
		/*!
		 * Creates a DebugAppender with the specified layout \a pLayout
		 */
	    DebugAppender(Layout *pLayout,
	                  QObject *pParent = 0);
	    
	    // virtual ~DebugAppender(); // Use compiler default
	private:
	    DebugAppender(const DebugAppender &rOther); // Not implemented
	    DebugAppender &operator=(const DebugAppender &rOther); // Not implemented
	        
	public:
		/*!
		 * The DebugAppended requires a layout. The function returns true.
		 * 
		 * \sa setLayout()
		 */
	    virtual bool requiresLayout() const;
	    
	protected:
		/*!
		 * Appends the specified logging event \a rEvent to the debug output. 
		 * The output is formatted using the appender's layout.
		 *
		 * The method is called by the AppenderSkeleton::doAppend() after it 
		 * the entry conditions have been tested and it has been found that the 
		 * logging event needs to be appended.
		 *
		 * \sa setLayout(), AppenderSkeleton::doAppend(), checkEntryConditions()
		 */
	    virtual void append(const LoggingEvent &rEvent);
	
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream \a rDebug 
	     * and returns the stream.
	     *
	     * <tt>
	     * %DebugAppender(name:"DA" filter:0x3bee6b8 isactive:true isclosed:false 
	     *                layout:"SL" referencecount:1 threshold:"NULL")  
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
	
	inline DebugAppender::DebugAppender(QObject *pParent) :
	    AppenderSkeleton(pParent)
	{}
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::DebugAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_DEBUGAPPENDER_H
