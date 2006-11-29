// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/QtObjectUtils.h>

#include <QSlider>

class QtObjectUtilsTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {
        }
    void tearDown()
        {
        }
    void test_signatureStuff()
        {
            QString in;
            QString out;

            std::string out2, ret;

            in = "valueChanged( int & )";
            out = "valueChanged(int&)";
            TS_ASSERT_EQUALS(QtObjectUtils::normalizeSignature(in), out);

            in = SIGNAL();
            in += "valueChanged( int & )";
            out = SIGNAL();
            out += "valueChanged(int&)";
            TS_ASSERT_EQUALS(QtObjectUtils::normalizeSignature(in), out);

            in = SLOT();
            in += "valueChanged( int & )";
            out = "valueChanged( int & )";
            TS_ASSERT_EQUALS(QtObjectUtils::removeSlotPrefix(in), out);

            in = SIGNAL();
            in += "valueChanged( int & )";
            out = "valueChanged( int & )";
            TS_ASSERT_EQUALS(QtObjectUtils::removeSignalPrefix(in), out);

            in = "valueChanged(int)";
            out2 = std::string(SLOT(valueChanged(int)));
            ret = std::string(QtObjectUtils::addSlotPrefix(in));
            TS_ASSERT_EQUALS(ret, out2);

            in = "valueChanged(int)";
            out2 = std::string(SIGNAL(valueChanged(int)));
            ret = std::string(QtObjectUtils::addSignalPrefix(in));
            TS_ASSERT_EQUALS(ret, out2);

        }
    void test_isSerializable()
        {

            QSlider obj;
            const QMetaObject * meta = obj.metaObject();
            QMetaMethod method;
            QMetaProperty property;

            // Serializable

            method = meta->method(meta->indexOfSignal("destroyed(QObject*)"));
            TS_ASSERT_EQUALS(QtObjectUtils::isSerializable(method), false);

            method = meta->method(meta->indexOfSignal("destroyed()"));
            TS_ASSERT_EQUALS(QtObjectUtils::isSerializable(method), true);

            method = meta->method(meta->indexOfSignal("valueChanged(int)"));
            TS_ASSERT_EQUALS(QtObjectUtils::isSerializable(method), true);

            method = meta->method(meta->indexOfSlot("setValue(int)"));
            TS_ASSERT_EQUALS(QtObjectUtils::isSerializable(method), false);

            // Connectable

            method = meta->method(meta->indexOfSlot("setValue(int)"));
            TS_ASSERT_EQUALS(QtObjectUtils::isConnectable(method), true);

            method = meta->method(meta->indexOfSignal("valueChanged(int)"));
            TS_ASSERT_EQUALS(QtObjectUtils::isConnectable(method), false);

        }

    void test_toFromStream()
        {
            QVariant i((int)666);
            QVariant f((float)0.999);
            QVariant str((QString)"Hello");

            QList<QVariant> list, listRet;
            list.push_back(i);
            list.push_back(f);
            list.push_back(str);


            QByteArray a;
            QDataStream s(&a, QIODevice::WriteOnly);
            s << i;
            s << f;
            s << str;
            TS_ASSERT_EQUALS(QtObjectUtils::toStream(list), a);

            QByteArray a2;
            QDataStream s2(&a2, QIODevice::WriteOnly);
            s2 << str;
            TS_ASSERT_EQUALS(QtObjectUtils::toStream(str), a2);

            listRet = QtObjectUtils::fromStream(a);
            TS_ASSERT_EQUALS(list, listRet);
        }
    void test_normalizeParamType()
        {
            QString in;
            QString out, ret;

            in = "QString&";
            out = "QString";
            ret = QtObjectUtils::normalizeParamType(in);
            TS_ASSERT_EQUALS(ret, out);

            in = "QString*";
            out = "QString";
            ret = QtObjectUtils::normalizeParamType(in);
            TS_ASSERT_EQUALS(ret, out);

        }

    void test_paramList()
        {

            QString in;
            QStringList out, ret;

            in = "valueChanged(int,float,QString&)";
            out << "int";
            out << "float";
            out << "QString&";
            ret = QtObjectUtils::getParamTypes(in);
            TS_ASSERT_EQUALS(ret, out);

            for (int i = 0; i < ret.size(); i++) {
                QString paramType = QtObjectUtils::normalizeParamType(ret[i]);
                TS_ASSERT_DIFFERS(QVariant::nameToType(paramType.toAscii()), QVariant::Invalid);
            }

            in = "click()";
            out.clear();
            ret = QtObjectUtils::getParamTypes(in);
            TS_ASSERT_EQUALS(ret.size(), 0);
            TS_ASSERT_EQUALS(ret, out);

        }

    void test_uniqueObjectName()
        {

            QString name1, name2;
            name1 = QtObjectUtils::getUniqueIdentifier();
            name2 = QtObjectUtils::getUniqueIdentifier();
            TS_ASSERT(!name1.isEmpty());
            TS_ASSERT(!name2.isEmpty());
            TS_ASSERT_DIFFERS(name1.toStdString(), name2.toStdString());
        }


};
