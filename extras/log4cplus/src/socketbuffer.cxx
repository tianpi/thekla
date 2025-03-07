// Module:  Log4CPLUS
// File:    socketbuffer.cxx
// Created: 5/2003
// Author:  Tad E. Smith
//
//
// Copyright (C) Tad E. Smith  All rights reserved.
//
// This software is published under the terms of the Apache Software
// License version 1.1, a copy of which has been included with this
// distribution in the LICENSE.APL file.
//
// $Log: socketbuffer.cxx,v $
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
// Revision 1.5  2003/11/21 21:23:29  tcsmith
// Fixed memory alignment errors on Solaris.
//
// Revision 1.4  2003/09/28 04:26:02  tcsmith
// Added include for <winsock.h> on WIN32.
//
// Revision 1.3  2003/08/08 05:36:51  tcsmith
// Changed the #if checks to look for _WIN32 and not WIN32.
//
// Revision 1.2  2003/05/21 22:11:00  tcsmith
// Added appendSize_t() method.
//
// Revision 1.1  2003/05/04 07:25:16  tcsmith
// Initial version.
//

#include <log4cplus/helpers/socketbuffer.h>
#include <log4cplus/helpers/loglog.h>

#if !defined(_WIN32)
#  include <netdb.h>
#else
#include <winsock.h>
#endif

using namespace log4cplus;
using namespace log4cplus::helpers;


//////////////////////////////////////////////////////////////////////////////
// SocketBuffer ctors and dtor
//////////////////////////////////////////////////////////////////////////////

log4cplus::helpers::SocketBuffer::SocketBuffer(size_t maxsize)
: maxsize(maxsize),
  size(0),
  pos(0),
  buffer(new char[maxsize])
{
}



log4cplus::helpers::SocketBuffer::SocketBuffer(const SocketBuffer& rhs)
{
    copy(rhs);
}



log4cplus::helpers::SocketBuffer::~SocketBuffer()
{
    delete buffer;
}



SocketBuffer& 
log4cplus::helpers::SocketBuffer::operator=(const SocketBuffer& rhs)
{
    if(&rhs != this) {
        delete buffer;
        copy(rhs);
    }

    return *this;
}



void
log4cplus::helpers::SocketBuffer::copy(const SocketBuffer& r)
{
    SocketBuffer& rhs = const_cast<SocketBuffer&>(r);
    maxsize = rhs.maxsize;
    size = rhs.size;
    pos = rhs.pos;
    buffer = rhs.buffer;

    rhs.maxsize = 0;
    rhs.size = 0;
    rhs.pos = 0;
    rhs.buffer = 0;
}




//////////////////////////////////////////////////////////////////////////////
// SocketBuffer methods
//////////////////////////////////////////////////////////////////////////////

unsigned char
log4cplus::helpers::SocketBuffer::readByte()
{
    if(pos >= maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readByte()- end of buffer reached"));
        return 0;
    }
    else if((pos + sizeof(unsigned char)) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readByte()- Attempt to read beyond end of buffer"));
        return 0;
    }

    unsigned char ret = *((unsigned char*)&buffer[pos]);
    pos += sizeof(unsigned char);

    return ret;
}



unsigned short
log4cplus::helpers::SocketBuffer::readShort()
{
    if(pos >= maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readShort()- end of buffer reached"));
        return 0;
    }
    else if((pos + sizeof(unsigned short)) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readShort()- Attempt to read beyond end of buffer"));
        return 0;
    }

    unsigned short ret;
    memcpy(&ret, buffer + pos, sizeof(ret));
    ret = ntohs(ret);
    pos += sizeof(unsigned short);

    return ret;
}



unsigned int
log4cplus::helpers::SocketBuffer::readInt()
{
    if(pos >= maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readInt()- end of buffer reached"));
        return 0;
    }
    else if((pos + sizeof(unsigned int)) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readInt()- Attempt to read beyond end of buffer"));
        return 0;
    }

    unsigned int ret;
    memcpy (&ret, buffer + pos, sizeof(ret));
    ret = ntohl(ret);
    pos += sizeof(unsigned int);
    
    return ret;
}


