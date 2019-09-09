/******************************************************************************
 *
 * package:     Log4Qt
 * file:        patternlayout.cpp
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


#include "log4qt/patternlayout.h"

#include <QtCore/QDebug>
#include "log4qt/helpers/patternformatter.h"
#include "log4qt/loggingevent.h"



namespace  Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * Class implementation: PatternLayout
	 **************************************************************************/
	
	
	PatternLayout::PatternLayout(QObject *pParent) :
	    Layout(pParent),
	    mPattern(),
	    mpPatternFormatter(0)
	{
		setConversionPattern(DEFAULT_CONVERSION_PATTERN);
	}
	
	
	PatternLayout::PatternLayout(const QString &rPattern,
	                             QObject *pParent) :
	    Layout(pParent),
	    mPattern(),
	    mpPatternFormatter(0)
	{
		setConversionPattern(rPattern);
	}
	
	
	PatternLayout::PatternLayout(ConversionPattern conversionPattern,
	                             QObject *pParent) :
	    Layout(pParent),
	    mPattern(),
	    mpPatternFormatter(0)
	{
		setConversionPattern(conversionPattern);
	}
	
	
	PatternLayout::~PatternLayout()
	{
		delete mpPatternFormatter;
	}
	
	
	void PatternLayout::setConversionPattern(ConversionPattern conversionPattern)
	{
		switch (conversionPattern)
		{
		    case DEFAULT_CONVERSION_PATTERN:
		    	setConversionPattern(QLatin1String("%m%n"));
		        break;
		    case TTCC_CONVERSION_PATTERN:
		    	setConversionPattern(QLatin1String("%r [%t] %p %c %x - %m%n"));
		        break;
		    default:
		        Q_ASSERT_X(false, "PatternLayout::setConversionFormat", "Unkown ConversionFormat");
		        setConversionPattern(QString());
		}
	}
	
	
	QString PatternLayout::format(const LoggingEvent &rEvent)
	{
		Q_ASSERT_X(mpPatternFormatter, "PatternLayout::format()", "mpPatternConverter must not be null");
		
		return mpPatternFormatter->format(rEvent);
	}
	
	
	void PatternLayout::updatePatternFormatter()
	{
		delete mpPatternFormatter;
		mpPatternFormatter = new PatternFormatter(mPattern);
	}
	
	
#ifndef QT_NO_DEBUG_STREAM
	QDebug PatternLayout::debug(QDebug &rDebug) const
	{
	    rDebug.nospace() << "PatternLayout(" 
	    	<< "name:" << name() << " "
	    	<< "pattern:" << conversionPattern() << " "
	    	<< "referencecount:" << referenceCount()
	        << ")";
	    return rDebug.space();    
	}
#endif // QT_NO_DEBUG_STREAM
	
	
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
} // namespace Log4Qt
