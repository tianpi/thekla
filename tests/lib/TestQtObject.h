// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/QtObject.h>
#include <common/Observable.h>

class QtObjectTest : public CxxTest::TestSuite
{
private:
    class TestQtObject : public QtObject
    {
    public:
        TestQtObject()
            : QtObject()
            {};
        TestQtObject(const TestQtObject & copy)
            : QtObject(copy)
            {};
        ~TestQtObject()
            {};
    public:
        virtual XmlElement * clone() const { return new TestQtObject(*this); };
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
            TestQtObject * qtObj = new TestQtObject;
            TS_ASSERT_EQUALS(qtObj->getClassName(), "");
            TS_ASSERT_EQUALS(qtObj->getObjectName(), "");
            TS_ASSERT(qtObj->getQObject() == NULL);

            qtObj->setObjectName("object");
            TS_ASSERT_EQUALS(qtObj->getObjectName(), "object");

            delete qtObj;
        }
    void test_canInit()
        {
            TestQtObject * qtObj = new TestQtObject;

            TS_ASSERT(qtObj->canInit(Observable::QOBJECT));

            delete qtObj;
        }
    void test_initObservable()
        {
            TestQtObject * qtObj = new TestQtObject;

            TS_ASSERT(qtObj->canInit(Observable::QOBJECT));

            QObject * qObj = new QObject;
            qObj->setObjectName("objectName");
            Observable * obs = new Observable(qObj);
            qtObj->init(obs);

            TS_ASSERT_EQUALS(qtObj->getClassName(), "QObject");
            TS_ASSERT_EQUALS(qtObj->getObjectName(), "objectName");

            TS_ASSERT_EQUALS(qtObj->getChildCount(), 0);

            // NOTE: see TestXmlDocument (!): need an XmlDocument to access the
            // AdapterFactory -> create an QtAdapter without bind() is not possible
//            TS_ASSERT(qtObj->canBind());

            delete qtObj;
        }

    void test_update()
        {
            TestQtObject * qtObj = new TestQtObject;
            QObject qObj;
            qtObj->init(new Observable(&qObj));

            TS_ASSERT_EQUALS(qtObj->getObjectName(), "");

            // changing object name
            qObj.setObjectName("XXX");
            qtObj->update();
            TS_ASSERT_EQUALS(qtObj->getObjectName(), "XXX");

        }

    void test_merge()
        {
            TestQtObject * qtObj = new TestQtObject;
            QObject qObj;
            qtObj->init(new Observable(&qObj));

            // init Element from XML
            TestQtObject * qtObjFromXml = new TestQtObject;
            ((XmlElement*)qtObjFromXml)->init(qtObj->getDom().documentElement().cloneNode(false).toElement());

            // set XML attribute
            qtObjFromXml->getXmlData().setConfigFlag(XmlElementData::BIND, true);

            TS_ASSERT_EQUALS(qtObjFromXml->getObjectName(), "");
            qObj.setObjectName("XXX");
            qtObjFromXml->merge(new Observable(&qObj));

            // this XML element must have been modified
            TS_ASSERT_EQUALS(qtObjFromXml->getObjectName(), "XXX");
            // the above XML attribute is not touched by merge -> MUST still be there
            TS_ASSERT_EQUALS(qtObjFromXml->getXmlData().getConfigFlag(XmlElementData::BIND), true);


            //// Element: from XML (copy)

            TestQtObject qtObj2;
            ((XmlElement*)&qtObj2)->init(qtObj->getDom().documentElement().cloneNode(true).toElement());

            // change object name
            qtObj2.setObjectName("YYY");

            // Observable: merge + sync first
            qtObj2.merge(new Observable(&qObj), true);
            TS_ASSERT_EQUALS(qObj.objectName(), "YYY");


            //// Merge twice (kill Observable first):
            ////
            //// NOTE: Killing of current Observable done within XmlElement::merge() but
            //// cannot be tested there (does not support any Observable type -> Exception
            //// was thrown). That's why this feature must be tested with a XmlElement
            //// specialization.

            TestQtObject qtObj3;
            ((XmlElement*)&qtObj3)->init(qtObj->getDom().documentElement().cloneNode(true).toElement());

            // set XML attribute
            qtObj3.setObjectName("YYY");

            // (1) merge w/ Observable (sync first)
            qObj.setObjectName("XXX");
            qtObj3.merge(new Observable(&qObj), true);
            TS_ASSERT_EQUALS(qObj.objectName(), "YYY");

            // (2) merge w/ (the same) Observable (NO sync first)
            qObj.setObjectName("XXX");
            qtObj3.merge(new Observable(&qObj));
            TS_ASSERT_EQUALS(qObj.objectName(), "XXX");



        }

    void test_syncObservable()
        {
            TestQtObject * qtObj = new TestQtObject;
            QObject qObj;

            qObj.setObjectName("Hello");
            qtObj->init(new Observable(&qObj));
            TS_ASSERT_EQUALS(qObj.objectName(), "Hello");

            qtObj->setObjectName("World");
            qtObj->syncObservable();
            TS_ASSERT_EQUALS(qObj.objectName(), "World");

            delete qtObj;
        }

    void test_bind()
        {
            // See test in TestXmlDocument suite
        }

    void test_connect_sync()
        {
            QSlider qSlider1, qSlider2;
            qSlider1.setObjectName("XXX");
            qSlider2.setObjectName("YYY");

            QPushButton qButton;

            // Init QtObject with QSlider
            QtObject qtObj;
            qtObj.init(new Observable(&qSlider1));

            //// Test connect

            // Not allowed to connect to differnt QObject type: QSlider != QPushButton
            TS_ASSERT_THROWS(qtObj.connect(new Observer(&qButton)), Exception);

            // Allowed to connect equal types: QSlider == QSlider
            qtObj.connect(new Observer(&qSlider2));

            // NOTE: see successive test in TestXmlDocument suite


            //// Test sync

            qtObj.sync();

            // object name must have been synchronized
            TS_ASSERT_EQUALS(qSlider2.objectName(), "XXX");
        }


};
