// Module:  Log4CPLUS
// File:    loggingevent.cxx
// Created: 6/2003
// Author:  Tad E. Smith
//
//
// Copyright (C) Tad E. Smith  All rights reserved.
//
// This software is published under the terms of the Apache Software
// License version 1.1, a copy of which has been included with this
// distribution in the LICENSE.APL file.
//
// $Log: loggingevent.cxx,v $
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
// Revision 1.3  2003/09/28 04:01:40  tcsmith
// Made some fixes for the VisualAge 5 compiler on AIX.
//
// Revision 1.2  2003/06/29 02:04:23  tcsmith
// Added the clone() method.
//
// Revision 1.1  2003/06/27 15:44:34  tcsmith
// Initial version.
//

#include <log4cplus/spi/loggingevent.h>


using namespace log4cplus;
using namespace log4cplus::spi;


#define LOG4CPLUS_DEFAULT_TYPE 1


///////////////////////////////////////////////////////////////////////////////
// InternalLoggingEvent dtor
///////////////////////////////////////////////////////////////////////////////

InternalLoggingEvent::~InternalLoggingEvent()
{
}



///////////////////////////////////////////////////////////////////////////////
// InternalLoggingEvent static methods
///////////////////////////////////////////////////////////////////////////////

unsigned int
InternalLoggingEvent::getDefaultType()
{
    return LOG4CPLUS_DEFAULT_TYPE;
}



///////////////////////////////////////////////////////////////////////////////
// InternalLoggingEvent implementation
///////////////////////////////////////////////////////////////////////////////

const log4cplus::tstring& 
InternalLoggingEvent::getMessage() const
{
    return message;
}


unsigned int
InternalLoggingEvent::getType() const
{
    return LOG4CPLUS_DEFAULT_TYPE;
}



std::auto_ptr<InternalLoggingEvent>
InternalLoggingEvent::clone() const
{
    std::auto_ptr<InternalLoggingEvent> tmp(new InternalLoggingEvent(*this));
    return tmp;
}



log4cplus::spi::InternalLoggingEvent&
InternalLoggingEvent::operator=(const log4cplus::spi::InternalLoggingEvent& rhs)
{
    if(this == &rhs) return *this;

    message = rhs.message;
    loggerName = rhs.loggerName;
    ll = rhs.ll;
    ndc = rhs.getNDC();
    thread = rhs.getThread();
    timestamp = rhs.timestamp;
    file = rhs.file;
    line = rhs.line;
    threadCached = true;
    ndcCached = true;

    return *this;
}


