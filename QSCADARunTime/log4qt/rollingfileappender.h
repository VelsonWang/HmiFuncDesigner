/******************************************************************************
 *
 * package:     Log4Qt
 * file:        rollingfileappender.h
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

#ifndef LOG4QT_ROLINGFILEAPPENDER_H
#define LOG4QT_ROLINGFILEAPPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/fileappender.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	/*!
	 * \brief The class RollingFileAppender extends FileAppender to backup 
	 *        the log files when they reach a certain size.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 *    
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class RollingFileAppender : public FileAppender
	{
	    Q_OBJECT
	
	    /*!
	     * The property holds the maximum backup count used by the appender.
	     *
	     * The default is 1.
	     *
	     * \sa maxBackupIndex(), setMaxBackupIndex()
	     */
	    Q_PROPERTY(int maxBackupIndex READ maxBackupIndex WRITE setMaxBackupIndex)
	    
        /*!
         * The property holds the maximum file size used by the appender.
         *
         * The default is 10 MB (10 * 1024 * 1024).
         *
         * \sa maximumFileSize(), setMaximumFileSize()
         */
        Q_PROPERTY(qint64 maximumFileSize READ maximumFileSize WRITE setMaximumFileSize)
        
        /*!
         * The property sets the maximum file size from a string value.
         *
         * \sa setMaxFileSize(), maximumFileSize()
         */
        Q_PROPERTY(QString maxFileSize WRITE setMaxFileSize)
        
	public:
	    RollingFileAppender(QObject *pParent = 0);
	    RollingFileAppender(Layout *pLayout,
	                        const QString &rFileName,
	                        QObject *pParent = 0);
	    RollingFileAppender(Layout *pLayout,
	                        const QString &rFileName,
	                        bool append,
	                        QObject *pParent = 0);
	    virtual ~RollingFileAppender();
	private:
	    RollingFileAppender(const RollingFileAppender &rOther); // Not implemented
	    RollingFileAppender &operator=(const RollingFileAppender &rOther); // Not implemented
	        
	public:
	    int maxBackupIndex() const;
	    qint64 maximumFileSize() const;
	    void setMaxBackupIndex(int maxBackupIndex);
        void setMaximumFileSize(qint64 maximumFileSize);
        void setMaxFileSize(const QString &rMaxFileSize);
	
	protected:
	    virtual void append(const LoggingEvent &rEvent);
	
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %RollingFileAppender(name:"RFA" appendfile:false bufferedio:true 
	     *                      encoding:"" file:"/log.txt" filter: 0x0 
	     *                      immediateflush:true isactive:true 
	     *                      isclosed:false layout:"TTCC" maxbackupindex:2  
	     *                      maximumfilesize:40 referencecount:1 
	     *                      threshold:"NULL" writer:0x4175af8) 
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	    virtual QDebug debug(QDebug &rDebug) const;	
#endif // QT_NO_DEBUG_STREAM
	
	private:
	    void rollOver();
	    
	private:
	    int mMaxBackupIndex;
	    qint64 mMaximumFileSize;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline int RollingFileAppender::maxBackupIndex() const
	{   QMutexLocker locker(&mObjectGuard);
		return mMaxBackupIndex; }
	
	inline qint64 RollingFileAppender::maximumFileSize() const
	{   QMutexLocker locker(&mObjectGuard);
		return mMaximumFileSize;    }
	
	inline void RollingFileAppender::setMaxBackupIndex(int maxBackupIndex)
	{   QMutexLocker locker(&mObjectGuard);
		mMaxBackupIndex = maxBackupIndex;   }
	
	inline void RollingFileAppender::setMaximumFileSize(qint64 maximumFileSize)
	{   QMutexLocker locker(&mObjectGuard);
		mMaximumFileSize = maximumFileSize; }
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::RollingFileAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_ROLINGFILEAPPENDER_H
