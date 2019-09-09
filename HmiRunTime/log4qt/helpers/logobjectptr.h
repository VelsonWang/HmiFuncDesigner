/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logobjectptr.h
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

#ifndef LOG4QT_LOGOBJECTPTR_H
#define LOG4QT_LOGOBJECTPTR_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/helpers/logobject.h"

namespace Log4Qt
{
	/*!
	 * \brief The class LogObjectPtr implements automatic reference counting
	 *        for LogObject objects.
	 */
	template <class T>
	class LogObjectPtr
	{
	public:
		/*!
		 * Constructs a 0 LogObject pointer.
		 */
		LogObjectPtr();

		/*!
		 * Constructs a LogObject pointer that points to the same object then
		 * \a rOther. The reference counter of the object is incremented by
		 * one.
		 */
	    LogObjectPtr(const LogObjectPtr<T> &rOther);

		/*!
		 * Constructs a LogObject pointer that points to the object
		 * \a LogObject. The reference counter of the object is incremented by
		 * one.
		 */
		LogObjectPtr(T *pLogObject);

		/*!
		 * Assignment operator. Sets the LogObject pointer to point to the
		 * same object that \a rOther points to. The reference counter of the
		 * object the LogObjectPtr pointed to before the assignment is
		 * decremented by one. The reference counter of the object \a rOther
		 * is pointing to is incremented by one.
		 */
	    LogObjectPtr<T> &operator=(const LogObjectPtr<T> &rOther);

		/*!
		 * Destructs the object. The reference counter of the object the
		 * LogObjectPtr points to is decremented by one.
		 */
	    ~LogObjectPtr();

		/*!
		 * Assignment operator. Sets the LogObject pointer to point to the
		 * object \a pLogObject. The reference counter of the object the
		 * LogObjectPtr pointed to before the assignment is decremented by
		 * one. The reference counter of the object \a pLogObject is pointing
		 * to is incremented by one.
		 */
	    LogObjectPtr<T> &operator=(T *pLogObject);

		/*!
		 * Arrow operator. Returns the LogObject the object points to.
		 */
	    T *operator->() const;

		/*!
		 * Dereference operator. Returns a pointer to the LogObject the
		 * object points to.
		 */
	    T &operator*() const;

		/*!
		 * Cast operator. Cast the object to the LogObject the object points
		 * to.
		 */
	    operator T*() const;

	private:
		void retain() const;
		void release() const;

	private:
		T *mpLogObject;
	};


	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/


	/**************************************************************************
	 * Inline
	 **************************************************************************/

	template <class T>
	inline LogObjectPtr<T>::LogObjectPtr() :
		mpLogObject(0)
	{}

	template <class T>
	inline LogObjectPtr<T>::LogObjectPtr(const LogObjectPtr<T> &rOther) :
		mpLogObject(rOther.mpLogObject)
	{	retain();	}

	template <class T>
	inline LogObjectPtr<T>::LogObjectPtr(T *pLogObject) :
		mpLogObject(pLogObject)
	{	retain();	}

	template <class T>
	inline LogObjectPtr<T> &LogObjectPtr<T>::operator=(const LogObjectPtr<T> &rOther)
	{	rOther.retain();
	    release();
	    mpLogObject = rOther.mpLogObject;
	    return *this;	}

	template <class T>
	inline LogObjectPtr<T>::~LogObjectPtr()
	{	release();	}

	template <class T>
	inline LogObjectPtr<T> &LogObjectPtr<T>::operator=(T *pLogObject)
	{	if (pLogObject)
			reinterpret_cast<LogObject *>(pLogObject)->retain();
		release();
		mpLogObject = pLogObject;
		return *this;	}

	template <class T>
	inline T *LogObjectPtr<T>::operator->() const
	{	return mpLogObject;	}

	template <class T>
	inline T &LogObjectPtr<T>::operator*() const
	{	return *mpLogObject;	}

	template <class T>
	inline LogObjectPtr<T>::operator T*() const
	{	return mpLogObject;	}

	template <class T>
	inline void LogObjectPtr<T>::retain() const
	{	if (mpLogObject)
			reinterpret_cast<LogObject *>(mpLogObject)->retain();	}

	template <class T>
	inline void LogObjectPtr<T>::release() const
	{
		if (mpLogObject)
			reinterpret_cast<LogObject *>(mpLogObject)->release();
	}

} // namespace Log4Qt


//Q_DECLARE_TYPEINFO(Log4Qt::LogObjectPtr<T>, Q_MOVABLE_TYPE); // Declare within T


#endif // LOG4QT_LOGOBJECTPTR_H
