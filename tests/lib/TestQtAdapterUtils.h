// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/QtAdapterUtils.h>

class QtAdapterUtilsTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {
        }
    void tearDown()
        {
        }
    void test_getNameSignature()
        {

            QString in, out;

            in = "signature";
            out = in + "Slot";
            TS_ASSERT_EQUALS(QtAdapterUtils::getSlotName(in), out);

            in = "signature";
            out = in + "Signal";
            TS_ASSERT_EQUALS(QtAdapterUtils::getSignalName(in), out);

            in = "signature(int)";
            out = "signatureSignal(int)";
            TS_ASSERT_EQUALS(QtAdapterUtils::getSignalSignature(in), out);

            in = "signature(int)";
            out = "signatureSlot(int)";
            TS_ASSERT_EQUALS(QtAdapterUtils::getSlotSignature(in), out);


        }
};
