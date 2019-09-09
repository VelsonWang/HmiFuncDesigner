/******************************************************************************
 *
 * package:     Log4Qt
 * file:        consoleappender.cpp
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
 ******************************************************************************/


#include "log4qt/consoleappender.h"

#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include "log4qt/helpers/optionconverter.h"
#include "log4qt/layout.h"
#include "log4qt/loggingevent.h"



namespace Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * Class implementation: ConsoleAppender
	 **************************************************************************/
	
	
	ConsoleAppender::ConsoleAppender(QObject *pParent) :
	    WriterAppender(pParent),
	    mTarget(STDOUT_TARGET),
	    mpTextStream(0)
	{
	}
	
	
	ConsoleAppender::ConsoleAppender(Layout *pLayout,
	                                 QObject *pParent) :
	    WriterAppender(pLayout, pParent),
	    mTarget(STDOUT_TARGET),
	    mpTextStream(0)
	{
	}
	
	                                	 
    ConsoleAppender::ConsoleAppender(Layout *pLayout,
                                     const QString &rTarget,
                                     QObject *pParent) :
        WriterAppender(pLayout, pParent),
        mTarget(STDOUT_TARGET),
        mpTextStream(0)
    {
        setTarget(rTarget);
    }

    
    ConsoleAppender::ConsoleAppender(Layout *pLayout,
	                                 Target target,
	                                 QObject *pParent) :
	    WriterAppender(pLayout, pParent),
	    mTarget(target),
	    mpTextStream(0)
	{
	}
	

	ConsoleAppender::~ConsoleAppender()
	{
	    close();
	}

	
    QString ConsoleAppender::target() const
    {   
        // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe

        if (mTarget == STDOUT_TARGET)
            return QLatin1String("STDOUT_TARGET");
        else
            return QLatin1String("STDERR_TARGET");
    }
    
    
    void ConsoleAppender::setTarget(const QString &rTarget)
    {
        bool ok;
        Target target = (Target)OptionConverter::toTarget(rTarget, &ok);
        if (ok)
            setTarget(target);
    }
    

	void ConsoleAppender::activateOptions()
	{
	    QMutexLocker locker(&mObjectGuard);
	    
	    closeStream();
	    
	    if (mTarget == STDOUT_TARGET)
	        mpTextStream = new QTextStream(stdout);
	    else
	        mpTextStream = new QTextStream(stderr);
	    setWriter(mpTextStream);
	    
	    WriterAppender::activateOptions();
	}
	
	
	void ConsoleAppender::close()
	{
	    QMutexLocker locker(&mObjectGuard);
	
	    if (isClosed())
	        return;
	
	    WriterAppender::close();
	    closeStream();
	}
	
	
	void ConsoleAppender::closeStream()
	{
	    // Q_ASSERT_X(, "ConsoleAppender::closeStream()", "Lock must be held by caller")
	
	    setWriter(0);
	    delete mpTextStream;
	    mpTextStream = 0;
	}
	
	
#ifndef QT_NO_DEBUG_STREAM
	QDebug ConsoleAppender::debug(QDebug &rDebug) const
	{
	    QString layout_name;
	    if (layout())
	        layout_name = layout()->name();
	    QString target;
	    if (mTarget == STDOUT_TARGET)
	        target = QLatin1String("STDOUT");
	    else
	        target = QLatin1String("STDERR");
	    
	    rDebug.nospace() << "ConsoleAppender(" 
	        << "name:" << name() << " "
	        << "filter:" << firstFilter() << " "
	        << "isactive:" << isActive() << " "
	        << "isclosed:" << isClosed() << " "
	        << "layout:" << layout_name << " "
	        << "target:" << target << " "
	        << "referencecount:" << referenceCount() << " "
	        << "threshold:" << threshold().toString()
	        << ")";
	    return rDebug.space();    
	}
#endif // QT_NO_DEBUG_STREAM
	
	
	
	/******************************************************************************
	 * Implementation: Operators, Helper
	 ******************************************************************************/
	
	
} // namespace Log4Qt
