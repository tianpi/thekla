// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/QtProperty.h>
#include <common/Observable.h>

class QtPropertyTest : public CxxTest::TestSuite
{
private:
    class TestQtProperty : public QtProperty
    {
    public:
        TestQtProperty()
            : QtProperty()
            {};
        TestQtProperty(const TestQtProperty & copy)
            : QtProperty(copy)
            {};
        ~TestQtProperty()
            {};
    public:
        virtual XmlElement * clone() const { return new TestQtProperty(*this); };
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
            TestQtProperty * qtProperty = new TestQtProperty;
            TS_ASSERT_EQUALS(qtProperty->getName(), "");
            TS_ASSERT_EQUALS(qtProperty->getType(), "");
            TS_ASSERT_EQUALS(qtProperty->getXmlData().getConfigFlag(XmlElementData::BIND), false);

            delete qtProperty;
        }
    void test_canInit()
        {
            XmlElement * elem = new TestQtProperty;
            QtProperty * qtProperty = dynamic_cast<QtProperty*>(elem);

            TS_ASSERT(elem->canInit(Observable::QOBJECTPROPERTY));

            delete qtProperty;
        }
    void test_initObservable()
        {
            TestQtProperty * qtProperty = new TestQtProperty;

            QObject * qObj = new QObject;
            const QMetaObject * qMeta = qObj->metaObject();
            QMetaProperty qProperty = qMeta->property(0);

            Observable * obs = new Observable(qObj, qProperty);
            qtProperty->init(obs);

            TS_ASSERT_EQUALS(qtProperty->getName(), qProperty.name());
            TS_ASSERT_EQUALS(qtProperty->getType(), qProperty.typeName());
            TS_ASSERT_EQUALS(qtProperty->getChildCount(), 0);

            // NOTE: see TestXmlDocument (!): need an XmlDocument to access the
            // AdapterFactory -> create an QtAdapter without bind() is not possible
//            TS_ASSERT(qtProperty->canBind());

            delete qtProperty;
        }
    void test_update()
        {
            QVariant qVar((int)9999);
            QVariant v1, v2;

            TestQtProperty qtProp;
            QSlider qObj;
            QMetaProperty qProp = QtObjectUtils::getMetaProperty(qObj, "maximum");

            qtProp.init(new Observable(&qObj, qProp));

            v1 = QtObjectUtils::fromStream(qtProp.getXmlData().getText(true))[0];
            v2 = qObj.property("maximum");
            TS_ASSERT_EQUALS(v1.toString().toStdString(), v2.toString().toStdString());

            qObj.setProperty("maximum", qVar);
            qtProp.update();

            TS_ASSERT_EQUALS(qtProp.getXmlData().getText(true), QtObjectUtils::toStream(qVar));

        }

    void test_merge()
        {
            QVariant qVar((int)99999), v1, v2;

            // init property from Observable
            TestQtProperty qtObj;
            QSlider qObj;
            QMetaProperty qProp = QtObjectUtils::getMetaProperty(qObj, "maximum");
            qtObj.init(new Observable(&qObj, qProp));
            TS_ASSERT_EQUALS(qtObj.getName(), "maximum");

            // init property element from xml (using the previous element)
            TestQtProperty qtObjFromXml;
            ((XmlElement *)(&qtObjFromXml))->init(qtObj.getDom().documentElement().cloneNode(true).toElement());
            TS_ASSERT_EQUALS(qtObjFromXml.getName(), "maximum");

            qtObjFromXml.getXmlData().setConfigFlag(XmlElementData::BIND, true);

            // empty (!)
            TS_ASSERT_EQUALS(qtObjFromXml.getXmlData().getText(true), qtObj.getXmlData().getText(true));

            // changing observable
            qObj.setProperty("maximum", qVar);
            TS_ASSERT_EQUALS(qObj.property("maximum"), qVar);

            // merge property with observable
            qtObjFromXml.merge(new Observable(&qObj, qProp));

            // NOT empty (!)
            TS_ASSERT_EQUALS(qtObjFromXml.getXmlData().getText(true), QtObjectUtils::toStream(qVar));
            TS_ASSERT_EQUALS(qtObjFromXml.getXmlData().getConfigFlag(XmlElementData::BIND), true);


            //// Element: from XML (copy)

            TestQtProperty qtObj2;
            ((XmlElement*)&qtObj2)->init(qtObj.getDom().documentElement().cloneNode(true).toElement());

            // change property value
            qtObj2.getXmlData().setText(QtObjectUtils::toStream(QVariant(555)), true);

            // Observable: merge + sync first
            qtObj2.merge(new Observable(&qObj, qProp), true);

            TS_ASSERT_EQUALS(qObj.property("maximum"), QVariant(555));


            //// Merge twice (kill Observable first):
            ////
            //// NOTE: Killing of current Observable done within XmlElement::merge() but
            //// cannot be tested there (does not support any Observable type -> Exception
            //// was thrown). That's why this feature must be tested with a XmlElement
            //// specialization.

            TestQtProperty qtObj3;
            qtObj3.init(new Observable(&qObj, qProp));

            // set XML attribute
            qtObj3.getXmlData().setText(QtObjectUtils::toStream(QVariant(555)), true);

            // (1) merge w/ Observable (sync first)
            qObj.setMaximum(111);
            qtObj3.merge(new Observable(&qObj, qProp), true);
            TS_ASSERT_EQUALS(qObj.maximum(), 555);

            // (2) merge w/ (the same) Observable (NO sync first)
            qObj.setMaximum(111);
            qtObj3.merge(new Observable(&qObj, qProp));
            TS_ASSERT_EQUALS(qObj.maximum(), 111);




        }

    void test_connect()
        {

            // QtProperty: init from Observable

            TestQtProperty qtProp;
            QSlider qObj;
            QMetaProperty qProp = QtObjectUtils::getMetaProperty(qObj, "maximum");
            qtProp.init(new Observable(&qObj, qProp));

            //// Connect Property -> SoField

            // Observer: init from SoSFInt32

            SoSFInt32 field;
            Observer obs(&field);
            TS_ASSERT_THROWS_NOTHING(qtProp.connect(new Observer(obs)));

            // connect Prop -> Field
            qtProp.connect(new Observer(obs));

            // Observable: set value
            qObj.setMaximum(815);

            // update Element, sync Observers
            qtProp.update();
            qtProp.sync();

            // Observer: value was forwarded !?!?
            TS_ASSERT_EQUALS(field.getValue(), 815);


            //// Connect Property -> Property

            // Observer: init from QMetaProperty
            QSlider qObj2;
            QMetaProperty qProp2 = QtObjectUtils::getMetaProperty(qObj, "maximum");
            Observer obs2(&qObj2, qProp2);

            // connect Prop -> Field
            qtProp.connect(new Observer(obs2));

            // Observable: set value
            qObj.setMaximum(3317);

            // update Element, sync Observers
            qtProp.update();
            qtProp.sync();

            // Observer: value was forwarded !?!?
            TS_ASSERT_EQUALS(qObj2.property("maximum").toInt(), 3317);
        }

    void test_syncObservable()
        {
            TestQtProperty qtProp;
            QSlider qObj;
            QMetaProperty qProp = QtObjectUtils::getMetaProperty(qObj, "maximum");
            qtProp.init(new Observable(&qObj, qProp));

            qObj.setMaximum(111);
            TS_ASSERT_EQUALS(qObj.maximum(), 111);

            qtProp.getXmlData().setText(QtObjectUtils::toStream(QVariant((int)222)), true);
            qtProp.syncObservable();
            TS_ASSERT_EQUALS(qObj.maximum(), 222);

        }

};
