/******************************************************************************
 *
 * package:     Log4Qt
 * file:        appenderskeleton.h
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

#ifndef LOG4QT_APPENDERSKELETON_H
#define LOG4QT_APPENDERSKELETON_H


/******************************************************************************
 * Dependencies
******************************************************************************/

#include "log4qt/appender.h"

#include <QtCore/QMutex>
#include "log4qt/helpers/logobjectptr.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	    
	class Filter;
	class Layout;
	class Logger;
	class LoggingEvent;
	
	/*!
	 * \brief The class AppenderSkeleton implements general Appender functionality.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. See
	 *       \ref Ownership "Object ownership" for more details.
	 */
	class AppenderSkeleton : public Appender
	{
	    Q_OBJECT
	    
	    /*!
	     * The property holds if the Appender has been activated.
	     *
	     * \sa isActive()
	     */
	    Q_PROPERTY(bool isActive READ isActive)
	    
	    /*!
	     * The property holds if the Appender has been closed.
	     *
	     * \sa isClosed()
	     */
	    Q_PROPERTY(bool isClosed READ isClosed)
	
	    /*!
	     * The property holds the threshold level used by the Appender.
	     *
	     * \sa threshold(), setThreshold()
	     */
	     Q_PROPERTY(Level threshold READ threshold WRITE setThreshold)
	
	public:
	    AppenderSkeleton(QObject *pParent = 0);
	protected:
	    AppenderSkeleton(const bool isActive, 
	                     QObject *pParent = 0);
	public:
	    // virtual ~AppenderSkeleton(); Use compiler default
	private:
	    AppenderSkeleton(const AppenderSkeleton &rOther); // Not implemented
	    AppenderSkeleton &operator=(const AppenderSkeleton &rOther); // Not implemented
	        
	public:
	    // JAVA: ErrorHandler* errorHandler();
	    virtual Filter *filter() const;
	    virtual Layout *layout() const;
	    bool isActive() const;
	    bool isClosed() const;
	    virtual QString name() const;
	    Level threshold() const;
	    // JAVA: void setErrorHandler(ErrorHandler *pErrorHandler);
	    virtual void setLayout(Layout *pLayout);
	    virtual void setName(const QString &rName);
	    void setThreshold(Level level);
	    
	    virtual void activateOptions();
	    virtual void addFilter(Filter *pFilter);
	    virtual void clearFilters();
	    virtual void close();
	    
	    /*!
	     * Performs checks and delegates the actuall appending to the subclass 
	     * specific append() function.
	     * 
	     * \sa append(), checkEntryConditions(), isAsSevereAsThreshold(), Filter
	     */
	    virtual void doAppend(const LoggingEvent &rEvent);
	    
	    // JAVA: void finalize();
	    Filter* firstFilter() const;
	    bool isAsSevereAsThreshold(Level level) const;
	    
	protected:
	    virtual void append(const LoggingEvent &rEvent) = 0;
	
	    /*!
	     * Tests if all entry conditions for using append() in this class are 
	     * met.
	     * 
	     * If a conditions is not met, an error is logged and the function 
	     * returns false.
	     * 
	     * The checked conditions are:
	     * - That the appender has been activated (APPENDER_NOT_ACTIVATED_ERROR)
	     * - That the appender was not closed (APPENDER_CLOSED_ERROR)
	     * - That the appender has a layout set, if it requires one 
	     *   (logging_error(APPENDER_USE_MISSING_LAYOUT_ERROR)
	     * 
	     * The function is called as part of the checkEntryConditions() chain 
	     * started by doAppend(). The doAppend() function calls the subclass 
	     * specific checkEntryConditions() function. The function checks the 
	     * class specific conditions and calls checkEntryConditions() of 
	     * it's parent class. The last function called is 
	     * AppenderSkeleton::checkEntryConditions().
	     * 
	     * \sa doAppend()
	     */
	    virtual bool checkEntryConditions() const;
	    
	protected:
	    mutable QMutex mObjectGuard;
	private:
		bool mAppendRecursionGuard;
		volatile bool mIsActive;
		volatile bool mIsClosed;
	    LogObjectPtr<Layout> mpLayout;
	    Level mThreshold;
	    LogObjectPtr<Filter> mpHeadFilter;
	    LogObjectPtr<Filter> mpTailFilter;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline Filter *AppenderSkeleton::filter() const
	{	QMutexLocker locker(&mObjectGuard);
		return mpHeadFilter;    }
	
	inline Layout *AppenderSkeleton::layout() const
	{   QMutexLocker locker(&mObjectGuard);
		return mpLayout;    }
	
	inline QString AppenderSkeleton::name() const
	{	QMutexLocker locker(&mObjectGuard);
	    return objectName();	}
	
	inline Level AppenderSkeleton::threshold() const
	{   // QMutexLocker locker(&mObjectGuard); // Level is threadsafe
		return mThreshold;  }
	
	inline void AppenderSkeleton::setLayout(Layout *pLayout)
	{   QMutexLocker locker(&mObjectGuard);
		mpLayout = pLayout; }
	
	inline void AppenderSkeleton::setName(const QString &rName)
	{   QMutexLocker locker(&mObjectGuard);
	    setObjectName(rName);  }
	
	inline void AppenderSkeleton::setThreshold(Level level)
	{   // QMutexLocker locker(&mObjectGuard); // Level is threadsafe
		mThreshold = level;    }
	
	inline bool AppenderSkeleton::isActive() const
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		return mIsActive;   }
	
	inline bool AppenderSkeleton::isClosed() const
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		return mIsClosed;   }
	
	inline Filter *AppenderSkeleton::firstFilter() const
	{	QMutexLocker locker(&mObjectGuard);
	    return filter();    }
	
	inline bool AppenderSkeleton::isAsSevereAsThreshold(Level level) const
	{   // QMutexLocker locker(&mObjectGuard); // Level is threadsafe
		return (mThreshold <= level);   }
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::AppenderSkeleton, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_APPENDERSKELETON_H
