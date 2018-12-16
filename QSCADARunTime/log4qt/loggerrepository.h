/******************************************************************************
 *
 * package:     Log4Qt
 * file:        loggerrepository.h
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

#ifndef LOG4QT_LOGGERREPOSITORY_H
#define LOG4QT_LOGGERREPOSITORY_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QList>
#include "log4qt/level.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	   
	class Logger;
	
	/*!
	 * \brief The class LoggerRepository is abstract base class for a logger 
	 *        repository.
	 */
	class LoggerRepository
	{
	public:
	    // LoggerRepository(); // Use compiler default
	    // LoggerRepository(const LoggerRepository &rOther); // Use compiler default
	    // virtual ~LoggerRepository(); // Use compiler default
	    // LoggerRepository &operator=(const LoggerRepository &rOther); // Use compiler default
	        
	public:
	    virtual bool exists(const QString &rName) const = 0;
	    virtual Logger *logger(const QString &rName) = 0;
	    // JAVA: virtual Logger *logger(const String &rName, LoggerFactory *pFactory);
	    virtual QList<Logger *> loggers() const = 0;
	    virtual Logger *rootLogger() const = 0;
	    virtual Level threshold() const = 0;
	    virtual void setThreshold(Level level) = 0;
	    virtual void setThreshold(const QString &rThreshold) = 0;
	    
	    virtual bool isDisabled(Level level) = 0;
	    virtual void resetConfiguration() = 0;
	    virtual void shutdown() = 0;
	
	    // JAVA: virtual void addHierarchyEventListener(HierarchyEventListener *pEventListener);
	    // JAVA: virtual void emitNoAppenderWarning(Logger *plogger) const;
	    // JAVA: virtual void fireAddAppenderEvent(Logger *plogger, Appender *pAppender) const;
	
	protected:
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     *\relates LoggerRepository
	     *
	     * Writes all object member variables to the given debug stream \a rDebug 
	     * and returns the stream.
	     *
	     * The member function is used by 
	     * QDebug operator<<(QDebug debug, const LoggerRepository &rLoggerRepository)
	     * to generate class specific output.
	     *
	     * \sa QDebug operator<<(QDebug debug, const LoggerRepository &rLoggerRepository)
	     */
	    virtual QDebug debug(QDebug &rDebug) const = 0;
	    friend QDebug operator<<(QDebug debug, 
	                             const LoggerRepository &rLoggerRepository);
#endif
	};
	
	
	/******************************************************************************
	 * Operators, Helper
	 ******************************************************************************/
	
#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates LoggerRepository
	 * Writes all object member variables to the given debug stream \a debug 
	 * and returns the stream.
	 *
	 * To handle subclassing the function uses the virtual member function 
	 * debug(). This allows each class to generate its own output.
	 *
	 * \sa QDebug, debug()
	 */
	QDebug operator<<(QDebug debug,
	                  const LoggerRepository &rLoggerRepository);
#endif
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::LoggerRepository, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_LOGGERREPOSITORY_H
