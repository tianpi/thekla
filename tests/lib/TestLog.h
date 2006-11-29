// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/Log.h>

class LogTest : public CxxTest::TestSuite
{
public:
public:
    void setUp()
        {

        }
    void tearDown()
        {
        }
    void test_rootLog()
        {
            TS_ASSERT(Log::isInitialized());
            if (!Log::isInitialized()) return;

            TS_ASSERT_EQUALS(rootLog.getAdditivity(), true);
            TS_ASSERT_EQUALS(rootLog.getAllAppenders().size(), 1);

            Log clog(Log::getClassLog("Class", log4cplus::WARN_LOG_LEVEL));
            TS_ASSERT_EQUALS(clog.getAdditivity(), true);
            TS_ASSERT_EQUALS(clog.getLogLevel(), log4cplus::WARN_LOG_LEVEL);
            TS_ASSERT_EQUALS(clog.getName(), "Class");
            TS_ASSERT_EQUALS(clog.getAllAppenders().size(), 0);

            Log mlog(Log::getMethodLog(clog, "method()", log4cplus::DEBUG_LOG_LEVEL));
            TS_ASSERT_EQUALS(mlog.getAdditivity(), true);
            TS_ASSERT_EQUALS(mlog.getLogLevel(), log4cplus::DEBUG_LOG_LEVEL);
            TS_ASSERT_EQUALS(mlog.getName(), "Class.method()");
            TS_ASSERT_EQUALS(mlog.getAllAppenders().size(), 0);

            Log mlog2(Log::getMethodLog(clog, "method()"));
            TS_ASSERT_EQUALS(mlog2.getLogLevel(), log4cplus::NOT_SET_LOG_LEVEL);

            Log clog2(Log::getClassLog("Class"));
            TS_ASSERT_EQUALS(clog2.getLogLevel(), log4cplus::NOT_SET_LOG_LEVEL);

        }
    void test_clientLog()
        {
            TS_ASSERT(Log::isInitialized());
            if (!Log::isInitialized()) return;

            TS_ASSERT_EQUALS(clientLog.getAdditivity(), false);
            TS_ASSERT_EQUALS(clientLog.getAllAppenders().size(), 1);
            TS_ASSERT_EQUALS(clientLog.getLogLevel(), log4cplus::ALL_LOG_LEVEL);

            Log log(Log::getClientLog("AppId", log4cplus::WARN_LOG_LEVEL));
            TS_ASSERT_EQUALS(log.getLogLevel(), log4cplus::WARN_LOG_LEVEL);
            TS_ASSERT_EQUALS(log.getAllAppenders().size(), 0);
            std::string name(clientLog.getName());
            name += ".";
            name += "AppId";
            TS_ASSERT_EQUALS(log.getName(), name);

            Log log2(Log::getClientLog("AnotherAppId"));
            TS_ASSERT_EQUALS(log2.getLogLevel(), log4cplus::NOT_SET_LOG_LEVEL);

        }
};
