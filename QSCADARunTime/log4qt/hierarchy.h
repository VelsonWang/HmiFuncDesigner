/******************************************************************************
 *
 * package:     Log4Qt
 * file:        hierarchy.h
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

#ifndef LOG4QT_HIERARCHY_H
#define LOG4QT_HIERARCHY_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/loggerrepository.h"

#include <QtCore/QHash>
#include <QtCore/QReadWriteLock>


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	    
	/*!
	 * \brief The class Hierarchy implements a logger repository.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 */
	class Hierarchy : public LoggerRepository
	{
	public:
	    Hierarchy();
	    // Hierarchy(const Hierarchy &rOther); // Use compiler default
	    virtual ~Hierarchy();
	    // Hierarchy &operator=(const Hierarchy &rOther); // Use compiler default
	        
	public:
	    virtual bool exists(const QString &rName) const;
	    virtual Logger *logger(const QString &rName);
	    virtual QList<Logger *> loggers() const;
	    // JAVA: virtual Logger *logger(const String &rName, LoggerFactory *pFactory);
	    virtual Logger *rootLogger() const;
	    virtual Level threshold() const;
	    virtual void setThreshold(Level level);
	    virtual void setThreshold(const QString &rThreshold);
	    
	    // JAVA: void clear();
	    virtual bool isDisabled(Level level);
	    virtual void resetConfiguration();
	    virtual void shutdown();
	    
	    // JAVA: virtual void addHierarchyEventListener(HierarchyEventListener *pEventListener);
	    // JAVA: virtual void emitNoAppenderWarning(Logger *plogger) const;
	    // JAVA: virtual void fireAddAppenderEvent(Logger *plogger, Appender *pAppender) const;
	
	    // JAVA: void addRenderer(const QString &rClass, ObjectRenderer *pObjectRenderer);
	    // JAVA: QHash<QString, ObjectRenderer *> getRendererMap() const;
	    // JAVA: setRenderer(const QString &rClass, ObjectRenderer *pObjectRenderer);
	
	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream \a rDebug and
	     * returns the stream.
	     *
	     * <tt>
	     * %Hierarchy(loggers:6 threshold:"ALL" root-level:"DEBUG" root-appenders:0)
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LoggerRepository &rLoggerRepository)
	     */
	    virtual QDebug debug(QDebug &rdebug) const;
#endif
	    
	private:
	    Logger *createLogger(const QString &rName);
	    void resetLogger(Logger *pLogger, Level level) const;
	
	private:
	    mutable QReadWriteLock mObjectGuard;
	    QHash<QString, Logger*> mLoggers;
	    volatile bool mHandleQtMessages;
	    Level mThreshold;
	    Logger *mpRootLogger;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline Logger *Hierarchy::rootLogger() const
	{   // QReadLocker locker(&mObjectGuard); // Constant for object lifetime
	    return mpRootLogger;    }
	
	inline Level Hierarchy::threshold() const
	{   // QReadLocker locker(&mObjectGuard); // Level is threadsafe
	    return mThreshold;  }
	
	inline void Hierarchy::setThreshold(Level level)
	{   // QReadLocker locker(&mObjectGuard); // Level is threadsafe
	    mThreshold = level;  }
	
	inline bool Hierarchy::isDisabled(Level level)
	{   // QReadLocker locker(&mObjectGuard); // Level is threadsafe
	    return level < mThreshold;  }
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::Hierarchy, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_HIERARCHY_H
