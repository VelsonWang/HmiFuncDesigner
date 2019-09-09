/******************************************************************************
 * 
 * package:     Log4Qt
 * file:        basicconfigurator.h
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

#ifndef LOG4QT_BASICCONFIGURATOR_H
#define LOG4QT_BASICCONFIGURATOR_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/


#include <QtCore/Qt>
#include "log4qt/log4qt.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	class Appender;
	
	/*!
	 * \brief The class BasicConfigurator provides a simple package 
	 *        configuration.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 */
	class BasicConfigurator
	{
	private:
	    BasicConfigurator(); // Not implemented
	    // BasicConfigurator(const BasicConfigurator &rOther); // Use compiler default
	    // virtual ~BasicConfigurator(); // Use compiler default
	    // BasicConfigurator &operator=(const BasicConfigurator &rOther); // Use compiler default
	    
	public:
	    static bool configure();
	    static void configure(Appender *pAppender);
	    static void resetConfiguration();
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	
} // namspace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::BasicConfigurator, Q_MOVABLE_TYPE); // Use default


#endif // LOG4QT_BASICCONFIGURATOR_H
