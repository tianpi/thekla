// Module:  Log4CPLUS
// File:    rootlogger.cxx
// Created: 4/2003
// Author:  Tad E. Smith
//
//
// Copyright (C) Tad E. Smith  All rights reserved.
//
// This software is published under the terms of the Apache Software
// License version 1.1, a copy of which has been included with this
// distribution in the LICENSE.APL file.
//
// $Log: rootlogger.cxx,v $
// Revision 1.1  2006-08-04 11:07:37  chris
// Restructure towards ICG SVN REPO part 1 ::
// o added extras
// - log4cplus
// . added vc.2005 directory w/ solution and project (for dll): TODO: tested on win32
// - cxxtest
// o tests
// - moved xml -> lib
// - moved adapter -> lib
// - removed adapter, xml
// - moved tests/common/TestInit.h -> tests/
// - removed common
// o root
// - updated Makefile (see tests)
// o misc
// - removed
//
// Revision 1.3  2003/06/12 23:54:17  tcsmith
// Corrected the getChainedLogLevel() signature to match the ancestor class.
//
// Revision 1.2  2003/04/19 22:56:24  tcsmith
// Fixed UNICODE support.
//
// Revision 1.1  2003/04/18 22:25:30  tcsmith
// Initial version.
//

#include <log4cplus/spi/rootlogger.h>

using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace log4cplus::spi;



//////////////////////////////////////////////////////////////////////////////
// RootLogger Constructor
//////////////////////////////////////////////////////////////////////////////

RootLogger::RootLogger(Hierarchy& h, LogLevel ll)
: LoggerImpl(LOG4CPLUS_TEXT("root"), h)
{
    setLogLevel(ll);
}



//////////////////////////////////////////////////////////////////////////////
// Logger Methods
//////////////////////////////////////////////////////////////////////////////

LogLevel 
RootLogger::getChainedLogLevel() const
{
    return ll;
}


void 
RootLogger::setLogLevel(LogLevel ll)
{
    if(ll == NOT_SET_LOG_LEVEL) {
        getLogLog().error(LOG4CPLUS_TEXT("You have tried to set NOT_SET_LOG_LEVEL to root."));
    }
    else {
        LoggerImpl::setLogLevel(ll);
    }
}

