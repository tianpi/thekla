
#ifndef THEKLA_LOG_H
#define THEKLA_LOG_H

#include <common/config-win32.h>

#include <log4cplus/logger.h>
#include <QString>

//--------------------------------------------------------------------------------
#define THEKLA_TRACE(log, ostream) LOG4CPLUS_TRACE(log, ostream)
#define THEKLA_DEBUG(a, b) LOG4CPLUS_DEBUG(a, b)
#define THEKLA_INFO(a, b) LOG4CPLUS_INFO(a, b)
#define THEKLA_WARN(a, b) LOG4CPLUS_WARN(a, b)
#define THEKLA_ERROR(a, b) LOG4CPLUS_ERROR(a, b)
#define THEKLA_FATAL(a, b) LOG4CPLUS_FATAL(a, b)

//--------------------------------------------------------------------------------
class THEKLA_EXPORT Log : public log4cplus::Logger
{
private:

    static bool isInitialized_;

private:

    static Log getLog(const std::string & name, log4cplus::LogLevel ll);

public:

    static bool isInitialized();

    static void init(log4cplus::LogLevel ll = log4cplus::NOT_SET_LOG_LEVEL);

    static Log getClassLog(const std::string & className,
                           log4cplus::LogLevel ll = log4cplus::NOT_SET_LOG_LEVEL);

    static Log getMethodLog(const Log & classLog,
                            const std::string & methodName,
                            log4cplus::LogLevel ll = log4cplus::NOT_SET_LOG_LEVEL);

    static Log getClientLog(const QString & clientAppId,
                            log4cplus::LogLevel ll = log4cplus::NOT_SET_LOG_LEVEL);

    static Log getChildLog(const Log & parentLog,
                           const std::string & logName,
                           log4cplus::LogLevel ll = log4cplus::NOT_SET_LOG_LEVEL);

private:

    Log();

public:

    Log(const log4cplus::Logger & logger)
        : Logger(logger)
        {};
    virtual ~Log()
        {};

private:

//    void initClientLog(const std::string & clientAppId, log4cplus::LogLevel ll);
};


//--------------------------------------------------------------------------------
static Log rootLog(Log::getRoot());

//--------------------------------------------------------------------------------
static Log clientLog(Log::getInstance("Client"));

//--------------------------------------------------------------------------------
static Log coutLog(Log::getInstance("ConsoleStdOut"));

#endif // THEKLA_LOG_H
