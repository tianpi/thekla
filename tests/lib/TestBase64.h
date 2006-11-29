// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/Base64.h>

class Base64Test : public CxxTest::TestSuite
{
public:
    void setUp()
        {
        }
    void tearDown()
        {
        }
    void testEncodeDecode()
        {
            QByteArray s("Hello World");

            QByteArray enc = Base64::encode(s);
            QByteArray dec = Base64::decode(enc);

            TS_ASSERT_EQUALS(s, dec);
        }
};
