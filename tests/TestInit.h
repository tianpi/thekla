// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/Log.h>
#include <common/TypeConverter.h>
#include <common/DatabaseConn.h>
#include <common/Exception.h>
#include <common/DatabaseEventDispatcher.h>

#include <QApplication>
#include <Inventor/SoDB.h>

namespace THEKLA {};
using namespace THEKLA;

//--------------------------------------------------------------------------------
Log testsLog(Log::getInstance("Tests"));

//--------------------------------------------------------------------------------
class InitTest : public CxxTest::TestSuite
{

public:

    InitTest()
        {
            // Init Qt application singleton object
            if (QApplication::instance() == NULL) {
                int argc = 1;
                char * argv[] = { "" };
                new QApplication(argc, argv);
            }

            // init Logging
            Log::init();
            Exception::setLog(testsLog);

            // init Coin meta database
            if (!SoDB::isInitialized())
                SoDB::init();

            // Init type converting
            TypeConverter::init();

            // Init event dispatcher
            DatabaseEventDispatcher::init();
        }

public:
    void setUp()
        {

        }
    void tearDown()
        {

        }
    void test_init()
        {
            TS_ASSERT(Log::isInitialized());
            TS_ASSERT(SoDB::isInitialized());
            TS_ASSERT(QApplication::instance() != NULL);
        }
};

