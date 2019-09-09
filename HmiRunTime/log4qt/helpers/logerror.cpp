/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logerror.cpp
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



/******************************************************************************
 * Dependencies
 *****************************************************************************/


#include "log4qt/helpers/logerror.h"

#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QTextCodec>
#include <QtCore/QThreadStorage>
#include "log4qt/helpers/initialisationhelper.h"


namespace Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/

    
    typedef QThreadStorage<LogError *> ThreadError;
	
    
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
    LOG4QT_GLOBAL_STATIC(ThreadError, thread_error)

    
    
	/**************************************************************************
	 * Class implementation: LogError
	 **************************************************************************/
	
	
	LogError::LogError() :
	    mCode(0),
        mContext(),
      	mMessage(),
      	mSymbol(),
      	mArgs(),
      	mCausingErrors()
	{
	}
	
	
	LogError::LogError(const QString &rMessage, 
	                   int code, 
	                   const QString &rSymbol, 
	                   const QString &rContext) :
      	mCode(code),
        mContext(rContext),
      	mMessage(cleanMessage(rMessage)),
      	mSymbol(rSymbol),
      	mArgs(),
      	mCausingErrors()
	{
	}

	
	LogError::LogError(const char *pMessage, 
	                   int code, 
	                   const char *pSymbol, 
	                   const char *pContext,
	                   Encoding encoding) :
	    mCode(code),
	    mContext(QString::fromLatin1(pContext)),
	    mMessage(),
	    mSymbol(QString::fromLatin1(pSymbol)),
	    mArgs(),
	    mCausingErrors()
	{
		switch(encoding)
		{
			case LATIN1:
				mMessage = QString::fromLatin1(pMessage);
				break;
			case CODECFORTR:
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
				mMessage = QTextCodec::codecForTr()->toUnicode(pMessage);
#else
                mMessage = QString::fromUtf8(pMessage);
#endif
				break;
			case UNICODEUTF8:
				mMessage = QString::fromUtf8(pMessage);
				break;
			default:
	            Q_ASSERT_X(false, "LogError::LogError", "Unkown encoding constant");
				mMessage = QString::fromLatin1(pMessage);
		}
		mMessage = cleanMessage(mMessage);
		
		if (mSymbol == QString::number(mCode))
			mSymbol.clear();
	}
	
	
	QString LogError::translatedMessage() const
	{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        return QCoreApplication::translate(mContext.toLatin1(), mMessage.toUtf8().data(), 0, QCoreApplication::UnicodeUTF8);
#else
        return QCoreApplication::translate(mContext.toLatin1(), mMessage.toUtf8().data(), 0);
#endif
	}
	
	
	LogError LogError::lastError()
	{
	    if (!thread_error()->hasLocalData())
            return LogError();
	    else
	        return *thread_error()->localData();
	}
	
	
	void LogError::setLastError(const LogError &rLogError)
	{
	    if (!thread_error()->hasLocalData())
	        thread_error()->setLocalData(new LogError);
	    
	    *thread_error()->localData() = rLogError;
	}
	
	
	QString LogError::toString() const
	{	    
	    QString result = messageWithArgs();
	    
	    QString context_symbol = mContext;
	    if (!context_symbol.isEmpty() && !mSymbol.isEmpty())
	        context_symbol.append(QLatin1String("::"));
	    context_symbol.append(mSymbol);
	    
	    if (!context_symbol.isEmpty() || mCode)
	    {
	        result.append(QLatin1String(" ("));
	        if (!context_symbol.isEmpty())
	            result.append(context_symbol);
	        if (!context_symbol.isEmpty() && mCode)
	            result.append(QLatin1String(", "));
	        if (mCode)
	            result.append(QString::number(mCode));
	        result.append(QLatin1String(")"));
	    }
	    
	    if (!mCausingErrors.isEmpty())
	    {
	        QString causing_errors_str = QLatin1String(": ") + mCausingErrors.at(0).toString();
	        int i = 1;
	        while (i < mCausingErrors.count())
	        {
	            causing_errors_str.append(QLatin1String(", ")).append(mCausingErrors.at(i).toString());
	            i++;
	        }
	        result.append(causing_errors_str);
	    }
	    
	    return result;
	}
	
	
	QString LogError::insertArgs(const QString &rMessage) const
	{
	    QString result;
	    
	    /*
	     
	    // Don't use a loop to be able to handle arguments that conatin strings
	    // like %1.
	    // Using this method only 9 arguments can be handled as the %1
	    // in %11 gets also replaced with the first argument.
	
	    switch (mArgs.count())
	    {
	        case 0:
	            break;
	        case 1:
	            result = rMessage.arg(mArgs.at(0));
	            break;
	        case 2:
	            result = rMessage.arg(mArgs.at(0), mArgs.at(1));
	            break;
	        case 3:
	            result = rMessage.arg(mArgs.at(0), mArgs.at(1), mArgs.at(2));
	            break;
	        case 4:
	            result = rMessage.arg(mArgs.at(0), mArgs.at(1), mArgs.at(2), mArgs.at(3));
	            break;
	        case 5:
	            result = rMessage.arg(mArgs.at(0), mArgs.at(1), mArgs.at(2), mArgs.at(3), mArgs.at(4));
	            break;
	        case 6:
	            result = rMessage.arg(mArgs.at(0), mArgs.at(1), mArgs.at(2), mArgs.at(3), mArgs.at(4), mArgs.at(5));
	            break;
	        case 7:
	            result = rMessage.arg(mArgs.at(0), mArgs.at(1), mArgs.at(2), mArgs.at(3), mArgs.at(4), mArgs.at(5), mArgs.at(6));
	            break;
	        case 8:
	            result = rMessage.arg(mArgs.at(0), mArgs.at(1), mArgs.at(2), mArgs.at(3), mArgs.at(4), mArgs.at(5), mArgs.at(6), mArgs.at(7));
	            break;
	        default:
	            result = rMessage.arg(mArgs.at(0), mArgs.at(1), mArgs.at(2), mArgs.at(3), mArgs.at(4), mArgs.at(5), mArgs.at(6), mArgs.at(7), mArgs.at(8));
	            break;
	    }
	
	    if (mArgs.count() > 9)
	    {
	        int i = 9;
	        while(i < mArgs.count())
	        {
	            result = result.arg(mArgs.at(i));
	            i++;
	        }
	    }
	    */
	    
	    result = rMessage;
	    QVariant arg;
	    Q_FOREACH(arg, mArgs)
	        result = result.arg(arg.toString());
	    return result;
	}
	
	
	QString LogError::cleanMessage(const QString &rMessage)
	{
	    if (rMessage.isEmpty())
	        return rMessage;
	    
	    QString result = rMessage;
	    if (rMessage.at(rMessage.size() - 1) == QLatin1Char('.'))
	        result = rMessage.left(rMessage.size() - 1);
	    return result;
	}
	
		
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
#ifndef QT_NO_DATASTREAM
	QDataStream &operator<<(QDataStream &rStream,
	                        const LogError &rLogError)
	{
		QBuffer buffer;
		buffer.open(QIODevice::WriteOnly);
		QDataStream stream(&buffer);
		
		// version
		quint16 version = 0; 
		stream << version;
		// version 0 data
		stream << rLogError.mCode
		       << rLogError.mContext
		       << rLogError.mMessage
		       << rLogError.mSymbol
		       << rLogError.mArgs
		       << rLogError.mCausingErrors;
		
		buffer.close();
		rStream << buffer.buffer();
		return rStream; 
	}
	
	
	QDataStream &operator>>(QDataStream &rStream, 
	                        LogError &rLogError)
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
		QString level;
		QString logger;
		stream >> rLogError.mCode
		       >> rLogError.mContext
		       >> rLogError.mMessage
		       >> rLogError.mSymbol
		       >> rLogError.mArgs
		       >> rLogError.mCausingErrors;
		
		buffer.close();
		return rStream; 
	}
#endif // QT_NO_DATASTREAM
	
	
#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug, 
	                  const LogError &rLogError)
	{
	    // Escape % sign
	    QString message = rLogError.message();
	    message.replace(QLatin1String("%"), QLatin1String("%%"));
	    
	    debug.nospace() << "LogError(" 
	        << "code:" << rLogError.code() << " "
	        << "context:" << rLogError.context() << " "
	        << "message:" << message << " "
	        << "symbol:" << rLogError.symbol() << " "
	        << "args:" << rLogError.args()
	        << "translatedMessage:" << rLogError.translatedMessage() 
	        << ")";
	    return debug.maybeSpace();
	}
#endif // QT_NO_DEBUG_STREAM
	
	
} // namespace Log4Qt
