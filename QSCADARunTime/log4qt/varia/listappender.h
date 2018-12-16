/******************************************************************************
 *
 * package:     Log4Qt
 * file:        listappender.h
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

#ifndef LOG4QT_LISTAPPENDER_H
#define LOG4QT_LISTAPPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/appenderskeleton.h"

#include <QtCore/QList>
#include <QtCore/QMutex>
#include "log4qt/loggingevent.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	    
	/*!
	 * \brief The class ListAppender appends logging events to a list for later
	 *        processing.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class ListAppender : public AppenderSkeleton
	{
	    Q_OBJECT
	    
	    /*!
	     * The property holds, if the Appender is used by a configurator.
	     *
	     * The default value is false for not being a configurator list.
	     *
	     * \sa configuratorList(), setConfiguratorList()
	     */
	    Q_PROPERTY(bool configuratorList READ configuratorList WRITE setConfiguratorList)
	    
	    /*!
	     * The property holds the maximum count used by the appender.
	     *
	     * The default maximum count is -1 for unlimited.
	     *
	     * \sa maxCount(), setMaxCount()
	     */
	    Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount)
	    
	public:
	    ListAppender(QObject *pParent = 0);
	    virtual ~ListAppender();
	private:
	    ListAppender(const ListAppender &rOther); // Not implemented
	    ListAppender &operator=(const ListAppender &rOther); // Not implemented
	        
	public:
		/*!
		 * Returns true, if the appender is used by a configurator. Otherweise it returns 
		 * false.
		 * 
		 * \sa setConfiguratorList()
		 */
		bool configuratorList() const;
		
	    QList<LoggingEvent> list() const;
	    int maxCount() const;
	    
		/*!
		 * Sets that the appender is used by a configurator. If set to true, the appender
		 * will not be removed from a Logger when Logger::removeAllAppenders()is called.
		 * This way the appender can collect events raised during the configuration process.
		 * 
		 * \sa configuratorList(), BasicConfigurator, PropertyConfigurator, 
		 *     ConfiguratorHelper::configureError()
		 */
	    void setConfiguratorList(bool isConfiguratorList);
	    
	    void setMaxCount(int n);
	    
	    QList<LoggingEvent> clearList();
	    virtual bool requiresLayout() const;
	    
	protected:
	    virtual void append(const LoggingEvent &rEvent);
	    
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %ListAppender(name:"LA" count:1 filter:0x41fa488 isactive:true 
	     *               isclosed:false maxcount:170 referencecount:1 
	     *               threshold:"TRACE_SET")  
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	    virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	
	    /*!
	     * Ensures that the count of events is less or equal then the maxium 
	     * count. If the list contains too many items, items are deleted from 
	     * the begin of the list.
	     */
	    void ensureMaxCount();
	    
	private:
		volatile bool mConfiguratorList; 
	    QList<LoggingEvent> mList;
	    volatile int mMaxCount;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline bool ListAppender::configuratorList() const
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		return mConfiguratorList;   }

	inline int ListAppender::maxCount() const
	{   return mMaxCount;    }
	
	inline void ListAppender::setConfiguratorList(bool isConfiguratorList)
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		mConfiguratorList = isConfiguratorList;   }

	inline bool ListAppender::requiresLayout() const
	{   return false;    }
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::ListAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_LISTAPPENDER_H
