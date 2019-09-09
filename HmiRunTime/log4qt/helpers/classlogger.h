/******************************************************************************
 *
 * package:     Log4Qt
 * file:        classlogger.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * changes:		Sep 2008, Martin Heinrich:
 * 				- Replaced usage of q_atomic_test_and_set_ptr with
 * 				  QAtomicPointer
 *
 *
 * Copyright 2007 - 2008 Martin Heinrich
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

#ifndef LOG4QT_CLASSLOGGER_H
#define LOG4QT_CLASSLOGGER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QObject>
#if QT_VERSION >= QT_VERSION_CHECK(4, 4, 0)
#	include <QtCore/QAtomicPointer>
#	ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
#		warning "QAtomicPointer test and set is not native. The class Log4Qt::ClassLogger is not thread-safe."
#	endif
#endif


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	class Logger;

	/*!
	 * \brief The class ClassLogger provides logging for a QObject derived
	 *        class.
	 *
	 * The class ClassLogger provides a logger for a specified QObject derived
	 * object. It is used by \ref LOG4QT_DECLARE_QCLASS_LOGGER to implement the
	 * member functions provided by the macro.
	 *
	 * \note All the functions declared in this class are thread-safe.
	 *
	 * \sa LOG4QT_DECLARE_QCLASS_LOGGER
	 */
	class ClassLogger
	{
	public:
	    /*!
	     * Creates a ClassLogger object.
	     */
	    ClassLogger();
	    // ~ClassLogger(); // Use compiler default
	    // ClassLogger(const ClassLogger &rOther); // Use compiler default
	    // ClassLogger &operator=(const ClassLogger &rOther); // Use compiler default

	    /*!
	     * Returns a pointer to a Logger named after the class of the object
	     * \a pObject.
	     *
	     * On the first invocation the Logger is requested by a call to
	     * LogManager::logger(const char *pName). The pointer is stored to be
	     * returned on subsequent invocations.
	     *
	     * \sa LogManager::logger(const char *pName)
	     */
	    Logger *logger(const QObject *pObject);

	private:
#if QT_VERSION < QT_VERSION_CHECK(4, 4, 0)
	    volatile Logger *mpLogger;
#else
	    mutable QAtomicPointer<Logger> mpLogger;
#endif
	};


	/******************************************************************************
	 * Operators, Helper
	 ******************************************************************************/


	/**************************************************************************
	 * Inline
	 **************************************************************************/


} // namespace Log4Qt


// Q_DECLARE_TYPEinfo(Log4Qt::ClassLogger, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_CLASSLOGGER_H
