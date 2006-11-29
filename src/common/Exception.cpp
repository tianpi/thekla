
#include "Exception.h"

//--------------------------------------------------------------------------------
Log Exception::exceptLog = rootLog;

//--------------------------------------------------------------------------------
Exception::Exception(const std::string & msg)
    : std::runtime_error(msg)
{
}

//--------------------------------------------------------------------------------
Exception::~Exception()
    throw ()
{
}

//--------------------------------------------------------------------------------
Exception::Exception(const Exception & rhs)
    : runtime_error(rhs)
{
}
