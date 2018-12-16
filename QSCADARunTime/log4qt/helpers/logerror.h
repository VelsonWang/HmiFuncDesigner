/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logerror.h
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

#ifndef LOG4QT_LOGERROR_H
#define LOG4QT_LOGERROR_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QVariant>


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{    
	/*!
	 * Creates an LogError object with the error message \a message, the error 
	 * code \a code and the context \a context. The symbol of the error is 
	 * set to \a code as string value.
	 * 
	 * The following example logs an error, if a character is not a digit.
	 * 
	 * \code
	 * if (!c.isDigit())
     * {
     *     Error e = LOG4QT_ERROR(QT_TR_NOOP("Found character '%1' where digit was expected."),
     *                            LAYOUT_EXPECTED_DIGIT_ERROR,
     *                            "Log4Qt::PatternFormatter");
     *     e << QString(c);
	 *     logger()->error(e);
     * }                          
	 * \endcode
	 */
	#define LOG4QT_ERROR(message, code, context)                              \
		LogError(message, code, #code, context)

	/*!
	 * Creates an LogError object with the error message \a message and the 
	 * error code \a code. The symbol of the error is set to \a code as string 
	 * value. The context is set to the class name of the current object. The 
	 * current objects class must be derived from QObject.
	 * 
	 * The following example handles an error while opening a file.
	 * 
	 * \code
	 * if (!mpFile->open(mode))
	 * {
	 *      LogError e = LOG4QT_QCLASS_ERROR(QT_TR_NOOP("Unable to open file '%1' for appender '%2'"),
     *                                       APPENDER_OPENING_FILE_ERROR);
	 *      e << mFileName << name();
	 *      e.addCausingError(LogError(mpFile->errorString(), mpFile->error()));
	 *      logger()->error(e);
	 *      return;
	 * }
	 * \endcode
	 */
	#define LOG4QT_QCLASS_ERROR(message, code)                                \
		LogError(message, code, #code, this->metaObject()->className())

	/*!
	 * \brief The class LogError represents an error.
	 * 
	 * The class error allows storing error information in a structured way. 
	 * The error message is stored separately from the information that may be 
	 * substituted into the message string. This way it is possible to access 
	 * all information after the error has been raised. It also allows to 
	 * translate the error at a later point in time or to get a translated and 
	 * a not translated error text (e.g. translated for the UI and not 
	 * translated for a log).
	 * 
	 * The message is accessed using message() and setMessage(). Arguments for 
	 * the message can be added using addArg() or operator<<(). The arguments 
	 * can be retrieved using args(). The message with substituted arguments 
	 * is returned by messageWithArgs().
	 * 
	 * An error code can be set as integer value code() and/or a symbolic value
	 * symbol(). 
	 * 
	 * To allow the translation of the message the error stores the translation 
	 * context (context(), setContext()). The translated message can be accessed
	 * using translatedMessage() or using translatedMessageWithArgs(), if it 
	 * should contain the arguments.
	 * 
	 * An error can have one or more related errors that caused it. An error is
	 * related using addCausingError(). All causing errors can be retrieved using
	 * causingErrors().
	 * 
	 * A per thread error can be maintained using lastError() and setLastError().
	 * 
	 * There are two macros avaiable to simplify the error creation. The macro 
	 * \ref Log4Qt::LOG4QT_ERROR "LOG4QT_ERROR" is used with classes not derived 
	 * from QObject. The macro \ref Log4Qt::LOG4QT_QCLASS_ERROR "LOG4QT_QCLASS_ERROR"
	 * is used with classes derived from QObject.
	 */
	class LogError
	{
	public:
		
	    /*!
	     * The enum Encoding defines the 8-bit encoding of a character string 
	     * arguments to \ref LogError::LogError(const char *, int, const char *, 
	     * const char *, Encoding) "LogError::LogError()".
	     * 
	     * \sa \ref LogError::LogError(const char *, int, const char *, const char *, Encoding) "LogError::LogError()"
	     */
	    enum Encoding
	    {
	    	/*! LATIN-1 */
	        LATIN1,
	    	/*! 
	    	 * The encoding specified by QTextCodec::codecForTr()
	    	 * (Latin-1 if none has been set). 
	    	 */
	        CODECFORTR,
	    	/*! UTF-8 */
            UNICODEUTF8
	    };
	    Q_ENUMS(Encoding)

	    /*!
		 * Creates an empty error. The error code is set to 0 and all other 
		 * members are set to be empty.
		 * 
		 * \sa isEmpty()
		 */
		LogError();
		
		/*!
		 * Creates an error with the Message \a rMessage and the error code 
		 * \a code. The symbol for the error code is set to \a rSymbol and the
		 * context to \a rContext. 
		 * 
		 * \a rContext must be string that can be converted to Latin-1. The 
		 * Latin-1 representation of the string is used with 
		 * QApplication::translate(), if a translation for \a rMessage is 
		 * requested.
		 * 
		 * \sa translatedMessage(), translatedMessageWithArgs()
		 */
	    LogError(const QString &rMessage, 
	             int code = 0,
	             const QString &rSymbol = QString(), 
	             const QString &rContext = QString());
	    
		/*!
		 * Creates an error with the Message \a pMessage and the error code 
		 * \a code. The symbol for the error code is set to \a pSymbol and the
		 * context to \a pContext. 
		 * 
		 * \a encoding specifies the encoding of \a pMessage. \a pSymbol and 
		 * \a pContext are expected to be Latin-1.
		 * 
		 * \note To support the macros \ref Log4Qt::LOG4QT_ERROR "LOG4QT_ERROR"
		 *       and \ref Log4Qt::LOG4QT_QCLASS_ERROR "LOG4QT_QCLASS_ERROR"
		 *       the function tests, if \a pSymbol is the string representation of 
		 *       \a code. If it is, the symbol is set to be empty. Otherwise symbol 
		 *       is set to \a pSymbol.
		 * 
		 * \sa translatedMessage(), translatedMessageWithArgs()
		 */
	    LogError(const char *pMessage, 
	             int code = 0, 
	             const char *pSymbol = 0, 
	             const char *pContext = 0,
	             Encoding encoding = LATIN1);
	    
	    // LogError(const LogError &rOther); // Use compiler default
	    // virtual ~LogError(); // Use compiler default
	    // LogError &operator=(const LogError &rOther); // Use compiler default
	    
	    /*!
	     * Returns the error code.
	     * 
	     * \sa setCode()
	     */
	    int code() const;
	    
	    /*!
	     * Returns the context for the error.
	     * 
	     * \sa setContext()
	     */
	    QString context() const;
	    
	    /*!
	     * Returns the error message.
	     * 
	     * \sa setMessage()
	     */
	    QString message() const;
	    
	    /*!
	     * Returns the symbol for the error code.
	     * 
	     * \sa setSymbol()
	     */
	    QString symbol() const;
	    
	    /*!
	     * Returns the translated error message.
	     * 
	     * The translated message is created by calling 
	     * QCoreApplication::translate() using context().toLatin1() as 
	     * context and message.toUtf8() as message.
	     * 
	     * \sa translatedMessageWithArgs()
	     */
	    QString translatedMessage() const;

	    /*!
	     * Sets the error code to \a code.
	     * 
	     * \sa code()
	     */
	    void setCode(int code);
	    
	    /*!
	     * Sets the context to \a rClassName. 		 
	     * 
	     * \a rContext must be string that can be converted to Latin-1. The 
		 * Latin-1 representation of the string is used with 
		 * QApplication::translate(), if a translation for \a rMessage is 
		 * requestd.
	     * 
		 * \sa context(), translatedMessage(), translatedMessageWithArgs()
	     */
	    void setContext(const QString &rClassName);
	    
	    /*!
	     * Sets the error message to \a rMessage
	     * 
	     * \sa message()
	     */
	    void setMessage(const QString &rMessage);
	    
	    /*!
	     * Sets the symbol for the error code to \a rSymbol.
	     * 
	     * \sa symbol()
	     */
	    void setSymbol(const QString &rSymbol);
	    
	    /*!
	     * Returns the last error set for the current thread using 
	     * setLastError().
	     * 
	     * \note: This function is thread-safe.
	     * 
	     * \sa setLastError()
	     */
	    static LogError lastError();
	    
	    /*!
	     * Sets the last error for the current thread to \a rLogError.
	     * 
	     * \note: This function is thread-safe.
	     * 
	     * \sa lastError()
	     */
	    static void setLastError(const LogError &rLogError);
	    
	    /*!
	     * Appends \a rArg to the list of arguments and returns a reference to 
	     * this error.
	     * 
	     * \sa operator<<(), args(), clearArgs()
	     */
	    LogError &addArg(const QVariant &rArg);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    LogError &addArg(int arg);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    LogError &addArg(const QString &rArg);
	    
	    /*!
	     * Appends \a rLogError to the list of causing errors and returns a 
	     * reference to this error.
	     * 
	     * \sa causingErrors(), clearCausingErrors()
	     */
	    LogError &addCausingError(const LogError &rLogError);
	    
	    /*!
	     * Returns the list of arguments that have been added to this error.
	     * 
	     * \sa addArg(), operator<<(), clearArgs()
	     */
	    QList<QVariant> args() const;
	    
	    /*!
	     * Returns the list of causing errors that have been added to this error.
	     * 
	     * \sa addArg(), operator<<(), clearArgs()
	     */
	    QList<LogError> causingErrors() const;
	    
	    /*!
	     * Clears the list of arguments that have been added to this error.
	     * 
	     * \sa addArg(), operator<<(), args()
	     */
	    void clearArgs();
	    
	    /*!
	     * Clears the list of causing errors that have been added to this error.
	     * 
	     * \sa addCausingError(), causingErrors()
	     */
	    void clearCausingErrors();
	    
	    /*!
	     * Returns true, if the error code is 0 and the message is empty. 
	     * Otherwise it returns false.
	     * 
	     * \sa code(), message()
	     */
	    bool isEmpty() const;
	    
	    /*!
	     * Returns the message with arguments. The arguments are incoorporated
	     * into the messag using QString::arg().
	     * 
	     * \sa QString::arg(), translatedMessageWithArgs()
	     */
	    QString messageWithArgs() const;
	    
	    /*!
	     * Returns the translated message with arguments. The arguments are 
	     * incoorporated into the messag using QString::arg().
	     * 
	     * \sa QString::arg(), messageWithArgs(), translatedMessage()
	     */
	    QString translatedMessageWithArgs() const;
	    
	    /*!
	     * Appends \a rArg to the list of arguments and returns a reference to 
	     * this error.
	     * 
	     * \sa addArg()
	     */
	    LogError &operator<<(const QVariant &rArg);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    LogError &operator<<(int arg);
	    
	    /*!
	     * This is an overloaded member function, provided for convenience.
	     */
	    LogError &operator<<(const QString &rArg);
	    
	    /*!
	     * Returns a string representation of the error.
	     * 
	     * The string has the following format:
	     * 
	     * <tt> 
	     * message (context::symbol, code): causing_error, causing_error
	     * </tt>
	     * 
	     * If members are empty they are omitted:
	     * - Omit context, if empty
	     * - Omit symbol, if empty
	     * - Omit double colon with context and symbol, if both are empty
	     * - Omit code, if 0
	     * - Omit bracket with context/symbol and code, if all are empty
	     * - Omit colon with causing errors, if no causing errors exist
	     */
	    QString toString() const;
	
	private:
	    QString insertArgs(const QString &rMessage) const;
	    QString cleanMessage(const QString &rMessage);
	   
	private:
	    int mCode;
	    QString  mContext;
	    QString  mMessage;
	    QString  mSymbol;
	    QList<QVariant> mArgs;
	    QList<LogError> mCausingErrors;

#ifndef QT_NO_DATASTREAM
	    // Needs to be friend to stream objects
	    friend QDataStream &operator<<(QDataStream &rStream, 
                                       const LogError &rLogError);
	    friend QDataStream &operator>>(QDataStream &rStream, 
	                                   LogError &rLogError);
#endif // QT_NO_DATASTREAM
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
#ifndef QT_NO_DATASTREAM
	/*!
	 * \relates LogError
	 * 
	 * Writes the given error \a rLogError to the given stream \a rStream, 
	 * and returns a reference to the stream.
	 */
	QDataStream &operator<<(QDataStream &rStream, 
	                        const LogError &rLogError);
	
	/*!
	 * \relates LogError
	 * 
	 * Reads an error from the given stream \a rStream into the given 
	 * error \a rLogError, and returns a reference to the stream.
	 */
	QDataStream &operator>>(QDataStream &rStream, 
	                        LogError &rLogError);
#endif // QT_NO_DATASTREAM
	
#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates LogError
	 * 
	 * Writes all object member variables to the given debug stream \a debug and
	 * returns the stream.
	 *
	 * <tt> 
	 * %LogError(code:7 context:"Log4Qt::FileAppender" 
	 *           message:"Unable to open file '%1' for appender '%2'" 
	 *           symbol:"APPENDER_OPENING_FILE_ERROR" 
	 *           args:(QVariant(QString, "G:\logs\client.log") ,  QVariant(QString, "Client FileAppender") )
	 *           translatedMessage: "Unable to open file '%1' for appender '%2'" )  
	 * </tt>
	 * 
	 * \sa QDebug
	 */
	QDebug operator<<(QDebug debug, 
	                  const LogError &rLogError);
#endif // QT_NO_DEBUG_STREAM
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline int LogError::code() const
	{   return mCode;   }
	  
	inline QString LogError::context() const
	{   return mContext;  }
	
	inline QString LogError::message() const
	{   return mMessage;    }
	
	inline QString LogError::symbol() const
	{   return mSymbol;    }
	
	inline void LogError::setCode(int code)
	{   mCode = code;    }
	
	inline void LogError::setContext(const QString &rContext)
	{   mContext = rContext;   }
	
	inline void LogError::setMessage(const QString &rMessage)
	{   mMessage = cleanMessage(rMessage);   }
	
	inline void LogError::setSymbol(const QString &rSymbol)
	{   mSymbol = rSymbol;    }
	
	inline LogError &LogError::addArg(const QVariant &rArg)
	{   mArgs << rArg; return *this; }
	
	inline LogError &LogError::addArg(int arg)
	{   mArgs << QVariant(arg); return *this;   }
	
	inline LogError &LogError::addArg(const QString &rArg)
	{   mArgs << QVariant(rArg); return *this;   }
	
	inline LogError &LogError::addCausingError(const LogError &rLogError)
	{   mCausingErrors << rLogError;  return *this;    }
	
	inline QList<QVariant> LogError::args() const
	{   return mArgs;   }
	
	inline void LogError::clearArgs()
	{	mArgs.clear();	}
	
	inline void LogError::clearCausingErrors()
	{	mCausingErrors.clear();	}
	
	inline QList<LogError> LogError::causingErrors() const
	{   return mCausingErrors;  }
	
	inline bool LogError::isEmpty() const
	{   return mCode || !mMessage.isEmpty(); }
	
	inline QString LogError::messageWithArgs() const
	{   return insertArgs(message());    }
	
	inline QString LogError::translatedMessageWithArgs() const
	{   return insertArgs(translatedMessage());    }
	
	inline LogError &LogError::operator<<(const QVariant &rArg)
	{   return addArg(rArg);  }
	
	inline LogError &LogError::operator<<(int arg)
	{   return addArg(arg);  }
	
	inline LogError &LogError::operator<<(const QString &rArg)
	{   return addArg(rArg);  }
	
	
} // namespace Log4Qt


Q_DECLARE_METATYPE(Log4Qt::LogError)
Q_DECLARE_TYPEINFO(Log4Qt::LogError, Q_MOVABLE_TYPE);


#endif // LOG4QT_ERROR_H
