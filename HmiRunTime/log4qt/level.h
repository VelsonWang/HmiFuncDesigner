/******************************************************************************
 *
 * package:     Log4Qt
 * file:        level.h
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

#ifndef LOG4QT_LEVEL_H
#define LOG4QT_LEVEL_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QMetaType>
#include "log4qt/log4qt.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	    
	/*!
	 * \brief The class Level defines the level of a logging event.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 */
	class Level
	{
	public:
	    // Comparisson operators rely on the order: 
	    // NULL_INT < ALL_INT < TRACE_INT < ...
	    // Serialisation uses unsigned 8 bit int
	    
	    /*!
	     * The enumeration Value contains all possible Level values.
	     */
	    enum Value
	    {
	        /*! NULL_INT is used for no level has been specified */ 
	        NULL_INT = 0,
	        ALL_INT = 32,
	        TRACE_INT = 64,
	        DEBUG_INT = 96,
	        INFO_INT = 128,
	        WARN_INT = 150,
	        ERROR_INT = 182,
	        FATAL_INT = 214,
	        OFF_INT = 255
	    };
	    
	public:
	    Level(Value value = NULL_INT);
	    // Level(const Level &rOther); // Use compiler default
	    // virtual ~Level(); // Use compiler default
	    // Level &operator=(const Level &rOther); // Use compiler default
	    
	    int syslogEquivalent() const;
	    int toInt() const;
	    
	    bool operator==(const Level &rOther) const;
	    bool operator!=(const Level &rOther) const;
	    bool operator<(const Level &rOther) const;
	    bool operator<=(const Level &rOther) const;
	    bool operator>(const Level &rOther) const;
	    bool operator>=(const Level &rOther) const;
	    QString toString() const; 
	
	    static Level fromString(const QString &rName, bool *pOk = 0); 
	
	private:
	    // QMutex mObjectGuard;
		volatile Value mValue;
		
#ifndef QT_NO_DATASTREAM
        // Needs to be friend to stream objects
	    friend QDataStream &operator<<(QDataStream &rStream, 
	                                   const Level &rLevel);
	    friend QDataStream &operator>>(QDataStream &rStream, 
	                                   Level &rLevel);
#endif // QT_NO_DATASTREAM
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
#ifndef QT_NO_DATASTREAM
    /*!
     * \relates Level
     * 
     * Writes the given error \a rLevel to the given stream \a rStream, 
     * and returns a reference to the stream.
     */
    QDataStream &operator<<(QDataStream &rStream, 
                            const Level &rLevel);
    
    /*!
     * \relates Level
     * 
     * Reads an error from the given stream \a rStream into the given 
     * error \a rLevel, and returns a reference to the stream.
     */
    QDataStream &operator>>(QDataStream &rStream, 
                            Level &rLevel);
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates Level
	 * 
	 * Writes all object member variables to the given debug stream \a rDebug 
	 * and returns the stream.
	 *
	 * <tt>
	 * %Level("ERROR")
	 * </tt>
	 * \sa QDebug
	 */
	QDebug operator<<(QDebug debug, 
	                  const Level &rLevel);
#endif // QT_NO_DEBUG_STREAM
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	
	inline Level::Level(Value value) :
	    mValue(value)
	{}
	
    inline int Level::toInt() const
    {   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
        return mValue; }

    inline bool Level::operator==(const Level &rOther) const
    {   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
        return mValue == rOther.mValue; }
	
	inline bool Level::operator!=(const Level &rOther) const
    {   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
	    return mValue != rOther.mValue; }
	
	inline bool Level::operator<(const Level &rOther) const
    {   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
	    return mValue < rOther.mValue; }
	
	inline bool Level::operator<=(const Level &rOther) const
    {   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
	    return mValue <= rOther.mValue; }
	
	inline bool Level::operator>(const Level &rOther) const
    {   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
	    return mValue > rOther.mValue; }
	
	inline bool Level::operator>=(const Level &rOther) const
    {   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
	    return mValue >= rOther.mValue; }
	
	
} // namespace Log4Qt


Q_DECLARE_METATYPE(Log4Qt::Level)
Q_DECLARE_TYPEINFO(Log4Qt::Level, Q_MOVABLE_TYPE);


#endif // LOG4QT_LEVEL_H
