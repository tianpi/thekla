// Module:  Log4CPLUS
// File:    pointer.cxx
// Created: 6/2001
// Author:  Tad E. Smith
//
//
// Copyright (C) Tad E. Smith  All rights reserved.
//
// This software is published under the terms of the Apache Software
// License version 1.1, a copy of which has been included with this
// distribution in the LICENSE.APL file.
//
// $Log: pointer.cxx,v $
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
// Revision 1.7  2003/06/29 16:44:01  tcsmith
// Now now longer used the LogLog class.
//
// Revision 1.6  2003/06/13 17:49:30  tcsmith
// Changed to use the old style C headers.
//
// Revision 1.5  2003/04/19 23:04:32  tcsmith
// Fixed UNICODE support.
//
// Revision 1.4  2003/04/18 21:13:25  tcsmith
// Converted from std::string to log4cplus::tstring.
//
// Revision 1.3  2003/04/03 01:20:23  tcsmith
// Standardized the formatting.
//

#include <log4cplus/streams.h>
#include <log4cplus/helpers/pointer.h>
#include <log4cplus/helpers/threads.h>
#include <assert.h>
#include <stdio.h>

using namespace std;
using namespace log4cplus::helpers;


///////////////////////////////////////////////////////////////////////////////
// public methods
///////////////////////////////////////////////////////////////////////////////

void
log4cplus::helpers::throwNullPointerException(const char* file, int line)
{
    tostringstream buf;
    buf << LOG4CPLUS_TEXT("NullPointer: file=")
        << file
        << LOG4CPLUS_TEXT(" line=")
        << line
        << endl;

    throw NullPointerException(LOG4CPLUS_TSTRING_TO_STRING(buf.str()));
}



///////////////////////////////////////////////////////////////////////////////
// log4cplus::helpers::SharedObject dtor
///////////////////////////////////////////////////////////////////////////////

SharedObject::~SharedObject()
{
    LOG4CPLUS_BEGIN_SYNCHRONIZE_ON_MUTEX( access_mutex )
        if(!destroyed) {
            assert(destroyed);
        }
    LOG4CPLUS_END_SYNCHRONIZE_ON_MUTEX
    LOG4CPLUS_MUTEX_FREE( access_mutex );
}



///////////////////////////////////////////////////////////////////////////////
// log4cplus::helpers::SharedObject public methods
///////////////////////////////////////////////////////////////////////////////

void
SharedObject::addReference()
{
    if(destroyed) {
        assert(!destroyed);
    }
    LOG4CPLUS_BEGIN_SYNCHRONIZE_ON_MUTEX( access_mutex )
        if(destroyed) {
            assert(!destroyed);
        }
        ++count;
    LOG4CPLUS_END_SYNCHRONIZE_ON_MUTEX
}


void
SharedObject::removeReference()
{
    if(destroyed) {
        assert(!destroyed);
    }
    LOG4CPLUS_BEGIN_SYNCHRONIZE_ON_MUTEX( access_mutex )
        if(destroyed) {
            assert(!destroyed);
        }
        if(--count == 0) destroyed = true;
    LOG4CPLUS_END_SYNCHRONIZE_ON_MUTEX
    if (destroyed) delete this;
}


