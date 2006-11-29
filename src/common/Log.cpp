
#include "Log.h"

#include <QRegExp>
#include <QMap>

#include <sstream>

#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

//--------------------------------------------------------------------------------

// 00041     const LogLevel OFF_LOG_LEVEL     = 60000;
// 00046     const LogLevel FATAL_LOG_LEVEL   = 50000;
// 00051     const LogLevel ERROR_LOG_LEVEL   = 40000;
// 00056     const LogLevel WARN_LOG_LEVEL    = 30000;
// 00062     const LogLevel INFO_LOG_LEVEL    = 20000;
// 00067     const LogLevel DEBUG_LOG_LEVEL   = 10000;
// 00072     const LogLevel TRACE_LOG_LEVEL   = 0;
// 00077     const LogLevel ALL_LOG_LEVEL     = TRACE_LOG_LEVEL;
// 00083     const LogLevel NOT_SET_LOG_LEVEL = -1;

//--------------------------------------------------------------------------------
#ifdef WIN32
// see http://www.tutorialsall.com/VC/strftime-crash/
static const std::string DEFAULT_CONVERSION_PATTERN_CLIENTLOG("%D{%H:%M:%S} (%-5p) [%c] : %m%n");
#else
static const std::string DEFAULT_CONVERSION_PATTERN_CLIENTLOG("%D{%H:%M:%S.%q} (%-5p) [%c] : %m%n");
#endif
static const std::string DEFAULT_CONVERSION_PATTERN("(%-5p) %c: %m%n");
static const std::string COUT_CONVERSION_PATTERN("%-5p: %m%n");

static ConsoleAppender* defaultConsoleAppender_;

//--------------------------------------------------------------------------------
PatternLayout* getPatternLayout(const std::string & pattern)
{
    Properties props;
    props.setProperty("ConversionPattern", pattern);
    return new PatternLayout(props);
}

//--------------------------------------------------------------------------------
PatternLayout* getDefaultPatternLayout()
{
    return getPatternLayout(DEFAULT_CONVERSION_PATTERN);
}


//================================================================================

//--------------------------------------------------------------------------------
bool Log::isInitialized_ = false;

//--------------------------------------------------------------------------------
bool Log::isInitialized()
{
    return isInitialized_;
}

//--------------------------------------------------------------------------------
void Log::init(LogLevel ll)
{
    if (isInitialized_)
        return;

    //// init default Console appender

    {
        Properties props;
        props.setProperty("logToStdErr", "true");
        props.setProperty("ImmediateFlush", "true");
        defaultConsoleAppender_ = new ConsoleAppender(props);
        defaultConsoleAppender_->setName("DefaultConsoleAppender");
        defaultConsoleAppender_->setLayout(std::auto_ptr<Layout>(getDefaultPatternLayout()));
    }

    //// init ROOT logger

    {
#if 0
        Properties props;
        props.setProperty("File", "Thekla-Debug.log");
        props.setProperty("ImmediateFlush", "true");
        props.setProperty("Append", "false");

//         // RollingFileAppender
//         props.setProperty("MaxFileSize", "200KB");
//         props.setProperty("MaxFileSize", "1MB");
//         props.setProperty("MaxBackupIndex", "1");

//         // DailyRollingFileAppender
//         props.setProperty("Schedule", "MONTHLY"); // WEEKLY, DAILY, etc

        FileAppender * appender = new FileAppender(props);
        appender->setName("RootLogFileAppender");
        appender->setLayout(std::auto_ptr<Layout>(getDefaultPatternLayout()));
#endif

		//// Add Appenders

        rootLog.addAppender(defaultConsoleAppender_);
//        rootLog.addAppender(appender);

        //// Set LogLevel

        rootLog.setLogLevel(INFO_LOG_LEVEL);
//        rootLog.setLogLevel(ALL_LOG_LEVEL);

        if (ll != NOT_SET_LOG_LEVEL) {
            rootLog.setLogLevel(ll);
        }
    }

    //// init CLIENT logger

    {

        //// Create FileAppender

        std::ostringstream fileName;
        fileName << "TheklaClient.log";

        FileAppender * appender = NULL;

        Properties props;
        props.setProperty("File", fileName.str());
        props.setProperty("ImmediateFlush", "true");
        props.setProperty("Append", "true");

        appender = new FileAppender(props);
        appender->setName("ClientFileAppender");
        appender->setLayout(std::auto_ptr<Layout>(getPatternLayout(DEFAULT_CONVERSION_PATTERN_CLIENTLOG)));

        //// Set Appenders

        clientLog.addAppender(appender);

        clientLog.setAdditivity(false);
        clientLog.setLogLevel(ALL_LOG_LEVEL);
    }

    //// init COUT logger

    {
        Properties props;
        props.setProperty("logToStdErr", "false");
        props.setProperty("ImmediateFlush", "true");
        ConsoleAppender * cAppender = new ConsoleAppender(props);
        cAppender->setName("StdOutConsoleAppender");
        cAppender->setLayout(std::auto_ptr<Layout>(getPatternLayout(COUT_CONVERSION_PATTERN)));

        coutLog.addAppender(cAppender);

        coutLog.setAdditivity(false);
        coutLog.setLogLevel(INFO_LOG_LEVEL);
    }

    isInitialized_ = true;
}