tstring
log4cplus::helpers::SocketBuffer::readString(unsigned char sizeOfChar)
{
    size_t strlen = readInt();
    size_t bufferLen = strlen * sizeOfChar;

    if(strlen == 0) {
        return tstring();
    }
    if(pos > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readString()- end of buffer reached"));
        return tstring();
    }

    if((pos + bufferLen) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readString()- Attempt to read beyond end of buffer"));
        bufferLen = (maxsize - 1) - pos;
        strlen = bufferLen / sizeOfChar;
    }

#ifndef UNICODE
    if(sizeOfChar == 1) {
        tstring ret(&buffer[pos], strlen);
        pos += strlen;
        return ret;
    }
    else if(sizeOfChar == 2) {
        tstring ret;
        for(tstring::size_type i=0; i<strlen; ++i) {
            unsigned short tmp = readShort();
            ret += static_cast<char>(tmp < 256 ? tmp : ' ');
        }
        return ret;
    }
    else {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readString()- Invalid sizeOfChar!!!!"));
    }

#else /* UNICODE */
    if(sizeOfChar == 1) {
        std::string ret(&buffer[pos], strlen);
        pos += strlen;
        return towstring(ret);
    }
    else if(sizeOfChar == 2) {
        tstring ret;
        for(tstring::size_type i=0; i<strlen; ++i) {
            ret += static_cast<tchar>(readShort());
        }
        return ret;
    }
    else {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::readString()- Invalid sizeOfChar!!!!"));
    }
#endif

    return tstring();
}



void
log4cplus::helpers::SocketBuffer::appendByte(unsigned char val)
{
    if((pos + sizeof(unsigned char)) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::appendByte()- Attempt to write beyond end of buffer"));
        return;
    }

    *((unsigned char*)&buffer[pos]) = val;
    pos += sizeof(unsigned char);
    size = pos;
}



void
log4cplus::helpers::SocketBuffer::appendShort(unsigned short val)
{
    if((pos + sizeof(unsigned short)) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::appendShort()- Attempt to write beyond end of buffer"));
        return;
    }

    *((unsigned short*)&buffer[pos]) = htons(val);
    pos += sizeof(unsigned short);
    size = pos;
}



void
log4cplus::helpers::SocketBuffer::appendInt(unsigned int val)
{
    if((pos + sizeof(unsigned int)) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::appendInt()- Attempt to write beyond end of buffer"));
        return;
    }

    int i = htonl(val);
    memcpy(buffer + pos, &i, sizeof (i));
    pos += sizeof(unsigned int);
    size = pos;
}



void
log4cplus::helpers::SocketBuffer::appendSize_t(size_t val)
{
    if((pos + sizeof(size_t)) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::appendInt(size_t)- Attempt to write beyond end of buffer"));
        return;
    }

    size_t st = htonl(val);
    memcpy(buffer + pos, &st, sizeof(st));
    pos += sizeof(size_t);
    size = pos;
}


void
log4cplus::helpers::SocketBuffer::appendString(const tstring& str)
{
#ifndef UNICODE
    size_t strlen = str.length();

    if((pos + sizeof(unsigned int) + strlen) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::appendString()- Attempt to write beyond end of buffer"));
        return;
    }

    appendSize_t(strlen);
    memcpy(&buffer[pos], str.data(), strlen);
    pos += strlen;
    size = pos;
#else
    size_t strlen = str.length();

    if((pos + sizeof(unsigned int) + (strlen * 2)) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::appendString()- Attempt to write beyond end of buffer"));
        return;
    }

    appendInt(strlen);
    for(tstring::size_type i=0; i<str.length(); ++i) {
        appendShort(static_cast<unsigned short>(str[i]));
    }
#endif
}



void
log4cplus::helpers::SocketBuffer::appendBuffer(const SocketBuffer& buf)
{
    if((pos + buf.getSize()) > maxsize) {
        getLogLog().error(LOG4CPLUS_TEXT("SocketBuffer::appendBuffer()- Attempt to write beyond end of buffer"));
        return;
    }

    memcpy(&buffer[pos], buf.buffer, buf.getSize());
    pos += buf.getSize();
    size = pos;
}



