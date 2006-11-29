
#ifndef THEKLA_CONFIGWIN32_H
#define THEKLA_CONFIGWIN32_H

#include <Qt/qglobal.h>

//
// Windows DLL export definitions
//

#if defined(WIN32) || defined(_WIN32_WCE)

#pragma warning( disable : 4003 )
// C:\dia\ACE_wrappers\ace/OS_NS_Thread.inl(3702) : warning C4805: '==' : unsafe mix of type 'BOOL' and type 'bool' in operation
#pragma warning( disable : 4805 )

// see http://www.theaceorb.com/faq/index.html#154
// add define to project: -D_WIN32_WINNT=0x0400

// see http://www.devx.com/tips/Tip/14540
#define NOMINMAX

// see http://www.softgames.de/forum/frage87836-0-asc-0.html
#include <winsock2.h>

#include <windows.h>

#ifdef THEKLA_LIB
# define THEKLA_EXPORT Q_DECL_EXPORT
#elif THEKLA_STATIC
# define THEKLA_EXPORT
#else
# define THEKLA_EXPORT Q_DECL_IMPORT
#endif

//
// non-WIN32
//

#else

#define THEKLA_EXPORT

#endif // WIN32

#endif // THEKLA_CONFIGWIN32_H
