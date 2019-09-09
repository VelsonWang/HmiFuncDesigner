/******************************************************************************
 *
 * package:     Log4Qt
 * file:        layout.cpp
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


#include "log4qt/layout.h"

#include <QtCore/QDebug>
#include "log4qt/loggingevent.h"
#include "log4qt/logmanager.h"



namespace Log4Qt
{
	
	
	/***************************************************************************
	 * Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * Class implementation: Layout
	 **************************************************************************/
	
	
	QString Layout::contentType() const
	{
	    return QString::fromLatin1("text/plain");
	}
	    
	
	void Layout::activateOptions()
	{
	}
	
	
	QString Layout::endOfLine()
	{
		// There seams to be no function in Qt for this
		
#ifdef Q_OS_WIN32
		return QLatin1String("\r\n");
#endif // Q_OS_WIN32
//#ifdef Q_OS_MAC
//		return QLatin1String("\r");
//#endif // Q_OS_MAC
		return QLatin1String("\n");
	}
	
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
} // namespace Log4Qt
