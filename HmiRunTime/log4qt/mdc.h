/******************************************************************************
 *
 * package:     Log4Qt
 * file:        mdc.h
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

#ifndef LOG4QT_MDC_H
#define LOG4QT_MDC_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QThreadStorage>
#include "log4qt/log4qt.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt 
{
	    
	
/*!
	 * \brief The class MDC implements a mapped diagnostic context.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 */
	class MDC
	{
	private:
	    MDC();
	    MDC(const MDC &rOther); // Not implemented
	    // virtual ~MDC(); // Use compiler default
	    MDC &operator=(const MDC &rOther); // Not implemented
	        
	public:
	    static QString get(const QString &rKey);
	    static QHash<QString, QString> context();

        /*!
         * Returns the MDC instance.
         */
        static MDC *instance();
        
	    static void put(const QString &rKey, const QString &rValue);
	    static void remove(const QString &rKey);
	
    private:
	    static QHash<QString, QString> *localData();
	    
	private:
	    QThreadStorage< QHash<QString, QString> * > mHash;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates MDC
	 * 
	 * Writes all object member variables to the given debug stream \a rDebug
	 * and returns the stream.
	 *
	 * <tt>
	 * %MDC(thread:"main" context:QHash(("login", "Peter")("database", "UAT")) ) 
	 * </tt>
	 * \sa QDebug
	 */
	QDebug operator<<(QDebug debug, 
	                  const MDC &rMDC);
	#endif // QT_NO_DEBUG_STREAM
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline MDC::MDC() :
	    mHash()
	{}
	
	inline void MDC::put(const QString &rKey, const QString &rValue)
	{   localData()->insert(rKey, rValue); }
	
	inline void MDC::remove(const QString &rKey)
	{   localData()->remove(rKey);  }
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::MDC, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_MDC_H
