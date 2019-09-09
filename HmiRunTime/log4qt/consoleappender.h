/******************************************************************************
 *
 * package:     Log4Qt
 * file:        consoleappender.h
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

#ifndef LOG4QT_CONSOLEAPPENDER_H
#define LOG4QT_CONSOLEAPPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/writerappender.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

class QFile;
class QTextStream;

namespace Log4Qt
{
	
	/*!
	 * \brief The class ConsoleAppender appends to stdout or stderr.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
	class ConsoleAppender : public WriterAppender
	{
	    Q_OBJECT
	    
	    /*!
	     * The property holds the target used by the appender.
	     *
	     * The default is STDOUT_TARGET for the standard output.
	     * 
	     * \sa Target, target(), setTarget()
	     */
	    Q_PROPERTY(QString target READ target WRITE setTarget)
	
	public:
	    /*!
	     * The enum defines the possible output targets
	     *
	     * \sa target(), setTarget()
	     */
	    enum Target {
	        /*! The output target is standard out. */
	        STDOUT_TARGET,
	        /*! The output target is standard error. */
	        STDERR_TARGET
	    }; 
	    Q_ENUMS(Target)
	    
	    ConsoleAppender(QObject *pParent = 0);
	    ConsoleAppender(Layout *pLayout,
	                    QObject *pParent = 0);
        ConsoleAppender(Layout *pLayout, 
                        const QString &rTarget,
                        QObject *pParent = 0); 

        /*!
         * Creates a ConsoleAppender with the layout \a pLayout, the target 
         * value specified by the \a target constant and the parent 
         * \a pParent.
         */
        ConsoleAppender(Layout *pLayout, 
                        Target target,
                        QObject *pParent = 0); 
        
	    virtual ~ConsoleAppender();
	private:
	    ConsoleAppender(const ConsoleAppender &rOther); // Not implemented
	    ConsoleAppender &operator=(const ConsoleAppender &rOther); // Not implemented
	
	public:
	    // JAVA: bool follow() const;
	    QString target() const;
	    // JAVA: void setFollow(bool follow);
        void setTarget(const QString &rTarget);
        
        /*!
         * Sets the target to the value specified by the \a target constant.
         */
        void setTarget(Target target);
	    
	    virtual void activateOptions();
	    virtual void close();
	 
	protected:
	    void closeStream();
	    
	#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
	     * %ConsoleAppender(name:"CA" filter:0x0 isactive:true isclosed:false 
	     *                  layout:"PL" target:"STDERR" referenceCount:1 
	     *                  threshold:"WARN_SET") 
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	    virtual QDebug debug(QDebug &rDebug) const;
	#endif // QT_NO_DEBUG_STREAM
	
	private:
	    volatile Target mTarget;
	    QTextStream *mpTextStream;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/

    inline void ConsoleAppender::setTarget(Target target)
    {   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
        mTarget = target;   }


} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::ConsoleAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_CONSOLEAPPENDER_H
