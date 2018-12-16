/******************************************************************************
 *
 * package:     Log4Qt
 * file:        mdc.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * changes      Feb 2009, Martin Heinrich
 *              - Fixed unreferenced formal parameter warning by using
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


#include "log4qt/mdc.h"

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
 
    
	
	/**************************************************************************
	 * Class implementation: MDC
	 **************************************************************************/
	
	
	QString MDC::get(const QString &rKey)
	{
	    if (!instance()->mHash.hasLocalData())
	        return QString();
	
	    return instance()->mHash.localData()->value(rKey);
	}
	
	
	QHash<QString, QString> MDC::context()
	{
	    if (!instance()->mHash.hasLocalData())
	        return QHash<QString, QString>();
	    
	    return *instance()->mHash.localData();
	}
	
	
    LOG4QT_IMPLEMENT_INSTANCE(MDC)
    

	QHash<QString, QString> *MDC::localData()
	{
	    if (!instance()->mHash.hasLocalData())
	        instance()->mHash.setLocalData(new QHash<QString, QString>);
	    return instance()->mHash.localData();
	}   

	
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug, const MDC &rMDC)
	{
		Q_UNUSED(rMDC); // To avoid warning C4100 on VS 2008
	    debug.nospace() << "MDC(" 
	        << "thread:" << QThread::currentThread()->objectName() << " "
	        << "context:" << rMDC.context()
	        << ")";
	    return debug.space();
	}
#endif // QT_NO_DEBUG_STREAM
	
	
} // namespace Log4Qt
