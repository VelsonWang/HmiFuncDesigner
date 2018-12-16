/******************************************************************************
 *
 * package:     Log4Qt
 * file:        appenderskeleton.cpp
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



/******************************************************************************
 * Dependencies
 ******************************************************************************/


#include "log4qt/appenderskeleton.h"

#include <QtCore/QDebug>
#include "log4qt/layout.h"
#include "log4qt/loggingevent.h"
#include "log4qt/logmanager.h"
#include "log4qt/spi/filter.h"


namespace Log4Qt
{
	    
	    
	/**************************************************************************
	* Declarations
	***************************************************************************/
	
	
	/*!
	 * \brief The class RecursionGuardLocker controls a boolean flag.
	 *
	 * It is a helper class to control a boolean flag. The class sets the flag
	 * on creation and resets it on destruction. 
	 */
	class RecursionGuardLocker
	{
	public:
	    RecursionGuardLocker(bool *pGuard);
	    ~RecursionGuardLocker();
	private:
	    RecursionGuardLocker(const RecursionGuardLocker &rOther); // Not implemented
	    RecursionGuardLocker &operator=(const RecursionGuardLocker &rOther); // Not implemented
	private:
		bool *mpGuard;
	};
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * Class implementation: RecursionGuardLocker
	***************************************************************************/
	
	
	inline RecursionGuardLocker::RecursionGuardLocker(bool *pGuard)
	{
	    Q_ASSERT_X(pGuard != 0, "RecursionGuardLocker::RecursionGuardLocker()", "Pointer to guard bool must not be null");
	    
	    mpGuard = pGuard;
	    *mpGuard = true;
	}
	
	
	inline RecursionGuardLocker::~RecursionGuardLocker()
	{
	    *mpGuard = false;
	};
	
	
	
	/**************************************************************************
	 * Class implementation: AppenderSkeleton
	 **************************************************************************/
	
	
	AppenderSkeleton::AppenderSkeleton(QObject *pParent) :
	    Appender(pParent),
	    mObjectGuard(QMutex::Recursive), // Recursive for doAppend()
	    mAppendRecursionGuard(false),
	    mIsActive(true),
	    mIsClosed(false),
	    mpLayout(0),
	    mThreshold(Level::NULL_INT),
		mpHeadFilter(0),
		mpTailFilter(0)
	{
	}
	
	
	AppenderSkeleton::AppenderSkeleton(const bool isActive, 
	                                   QObject *pParent) :
	    Appender(pParent),
	    mObjectGuard(QMutex::Recursive), // Recursive for doAppend()
	    mAppendRecursionGuard(false),
	    mIsActive(isActive),
	    mIsClosed(false),
	    mpLayout(0),
	    mThreshold(Level::NULL_INT),
	    mpHeadFilter(0),
	    mpTailFilter(0)
	{
	}
	
	
	void AppenderSkeleton::activateOptions() 
	{
	    QMutexLocker locker(&mObjectGuard);
	
	    if (requiresLayout() && !layout())
	    {
	    	LogError e = LOG4QT_QCLASS_ERROR(QT_TR_NOOP("Activation of appender '%1' that requires layout and has no layout set"), 
	    			                         APPENDER_ACTIVATE_MISSING_LAYOUT_ERROR);
	        e << name();
	        logger()->error(e);
	        return;
	    }
	    mIsActive = true;
	}
	
	
	void AppenderSkeleton::addFilter(Filter *pFilter)
	{
	    if(!pFilter)
	    {
	        logger()->warn("Adding null Filter to Appender '%1'", name());
	        return;
	    }
	
	    QMutexLocker locker(&mObjectGuard);
	    
	    mpTailFilter = pFilter;
	    if (mpHeadFilter)
	    	mpHeadFilter->setNext(pFilter);
	    else
	    	mpHeadFilter = pFilter;
	}
	
	
	void AppenderSkeleton::clearFilters()
	{
	    QMutexLocker locker(&mObjectGuard);
	
	    mpTailFilter = 0;
	    mpHeadFilter = 0;
	}
	
	
	void AppenderSkeleton::close()
	{
	    QMutexLocker locker(&mObjectGuard);
	    
	    mIsClosed = true;
	    mIsActive = false;
	}
	
	
	void AppenderSkeleton::doAppend(const LoggingEvent &rEvent)
	{
		// The mutex serialises concurrent access from multiple threads.
		// - e.g. two threads using the same logger 
		// - e.g. two threads using different logger with the same appender
		//
		// A call from the same thread will pass the mutex (QMutex::Recursive)
	    // and get to the recursion guard. The recursion guard blocks recursive
	    // invocation and prevents a possible endless loop.
		// - e.g. an appender logs an error with a logger that uses it

		QMutexLocker locker(&mObjectGuard);
	
	    if (mAppendRecursionGuard)
	        return;
	    
	    RecursionGuardLocker recursion_locker(&mAppendRecursionGuard);
	    
	    if (!checkEntryConditions())
	        return;
	    if (!isAsSevereAsThreshold(rEvent.level()))
	        return;
	    
	    Filter *p_filter = mpHeadFilter;
	    while(p_filter)
	    {
	    	Filter::Decision decision = p_filter->decide(rEvent); 
	    	if (decision == Filter::ACCEPT)
	    		break;
	    	else if (decision == Filter::DENY)
	    		return;
	    	else
	    		p_filter = p_filter->next();
	    }
	
	    append(rEvent);
	}
	
	
	bool AppenderSkeleton::checkEntryConditions() const
	{
	    // Q_ASSERT_X(, "WriterAppender::checkEntryConditions()", "Lock must be held by caller")
	
	    if (!isActive())
	    {
	        LogError e = LOG4QT_QCLASS_ERROR(QT_TR_NOOP("Use of non activated appender '%1'"),
                                            APPENDER_NOT_ACTIVATED_ERROR);
	        e << name();
	        logger()->error(e);
	        return false;
	    }
	    if (isClosed())
	    {
	        LogError e = LOG4QT_QCLASS_ERROR(QT_TR_NOOP("Use of closed appender '%1'"),
	                                         APPENDER_CLOSED_ERROR);
	        e << name();
	        logger()->error(e);
	        return false;
	    }
	    if (requiresLayout() && !layout())
	    {
	        LogError e = LOG4QT_QCLASS_ERROR(QT_TR_NOOP("Use of appender '%1' that requires layout and has no layout set"),
                                             APPENDER_USE_MISSING_LAYOUT_ERROR);
	        e << name();
	        logger()->error(e);
	        return false;
	    }
	    
	    return true;
	}
	
	
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
} // namespace Log4Qt
