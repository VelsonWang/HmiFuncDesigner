/******************************************************************************
 *
 * package:     Log4Qt
 * file:        ndc.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * changes      Feb 2009, Martin Heinrich
 *              - Fixed VS 2008 unreferenced formal parameter warning by using
 *                Q_UNUSED in operator<<.
 *
 *
 * Copyright 2007 - 2009 Martin Heinrich
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


#include "log4qt/ndc.h"

#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include "log4qt/helpers/initialisationhelper.h"
#include "log4qt/logger.h"



namespace Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	

    
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
    LOG4QT_DECLARE_STATIC_LOGGER(logger, Log4Qt:NDC)    
 
	
	
	/**************************************************************************
	 * Class implementation: NDC
	 **************************************************************************/
	
	
	void NDC::clear()
	{
	    if (!instance()->mStack.hasLocalData())
	        return;
	    
	    instance()->mStack.localData()->clear();
	}    
	
	
	int NDC::depth()
	{
	    if (!instance()->mStack.hasLocalData())
	        return 0;
	        
	    return instance()->mStack.localData()->count();
	}
    
	
    LOG4QT_IMPLEMENT_INSTANCE(NDC)
	
	
	QString NDC::pop()
	{
	    if (!instance()->mStack.hasLocalData() || instance()->mStack.localData()->isEmpty())
	    {
	        logger()->warn("Requesting pop from empty NDC stack");        
	        return QString();
	    }
	    
	    return instance()->mStack.localData()->pop();
	}
	
	
	void NDC::push(const QString &rMessage)
	{
	    if (!instance()->mStack.hasLocalData())
	        instance()->mStack.setLocalData(new QStack<QString>);
	    
	    instance()->mStack.localData()->push(rMessage);
	}
	
	
	void NDC::setMaxDepth(int maxDepth)
	{
	    if (!instance()->mStack.hasLocalData() || 
	        instance()->mStack.localData()->size() <= maxDepth)
	        return;
	
	    instance()->mStack.localData()->resize(maxDepth);
	}
	
	
	QString NDC::peek()
	{
	    if (!instance()->mStack.hasLocalData() || 
	        instance()->mStack.localData()->isEmpty())
	        return QString();
	    
	    return instance()->mStack.localData()->top();
	}
    
    
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug,
	                  const NDC &rNDC)
	{
		Q_UNUSED(rNDC); // To avoid warning C4100 on VS 2008
	    debug.nospace() << "NDC(" 
	        << "thread:" << QThread::currentThread()->objectName() << " "
	        << "peek:" << rNDC.peek() << " "
	        << "depth:" << rNDC.depth()
	        << ")";
	    return debug.space();
	}
#endif // QT_NO_DEBUG_STREAM
	
	
} // namespace Log4Qt
