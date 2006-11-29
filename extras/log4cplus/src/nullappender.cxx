// Module:  Log4CPLUS
// File:    nullappender.cxx
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
// $Log: nullappender.cxx,v $
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
// Revision 1.2  2003/07/30 05:52:29  tcsmith
// Modified to remove "unused parameter" warning.
//
// Revision 1.1  2003/06/23 21:02:53  tcsmith
// Initial version.
//

#include <log4cplus/nullappender.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;



///////////////////////////////////////////////////////////////////////////////
// log4cplus::NullAppender ctors and dtor
///////////////////////////////////////////////////////////////////////////////

log4cplus::NullAppender::NullAppender()
{
}


log4cplus::NullAppender::NullAppender(const log4cplus::helpers::Properties& properties)
: Appender(properties)
{
}



log4cplus::NullAppender::~NullAppender()
{
    destructorImpl();
}



///////////////////////////////////////////////////////////////////////////////
// log4cplus::NullAppender public methods
///////////////////////////////////////////////////////////////////////////////

void
log4cplus::NullAppender::close()
{
}



///////////////////////////////////////////////////////////////////////////////
// log4cplus::NullAppender protected methods
///////////////////////////////////////////////////////////////////////////////

// This method does not need to be locked since it is called by
// doAppend() which performs the locking
void
log4cplus::NullAppender::append(const spi::InternalLoggingEvent&)
{
}


