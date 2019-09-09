/******************************************************************************
*
* package:     Log4Qt
* file:        loggingevent.h
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

#ifndef LOG4QT_LOG4QTEVENT_H
#define LOG4QT_LOG4QTEVENT_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QDateTime>
#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>
#include "log4qt/level.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	class Logger;
	
	/*!
	 * \brief The class LoggingEvent is the internal representation of a 
	 *        logging event.
	 * 
	 * The class uses milliseconds since 1970-01-01T00:00:00, Coordinated 
	 * Universal Time for time values. For converstion from and to QDateTime
	 * use DateTime.
	 */
	class LoggingEvent
	{
	public:
	    LoggingEvent();
	    LoggingEvent(const Logger *pLogger, 
	                 Level level,
	                 const QString &rMessage);
	    LoggingEvent(const Logger *pLogger,
	    		     Level level, 
	    		     const QString &rMessage,
	                 qint64 timeStamp);
	    LoggingEvent(const Logger *pLogger, 
	                 Level level, 
	                 const QString &rMessage, 
	                 const QString &rNdc, 
	                 const QHash<QString, QString> &rProperties,
	                 const QString &rThreadName,
	                 qint64 timeStamp);
	    // LoggingEvent(const LoggingEvent &LoggingEvent::rOther); // Use compiler default
	    // virtual ~LoggingEvent(); // Use compiler default
	    // LoggingEvent &operator=(const LoggingEvent &LoggingEvent::rOther); // Use compiler default
	        
	    // JAVA: QString fqnOfLoggerClass() const;
	    Level level() const;
	    // LocationInformation locationInformation() const;
	    const Logger *logger() const;
	    QString message() const;
	    QHash<QString, QString> mdc() const;
	    QString ndc() const;
	    QHash<QString, QString> properties() const;
	    qint64 sequenceNumber() const;
	    QString threadName() const;
	    // JAVA: ThrowableInformation throwableInformation() const;
	    qint64 timeStamp() const;
	    
	    // JAVA: bool locationInformationExists() const;
	    QString loggerName() const;
	    QString property(const QString &rKey) const;
	    QStringList propertyKeys() const;
	    void setProperty(const QString &rKey, const QString &rValue);
	    // JAVA: QString throwableStrRep() const;
	    QString toString() const;
	    static qint64 sequenceCount();
	    static qint64 startTime();
	
	private:
		void setThreadNameToCurrent();
	    static qint64 nextSequenceNumber();
	
	private:
	    Level mLevel;
	    const Logger *mpLogger;
	    QString mMessage;
	    QString mNdc;
	    QHash<QString, QString> mProperties;
	    qint64 mSequenceNumber;
	    QString mThreadName;
	    qint64 mTimeStamp;
        static qint64 msSequenceCount;
	    
#ifndef QT_NO_DATASTREAM
	    // Needs to be friend to stream objects
	    friend QDataStream &operator<<(QDataStream &rStream, 
                                       const LoggingEvent &rLoggingEvent);
	    friend QDataStream &operator>>(QDataStream &rStream, 
                                       LoggingEvent &rLoggingEvent);
#endif // QT_NO_DATASTREAM
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
#ifndef QT_NO_DATASTREAM
	/*!
	 * \relates LoggingEvent
	 * 
	 * Writes the given error \a rLoggingEvent to the given stream \a rStream, 
	 * and returns a reference to the stream.
	 */
	QDataStream &operator<<(QDataStream &rStream,
	                        const LoggingEvent &rLoggingEvent);
	
	/*!
	 * \relates LoggingEvent
	 * 
	 * Reads an error from the given stream \a rStream into the given 
	 * error \a rLoggingEvent, and returns a reference to the stream.
	 */
	QDataStream &operator>>(QDataStream &rStream, 
	                        LoggingEvent &rLoggingEvent);
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates LoggingEvent
	 * 
	 * Writes all object member variables to the given debug stream \a debug and
	 * returns the stream.
	 *
	 * <tt> 
	 * %LoggingEvent(level:"WARN" logger:"Log4Qt::Properties" 
	 *               message:"Unknown escape sequence '\j' in property starting at line 1" 
	 *               sequencenumber:14 threadname:"main" 
	 *               timestamp:1194337148937(QDateTime("Tue Nov 6 03:19:08 2007") ) 
	 *               sequenceCount: 14 )  
	 * </tt>
	 * 
	 * \sa QDebug
	 */
	QDebug operator<<(QDebug debug, 
	                  const LoggingEvent &rLoggingEvent);
#endif // QT_NO_DEBUG_STREAM
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline Level LoggingEvent::level() const
	{   return mLevel;  }
	
	inline const Logger *LoggingEvent::logger() const
	{   return mpLogger;  }
	
	inline QString LoggingEvent::message() const
	{   return mMessage;  }
	
	inline QHash<QString, QString> LoggingEvent::mdc() const
	{   return mProperties;  }
	
	inline QString LoggingEvent::ndc() const
	{   return mNdc;  }
	
	inline QHash<QString, QString> LoggingEvent::properties() const
	{	return mProperties;	}
	
	inline qint64 LoggingEvent::sequenceNumber() const
	{   return mSequenceNumber;  }
	
	inline QString LoggingEvent::threadName() const
	{   return mThreadName;  }
	
	inline qint64 LoggingEvent::timeStamp() const
	{   return mTimeStamp;  }
	
	inline QString LoggingEvent::property(const QString &rKey) const
	{	return mProperties.value(rKey);	}
	
	inline QStringList LoggingEvent::propertyKeys() const
	{	return QStringList(mProperties.keys());	}
	
	inline void LoggingEvent::setProperty(const QString &rKey, const QString &rValue)
	{	mProperties.insert(rKey, rValue);	}
	
	
} // namespace Log4Qt


Q_DECLARE_METATYPE(Log4Qt::LoggingEvent)
Q_DECLARE_TYPEINFO(Log4Qt::LoggingEvent, Q_MOVABLE_TYPE);


#endif // LOG4QT_LOG4QTEVENT_H
