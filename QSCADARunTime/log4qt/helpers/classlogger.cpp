/******************************************************************************
 *
 * package:     Log4Qt
 * file:        classlogger.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * changes:		Sep 2008, Martin Heinrich:
 * 				- Replaced usage of q_atomic_test_and_set_ptr with
 * 				  QAtomicPointer
 *
 *
 * Copyright 2007 - 2008 Martin Heinrich
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


#include "log4qt/helpers/classlogger.h"

#include <QtCore/QDebug>
#include "log4qt/logmanager.h"


namespace Log4Qt
{


	/**************************************************************************
	 * Declarations
	 **************************************************************************/



	/**************************************************************************
	 * C helper functions
	 **************************************************************************/



	/**************************************************************************
	 * Class implementation: ClassLogger
	 **************************************************************************/


	ClassLogger::ClassLogger() :
	    mpLogger(0)
	{
	}


	Logger *ClassLogger::logger(const QObject *pObject)
	{
		Q_ASSERT_X(pObject, "ClassLogger::logger()", "pObject must not be null");
#if QT_VERSION < QT_VERSION_CHECK(4, 4, 0)
	    if (!mpLogger)
	        q_atomic_test_and_set_ptr(&mpLogger,
									  0,
									  LogManager::logger(QLatin1String(pObject->metaObject()->className())));
	    return const_cast<Logger *>(mpLogger);
#elif QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        if (!static_cast<Logger *>(mpLogger))
             mpLogger.testAndSetOrdered(0,
                                        LogManager::logger(QLatin1String(pObject->metaObject()->className())));
        return const_cast<Logger *>(static_cast<Logger *>(mpLogger));
#else
        if (!static_cast<Logger *>(mpLogger.loadAcquire()))
			 mpLogger.testAndSetOrdered(0,
										LogManager::logger(QLatin1String(pObject->metaObject()->className())));
        return const_cast<Logger *>(static_cast<Logger *>(mpLogger.loadAcquire()));
#endif
	}



	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/



} // namespace Log4Qt
