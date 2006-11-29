
#ifndef THEKLA_EXCEPTION_H
#define THEKLA_EXCEPTION_H

#include <common/config-win32.h>
#include "Log.h"
#include <stdexcept>

#include <string>
#include <QTextStream>

//--------------------------------------------------------------------------------
class THEKLA_EXPORT Exception : public std::runtime_error
{
private:
    static Log exceptLog;

public:
    static void setLog(Log log)
        { exceptLog = log; };
    static Log getLog()
        { return exceptLog; };

private:
    Exception operator=(const Exception & rhs);

public:
    Exception(const std::string & msg);
    Exception(const Exception & rhs);
    virtual ~Exception() throw ();

};

//--------------------------------------------------------------------------------
#define THROW_THEKLA_EXCEPTION(msg)                           \
    {                                                         \
        QString str;                                          \
        QTextStream stream(&str, QIODevice::WriteOnly);       \
        stream << __FILE__ << ":" << __LINE__ << ": " << msg; \
        std::string strStd(str.toStdString());                \
        THEKLA_ERROR(Exception::getLog(), strStd);            \
        throw Exception(strStd);                              \
    }



#endif // THEKLA_EXCEPTION_H
