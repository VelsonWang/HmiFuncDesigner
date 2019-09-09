/******************************************************************************
 *
 * package:     Log4Qt
 * file:        writerappender.h
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

#ifndef LOG4QT_WRITERAPPENDER_H
#define LOG4QT_WRITERAPPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/appenderskeleton.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

class QTextCodec;
class QTextStream;

namespace Log4Qt 
{
	
	/*!
	 * \brief The class WriterAppender appends log events to a QTextStream.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * &nbsp;
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class WriterAppender : public AppenderSkeleton
	{
	    Q_OBJECT
	
	    /*!
	     * The property holds the codec the appender uses.
	     *
	     * The default is null to use the codec the writer has set.
	     * 
	     * \sa encoding(), setEncoding()
	     */
	    Q_PROPERTY(QTextCodec* encoding READ encoding WRITE setEncoding)
	
	    /*!
	     * The property holds the writer the appender uses.
	     *
	     * \sa writer(), setWriter()
	     */
	    Q_PROPERTY(QTextStream* writer READ writer WRITE setWriter)
	
	    /*!
	     * The property holds, if the writer flushes after all write operations.
	     *
	     * The default is true for flushing.
	     *
	     * \sa immediateFlush(), setImmediateFlush()
	     */
	    Q_PROPERTY(bool immediateFlush READ immediateFlush WRITE setImmediateFlush)
	
	public:
	    WriterAppender(QObject *pParent = 0);
	    WriterAppender(Layout *pLayout, 
	                   QObject *pParent = 0);
	    WriterAppender(Layout *pLayout, 
	                   QTextStream *pTextStream,
	                   QObject *pParent = 0);
	    virtual ~WriterAppender();
	private:
	    WriterAppender(const WriterAppender &rOther); // Not implemented
	    WriterAppender &operator=(const WriterAppender &rOther); // Not implemented
	        
	public:
	    virtual bool requiresLayout() const;
	    QTextCodec *encoding() const;
	    bool immediateFlush() const;
	    QTextStream *writer() const;
	    
	    /*!
	     * Sets the codec used by the writer to \a pTextCoded. 
	     *
	     * If a codec is set with setEncoding, it will overwrite the codec set 
	     * in the text stream. A subsequent call with \a pTextCoded equals null
	     * will resets the codec to the default QTextCodec::codecForLocale().  
	     * 
	     * \sa encoding(), QTextSream::setCodec(), QTextCodec::codecForLocale()
	     */
	    void setEncoding(QTextCodec *pTextCodec); 
	    void setImmediateFlush(bool immediateFlush);
	    void setWriter(QTextStream *pTextStream);
	    
	    virtual void activateOptions();
	    virtual void close();
	    
	protected:
	    virtual void append(const LoggingEvent &rEvent);
	
	    /*!
	     * Tests if all entry conditions for using append() in this class are 
	     * met.
	     * 
	     * If a conditions is not met, an error is logged and the function 
	     * returns false. Otherwise the result of 
	     * AppenderSkeleton::checkEntryConditions() is returned.
	     * 
	     * The checked conditions are:
	     * - A writer has been set (APPENDER_USE_MISSING_WRITER_ERROR)
	     * 
	     * The function is called as part of the checkEntryConditions() chain 
	     * started by AppenderSkeleton::doAppend().
	     * 
	     * \sa AppenderSkeleton::doAppend(), 
	     *     AppenderSkeleton::checkEntryConditions()
	     */
	    virtual bool checkEntryConditions() const;
	    
	    void closeWriter();
	    
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %WriterAppender(name:"WA" encoding:"" immediateFlush:true 
	     *                 isactive:false isclosed:false layout:"TTCC" 
	     *                 referencecount:1 threshold:"NULL"  
	     *                 writer:0x0) 
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject	)
	     */
	    virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	
	    virtual bool handleIoErrors() const;
	    void writeFooter() const; 
	    void writeHeader() const;
	    
	private:
	    QTextCodec *mpEncoding;
	    QTextStream *mpWriter;
	    volatile bool mImmediateFlush;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline QTextCodec *WriterAppender::encoding() const
	{   QMutexLocker locker(&mObjectGuard);
		return mpEncoding; }
	
	inline bool WriterAppender::immediateFlush() const
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		return mImmediateFlush; }
	
	inline QTextStream *WriterAppender::writer() const
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		return mpWriter;  }
	
	inline void WriterAppender::setImmediateFlush(bool immediateFlush)
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		mImmediateFlush = immediateFlush;   }
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::WriterAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_WRITERAPPENDER_H
