/******************************************************************************
 *
 * package:     Log4Qt
 * file:        loggingevent.cpp
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
 *****************************************************************************/



/******************************************************************************
 * Dependencies
 ******************************************************************************/


#include "log4qt/loggingevent.h"

#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include "log4qt/helpers/datetime.h"
#include "log4qt/helpers/initialisationhelper.h"
#include "log4qt/logger.h"
#include "log4qt/mdc.h"
#include "log4qt/ndc.h"



namespace Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	
	    
	LOG4QT_GLOBAL_STATIC(QMutex, sequence_guard)
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * Class implementation: LoggingEvent
	 **************************************************************************/
	
	
	LoggingEvent::LoggingEvent() :
		mLevel(Level::NULL_INT),
	    mpLogger(0),
	    mMessage(),
	    mNdc(NDC::peek()),
	    mProperties(MDC::context()),
	    mSequenceNumber(nextSequenceNumber()),
	    mThreadName(),
	    mTimeStamp(DateTime::currentDateTime().toMilliSeconds())
	{
		setThreadNameToCurrent();
	}
	
	
	LoggingEvent::LoggingEvent(const Logger *pLogger, 
	                           Level level,
	                           const QString &rMessage) :
	    mLevel(level),
	    mpLogger(pLogger),
	    mMessage(rMessage),
	    mNdc(NDC::peek()),
	    mProperties(MDC::context()),
	    mSequenceNumber(nextSequenceNumber()),
	    mThreadName(),
	    mTimeStamp(DateTime::currentDateTime().toMilliSeconds())
	{
		setThreadNameToCurrent();
	}
	
	
	LoggingEvent::LoggingEvent(const Logger *pLogger,
	                           Level level,
	                           const QString &rMessage,
	                           qint64 timeStamp) :
	    mLevel(level),
	    mpLogger(pLogger),
	    mMessage(rMessage),
	    mNdc(NDC::peek()),
	    mProperties(MDC::context()),
	    mSequenceNumber(nextSequenceNumber()),
	    mThreadName(),
	    mTimeStamp(timeStamp)
	{
		setThreadNameToCurrent();
	}
	
	
	LoggingEvent::LoggingEvent(const Logger *pLogger, 
	                           Level level, 
	                           const QString &rMessage,
	                           const QString &rNdc,
	                           const QHash<QString, QString> &rProperties,
	                           const QString &rThreadName,
	                           qint64 timeStamp) :
	    mLevel(level),
	    mpLogger(pLogger),
	    mMessage(rMessage),
	    mNdc(rNdc),
	    mProperties(rProperties),
	    mSequenceNumber(nextSequenceNumber()),
	    mThreadName(rThreadName),
	    mTimeStamp(timeStamp)
	{
	}
	
	
	QString LoggingEvent::loggerName() const
	{	
		if (mpLogger)
			return mpLogger->name();
		else
			return QString();
	}
	
	
	QString LoggingEvent::toString() const
	{   
	    return level().toString() + QLatin1Char(':') + message();
	}
	
	
	qint64 LoggingEvent::sequenceCount()
	{
	    QMutexLocker locker(sequence_guard());
	    
	    return msSequenceCount;
	} 
	
	
	qint64 LoggingEvent::startTime()
	{
		return InitialisationHelper::startTime();
	}
	
	
	void LoggingEvent::setThreadNameToCurrent()
	{
		if (QThread::currentThread())
			mThreadName = QThread::currentThread()->objectName();
	}
	
	
	qint64 LoggingEvent::nextSequenceNumber()
	{   
	    QMutexLocker locker(sequence_guard());
	    
	    return ++msSequenceCount;    
	}
	
	
	qint64 LoggingEvent::msSequenceCount = 0;
	
	
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
#ifndef QT_NO_DATASTREAM
    QDataStream &operator<<(QDataStream &rStream, const LoggingEvent &rLoggingEvent)
    {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QDataStream stream(&buffer);
        
        // version
        quint16 version = 0; 
        stream << version;
        // version 0 data
        stream << rLoggingEvent.mLevel
               << rLoggingEvent.loggerName()
               << rLoggingEvent.mMessage
               << rLoggingEvent.mNdc
               << rLoggingEvent.mProperties
               << rLoggingEvent.mSequenceNumber
               << rLoggingEvent.mThreadName
               << rLoggingEvent.mTimeStamp;
        
        buffer.close();
        rStream << buffer.buffer();
        return rStream; 
    }
    
    
    QDataStream &operator>>(QDataStream &rStream, LoggingEvent &rLoggingEvent)
    {
        QByteArray array;
        rStream >> array;
        QBuffer buffer(&array);
        buffer.open(QIODevice::ReadOnly);
        QDataStream stream(&buffer);
        
        // version
        quint16 version; 
        stream >> version;
        // Version 0 data
        QString logger;
        stream >> rLoggingEvent.mLevel
               >> logger
               >> rLoggingEvent.mMessage
               >> rLoggingEvent.mNdc
               >> rLoggingEvent.mProperties
               >> rLoggingEvent.mSequenceNumber
               >> rLoggingEvent.mThreadName
               >> rLoggingEvent.mTimeStamp;
        if (logger.isEmpty())
            rLoggingEvent.mpLogger = 0;
        else
            rLoggingEvent.mpLogger = Logger::logger(logger);
        
        buffer.close();
        return rStream; 
    }
#endif // QT_NO_DATASTREAM

    
#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug, 
	                  const LoggingEvent &rLoggingEvent)
	{
	    QString logger;
	    if (rLoggingEvent.logger() != 0)
	        logger = rLoggingEvent.logger()->name();
	    
	    debug.nospace() << "LoggingEvent(" 
	        << "level:" << rLoggingEvent.level().toString() << " "
	        << "logger:" << logger << " "
	        << "message:" << rLoggingEvent.message() << " "
	        << "sequencenumber:" << rLoggingEvent.sequenceNumber() << " "
	        << "threadname:" << rLoggingEvent.threadName() << " "
	        << "timestamp:" << rLoggingEvent.timeStamp()
	        	<< "(" << DateTime::fromMilliSeconds(rLoggingEvent.timeStamp()) << ")"
	        << "sequenceCount:" << rLoggingEvent.sequenceCount()
	        << ")";
	    return debug.space();
	}
#endif // QT_NO_DEBUG_STREAM
	
	
} // namespace Log4Qt
