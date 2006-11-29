// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/QtSignal.h>
#include <common/Observable.h>

#include <QSlider>

class QtSignalTest : public CxxTest::TestSuite
{
private:
    class TestQtSignal : public QtSignal
    {
    public:
        TestQtSignal()
            : QtSignal()
            {};
        TestQtSignal(const TestQtSignal & copy)
            : QtSignal(copy)
            {};
        ~TestQtSignal()
            {};
    public:
        virtual XmlElement * clone() const { return new TestQtSignal(*this); };
    };

public:
    void setUp()
        {
        }
    void tearDown()
        {
        }

    void test_Ctor()
        {
            TestQtSignal * qtSignal = new TestQtSignal;
            TS_ASSERT_EQUALS(qtSignal->getSignature(), "");

            TS_ASSERT_EQUALS(qtSignal->getXmlData().getConfigFlag(XmlElementData::BIND), false);
            TS_ASSERT_EQUALS(qtSignal->getXmlData().getConfigFlag(XmlElementData::USE_AS_TRIGGER), false);

            delete qtSignal;
        }
    void test_canInit()
        {
            XmlElement * elem = new TestQtSignal;
            QtSignal * qtSignal = dynamic_cast<QtSignal*>(elem);

            TS_ASSERT(elem->canInit(Observable::QOBJECTSIGNAL));
            TS_ASSERT(qtSignal->canInit(Observable::QOBJECTSIGNAL));

            delete qtSignal;
        }
    void test_initObservable()
        {
            TestQtSignal * qtSignal = new TestQtSignal;

            QSlider * qObj = new QSlider;
            const QMetaObject * qMeta = qObj->metaObject();
            QMetaMethod qSignal = qMeta->method(qMeta->indexOfSignal("valueChanged(int)"));

            Observable * obs = new Observable(qObj, qSignal);
            qtSignal->init(obs);

            TS_ASSERT_EQUALS(qtSignal->getSignature(), qSignal.signature());
            TS_ASSERT_EQUALS(qtSignal->getChildCount(), 0);

            // NOTE: see TestXmlDocument (!): need an XmlDocument to access the
            // AdapterFactory -> create an QtAdapter without bind() is not possible
//            TS_ASSERT(qtSignal->canBind());

            delete qtSignal;
        }

    void test_update()
        {
            QVariant qVar((int)9999);
            QVariant v1, v2;

            TestQtSignal qtSignal;
            QSlider qObj;
            QMetaMethod qSignal = QtObjectUtils::getMetaMethod(qObj, "valueChanged(int)");

            qtSignal.init(new Observable(&qObj, qSignal));
            ((XmlElement*)(&qtSignal))->update();

            // nothing to do here ..
        }

    void test_merge()
        {
            QVariant qVar((int)99999), v1, v2;

            // init property from Observable
            TestQtSignal qtObj;
            QSlider qObj;
            QMetaMethod qSignal = QtObjectUtils::getMetaMethod(qObj, "valueChanged(int)");
            qtObj.init(new Observable(&qObj, qSignal));
            TS_ASSERT_EQUALS(qtObj.getSignature(), "valueChanged(int)");

            // init property element from xml (using the previous element)
            TestQtSignal qtObjFromXml;
            ((XmlElement *)(&qtObjFromXml))->init(qtObj.getDom().documentElement().cloneNode(true).toElement());
            TS_ASSERT_EQUALS(qtObjFromXml.getSignature(), "valueChanged(int)");

            qtObjFromXml.getXmlData().setConfigFlag(XmlElementData::BIND, true);

            // merge property with observable
            qtObjFromXml.merge(new Observable(&qObj, qSignal));

            // nothing to do here ..

        }

    void test_connect()
        {

            // QtSignal: init from Observable
            TestQtSignal qtSignal;
            QSlider qObj;
            QMetaMethod qSignal = QtObjectUtils::getMetaMethod(qObj, "rangeChanged(int,int)");
            qtSignal.init(new Observable(&qObj, qSignal));
            TS_ASSERT_EQUALS(qtSignal.getSignature(), "rangeChanged(int,int)");


            //// Signal -> FieldList

            SoFieldList fieldList;
            SoSFInt32 i1, i2;
            fieldList.append(&i1);
            fieldList.append(&i2);
            Observer obs(&fieldList);

            // connect Prop -> Field
            qtSignal.connect(new Observer(obs));

            // cannot test more .. (missing Adapter) ..
            //
            // .. Workaround ->

            // create QVariant parameter values manually, set Element text
            QList<QVariant> values;
            values.push_back(QVariant(123));
            values.push_back(QVariant(456));
            qtSignal.getXmlData().setText(QtObjectUtils::toStream(values), true);

            // Element -> Observers
            qtSignal.sync();

            // check Observers values
            TS_ASSERT_EQUALS(i1.getValue(), 123);
            TS_ASSERT_EQUALS(i2.getValue(), 456);


            //// Signal with no parameters

            TestQtSignal qtSignal2;
            QMetaMethod qSignal2 = QtObjectUtils::getMetaMethod(qObj, "sliderPressed()");
            qtSignal2.init(new Observable(&qObj, qSignal2));
            TS_ASSERT_EQUALS(qtSignal2.getSignature(), "sliderPressed()");

            SoFieldList fieldList2;
            SoSFTrigger t1;
            fieldList2.append(&t1);
            Observer obs2(&fieldList2);

        }
};