//--------------------------------------------------------------------------------
Log Log::getChildLog(const Log & parentLog,
                     const std::string & logName,
                     log4cplus::LogLevel ll )
{
    return getLog(parentLog.getName() + "." + logName, ll);
}

//--------------------------------------------------------------------------------
Log Log::getClassLog(const std::string & className, LogLevel ll)
{
    return getLog(className, ll);
}

//--------------------------------------------------------------------------------
Log Log::getMethodLog(const Log & classLog, const std::string & methodName, LogLevel ll)
{
    return getLog(classLog.getName() + "." + methodName, ll);
}

//--------------------------------------------------------------------------------
Log Log::getLog(const std::string & logName, LogLevel ll)
{
    // create Logger
    Log ret(getInstance(logName));

    // set LogLevel
    ret.setLogLevel(ll);

    return ret;
}

//--------------------------------------------------------------------------------
Log Log::getClientLog(const QString & clientAppId, LogLevel ll)
{
    THEKLA_DEBUG(rootLog, "Log::getClientLog(): ** START: clientAppId = " << clientAppId.toStdString());

    // convert application ID
    QString clientAppIdCopy(clientAppId);
    std::string clientAppIdEdit = clientAppIdCopy.remove(QRegExp("\\s")).toStdString();

    //// Create Logger

    std::string loggerName = clientLog.getName() + "." + clientAppIdEdit;
    bool existsLog = Log::exists(loggerName);
    Log ret = Log::getInstance(loggerName);

    if (existsLog) {
        return ret;
    }

    //// Set LogLevel

    ret.setAdditivity(true);
    ret.setLogLevel(ll);

    THEKLA_DEBUG(rootLog, "getClientLog() ** END");
    return ret;
}


// //--------------------------------------------------------------------------------
// void Log::initClientLog(const std::string & clientAppId, log4cplus::LogLevel ll)
// {
//     THEKLA_DEBUG(rootLog, "Log::initClientLog(): ** START: clientAppId = " << clientAppId);

// //     //// Create FileAppender

// //     std::ostringstream fileName;
// //     fileName << "TheklaClient-" << clientAppId << ".log";

// //     FileAppender * appender = NULL;

// //     Properties props;
// //     props.setProperty("File", fileName.str());
// //     props.setProperty("ImmediateFlush", "true");
// //     props.setProperty("Append", "false");

// //     appender = new FileAppender(props);
// //     appender->setName(clientAppId);
// //     appender->setLayout(std::auto_ptr<Layout>(getPatternLayout(DEFAULT_CONVERSION_PATTERN_CLIENTLOG)));

// //     //// Set Appenders

// //     addAppender(appender);

//     //// Set LogLevel

//     setLogLevel(ll);

//     THEKLA_DEBUG(rootLog, "Log::initClientLog(): ** END.");
// }

