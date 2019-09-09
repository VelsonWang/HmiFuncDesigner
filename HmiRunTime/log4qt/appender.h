/******************************************************************************
 *
 * package:     Log4Qt
 * file:        appender.h
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
 ********************************************************************************/

#ifndef LOG4QT_APPENDER_H
#define LOG4QT_APPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/helpers/logobject.h"

#include "log4qt/helpers/logobjectptr.h"

#include "log4qt/logger.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	class Filter;
	class Layout;
	class LoggingEvent;
	
	/*!
	 * \brief The class Appender is the base class for all Appenders.
	 *
	 * To allow the whole hirarchy to be an ascendant of QObject Appender is 
	 * not an interface.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed.
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class Appender : public LogObject
	{
	    Q_OBJECT
	    
	    /*!
	     * The property holds the Layout used by the Appender.
	     *
	     * \sa layout(), setLayout()
	     */
	    Q_PROPERTY(Layout* layout READ layout WRITE setLayout)
	    
	    /*!
	     * The property holds the name of the Appender.
	     *
	     * \sa name(), setName()
	     */
	    Q_PROPERTY(QString name READ name WRITE setName)
	    
	    /*!
	     * The property holds if the Appender requires a Layout or not.
	     *
	     * \sa requiresLayout(), setRequiresLayout()
	     */
	    Q_PROPERTY(bool requiresLayout READ requiresLayout)
	
	public:
	    Appender(QObject *pParent = 0);
	    virtual ~Appender();
	private:
	    Appender(const Appender &rOther); // Not implemented
	    Appender &operator=(const Appender &rOther); // Not implemented
	    
	public:
	    // JAVA: ErrorHandler* errorHandler();
	    virtual Filter *filter() const = 0;
	    virtual QString name() const = 0;
	    virtual Layout *layout() const = 0;
	    virtual bool requiresLayout() const = 0;
	    // JAVA: void setErrorHandler(ErrorHandler *pErrorHandler);
	    virtual void setLayout(Layout *pLayout) = 0;
	    virtual void setName(const QString &rName) = 0;
	    
	    virtual void addFilter(Filter *pFilter) = 0;
	    virtual void clearFilters() = 0; 
	    virtual void close() = 0;
	    virtual void doAppend(const LoggingEvent &rEvent) = 0;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline Appender::Appender(QObject *pParent) :
	   LogObject(pParent)
	{}
	
	inline Appender::~Appender()
	{}
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::Appender, Q_COMPLEX_TYPE); // Use default
Q_DECLARE_TYPEINFO(Log4Qt::LogObjectPtr<Log4Qt::Appender>, Q_MOVABLE_TYPE);


#endif // LOG4QT_APPENDER_H
