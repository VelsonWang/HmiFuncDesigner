/******************************************************************************
 *
 * package:     Log4Qt
 * file:        nullappender.h
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

#ifndef LOG4QT_NULLAPPENDER_H
#define LOG4QT_NULLAPPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/appenderskeleton.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{

/*!
 * \brief The class NullAppender ignores all requests to append.
 * 
 * \note All the functions declared in this class are thread-safe.
 * 
 * \note The ownership and lifetime of objects of this class are managed. See
 *       \ref Ownership "Object ownership" for more details.
 */
class NullAppender : public AppenderSkeleton
{
    Q_OBJECT

public:
    NullAppender(QObject *pParent = 0);
    virtual ~NullAppender();
private:
    NullAppender(const NullAppender &rOther); // Not implemented
    NullAppender &operator=(const NullAppender &rOther); // Not implemented

public:
    virtual bool requiresLayout() const;

protected:
    virtual void append(const LoggingEvent &rEvent);
    
#ifndef QT_NO_DEBUG_STREAM
    /*!
     * Writes all object member variables to the given debug stream \a rDebug and
     * returns the stream.
     *
     * <tt>
     * %NullAppender() 
     * </tt>
     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
     */
    virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
};


/******************************************************************************
 * Operators, Helper
 ******************************************************************************/


/******************************************************************************
 * Inline
 ******************************************************************************/

inline bool NullAppender::requiresLayout() const
{	return false;	}


} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::NullAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_NULLAPPENDER_H
