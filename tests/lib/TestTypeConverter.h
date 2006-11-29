// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/TypeConverter.h>

#include <Inventor/fields/SoFields.h>

class TypeConverterTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {
            TypeConverter::init();
        }
    void tearDown()
        {
        }
    void test_findOivType()
        {
            QVariant in;
            SoType out, ret;

            in = QVariant::String;
            out = SoMFString::getClassTypeId();
            ret = TypeConverter::findType(in.type());
            TS_ASSERT_EQUALS(ret, out);

            in = QVariant::UInt;
            out = SoSFUInt32::getClassTypeId();
            ret = TypeConverter::findType(in.type());
            TS_ASSERT_EQUALS(ret, out);

            in = QVariant::Invalid;
            out = SoSFTrigger::getClassTypeId();
            ret = TypeConverter::findType(in.type());
            TS_ASSERT_EQUALS(ret, out);


        };

    void test_findQtType()
        {
            SoType in;
            QVariant out, ret;

            in = SoMFString::getClassTypeId();
            out = QVariant::String;
            ret = TypeConverter::findType(in);
            TS_ASSERT_EQUALS(ret, out);

            in = SoSFUInt32::getClassTypeId();
            out = QVariant::UInt;
            ret = TypeConverter::findType(in);
            TS_ASSERT_EQUALS(ret, out);

            in = SoSFTrigger::getClassTypeId();
            out = QVariant::Invalid;
            ret = TypeConverter::findType(in);
            TS_ASSERT_EQUALS(ret, out);

        };

    void test_convertQtType()
        {
            QVariant in;

            {
                SoSFBool out, ret;
                bool b = false;
                in = b; out = b;
                TypeConverter::convert(in, ret);
                TS_ASSERT_EQUALS(ret, out);
            }
            {
                SoSFInt32 out, ret;
                int i = 666;
                in = i; out = i;
                TypeConverter::convert(in, ret);
                TS_ASSERT_EQUALS(ret, out);
            }
            {
                SoMFString out, ret;
                const char * str = "Blablaquatischi";
//                in.setValue(QString(str)); out.setValue(str);
                in = str; out = str;
                TS_ASSERT_EQUALS(in.type(), QVariant::String);
                TypeConverter::convert(in, ret);
                TS_ASSERT_EQUALS(ret.getNum(), out.getNum());
                for (int i = 0; i < out.getNum(); i++) {
                    TS_ASSERT_EQUALS(std::string(ret[i].getString()), std::string(out[i].getString()));
                }
            }
            {
                SoSFTrigger out, ret;
                in.convert(QVariant::Invalid);
                TS_ASSERT_EQUALS(in.type(), QVariant::Invalid);
                TypeConverter::convert(in, ret);

                // cannot test ..
//                TS_ASSERT_EQUALS(ret, out);
            }

        }
    void test_convertOivType()
        {
            QVariant out, ret;

            {
                SoSFBool in;
                bool b = false;
                in = b; out = b;
                TypeConverter::convert(in, ret);
                TS_ASSERT_EQUALS(ret, out);
            }
            {
                SoSFInt32 in;
                int i = 666;
                in = i; out = i;
                TypeConverter::convert(in, ret);
                TS_ASSERT_EQUALS(ret, out);
            }
            {
                SoMFString in;
                char * str0 = "Blablaquatischi";
                char * str1 = "Second line..";
                in.set1Value(0, str0);
                in.set1Value(1, str1);
                QString str;
                QTextStream stream(&str, QIODevice::WriteOnly);
                stream << str0 << endl << str1;
                out = str;
                TypeConverter::convert(in, ret);
                TS_ASSERT_EQUALS(ret, out);
            }
        }

    void test_getInstance()
        {
            SoType in;
            SoField * ret;

            in = SoMFString::getClassTypeId();
            ret = TypeConverter::getInstance(in);
            TS_ASSERT(ret != NULL);
            if (ret == NULL) return;
            SoMFString * out = dynamic_cast<SoMFString*>(ret);
            TS_ASSERT(out != NULL);
            if (out == NULL) return;
            delete out;

            in = SoSFUInt32::getClassTypeId();
            ret = TypeConverter::getInstance(in);
            TS_ASSERT(ret != NULL);
            if (ret == NULL) return;
            SoSFUInt32 * out2 = dynamic_cast<SoSFUInt32*>(ret);
            TS_ASSERT(out2 != NULL);
            if (out2 == NULL) return;
            out2->setValue(999);
            TS_ASSERT_EQUALS(out2->getValue(), 999);
            delete out2;

            in = SoSFTrigger::getClassTypeId();
            ret = TypeConverter::getInstance(in);
            TS_ASSERT(ret != NULL);
            if (ret == NULL) return;
            SoSFTrigger * out3 = dynamic_cast<SoSFTrigger*>(ret);
            TS_ASSERT(out3 != NULL);
            if (out3 == NULL) return;
            delete out3;


        }

};
