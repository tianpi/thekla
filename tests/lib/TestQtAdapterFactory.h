// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <adapter/QtAdapterFactory.h>

class QtAdapterFactoryTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {
//            Log::init(OFF_LOG_LEVEL);
        }
    void tearDown()
        {
        }
    void test_getAdapter()
        {
            QtAdapterFactory factory;
            QtAdapter * adapter;

            adapter = factory.getAdapter(new QObject, NULL);
            TS_ASSERT(adapter != NULL);

            adapter = factory.getAdapter("XXX", NULL);
            TS_ASSERT(adapter == NULL);
        }
    void test_existsAdapter()
        {
            QtAdapterFactory factory;

            TS_ASSERT(factory.existsAdapter("QObject"));
            TS_ASSERT(!factory.existsAdapter("xXX"));

        }
};
