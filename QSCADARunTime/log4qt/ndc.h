/******************************************************************************
 *
 * package:     Log4Qt
 * file:        ndc.h
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

#ifndef LOG4QT_NDC_H
#define LOG4QT_NDC_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QStack>
#include <QtCore/QThreadStorage>
#include "log4qt/log4qt.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt 
{
	    
	/*!
	 * \brief The class NDC implements a nested diagnostic context.
	 * 
	 * The method remove() is not required. QThreadStorage cleans up on thread 
	 * exit.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 */
	class NDC
	{
	private:
	    NDC();
	    NDC(const NDC &rOther); // Not implemented
	    // virtual ~NDC(); // Use compiler default
	    NDC &operator=(const NDC &rOther); // Not implemented
	        
	public:
	    static void clear();
	    // JAVA: static QStack<QString> cloneStack();
	    // JAVA: static QString get();
	    static int depth();
	    // JAVA: inherit(Stack stack) 

	    /*!
         * Returns the NDC instance.
         */
        static NDC *instance();
            
	    static QString pop();
	    static void push(const QString &rMessage); 
	    // JAVA: static void remove(); // Not required
	    static void setMaxDepth(int maxDepth);
	    static QString peek();

	private:
	    QThreadStorage< QStack<QString> * > mStack;
	};
	
	
	/******************************************************************************
	 * Operators, Helper
	 ******************************************************************************/
	
	#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates NDC
	 * 
	 * Writes all object member variables to the given debug stream \a rDebug and
	 * returns the stream.
	 *
	 * <tt>
	 * %NDC(thread:"main" peek:"i = 3" depth:4) 
	 * </tt>
	 * \sa QDebug
	 */
	QDebug operator<<(QDebug debug, 
	                  const NDC &rNDC);
	#endif // QT_NO_DEBUG_STREAM
	
	
	/******************************************************************************
	 * Inline
	 ******************************************************************************/
	
	inline NDC::NDC() :
	    mStack()
	{}
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::NDC, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_NDC_H
