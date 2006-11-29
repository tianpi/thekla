// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/XmlDocument.h>

#include <xml/QtObject.h>
#include <xml/QtSignal.h>
#include <xml/QtProperty.h>
#include <common/QtObjectUtils.h>
#include <common/Exception.h>

#include <QSlider>

class XmlDocumentTest : public CxxTest::TestSuite
{

private:
    class TestXmlDocument : public XmlDocument
    {
    private:
        friend class XmlDocumentTest;
    public:
        TestXmlDocument()
            : XmlDocument()
            {};
    private:
        virtual void initFactory() {};
        virtual void initDom() {};
    };

    class TestXmlElement : public XmlElement
    {
    private:
        friend class XmlElementTest;
    public:
        TestXmlElement(bool containsText = false)
            : XmlElement(containsText)
            {};
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
            TestXmlDocument doc;

            TS_ASSERT(doc.getRootElement() == NULL);
            TS_ASSERT(doc.adapterFactory_ != NULL);
        };

    void test_createSubDocumentWithRootElem()
        {
            TestXmlDocument doc;

            TS_ASSERT_THROWS(doc.createSubDocument((XmlElement*)NULL), Exception);
            TestXmlElement * e = new TestXmlElement;
            TS_ASSERT_THROWS(doc.createSubDocument(e), Exception);

            e->changeOwnerDocument(&doc);
            doc.setRootElement(e);
            XmlSubDocument subDoc = doc.createSubDocument(e);
            TS_ASSERT(subDoc.getRootElement() != NULL);
        }

    void test_createSubDocumentFromDom()
        {
            TestXmlDocument doc;

            doc.registerElement("QtObject", new QtObject, NULL);
            doc.registerElement("QtSignal", new QtSignal, NULL);
            doc.registerElement("QtProperty", new QtProperty, NULL);

            QDomDocument domDoc("Hello");
            QDomElement domObj = domDoc.createElement("QtObject");
            domDoc.appendChild(domObj);
            for (int i = 0; i < 2; i++) {
                QDomElement domSig = domDoc.createElement("QtSignal");
                domObj.appendChild(domSig);
                QDomText domSigText = domDoc.createTextNode("signalText");
                domSig.appendChild(domSigText);
            }
            for (int i = 0; i < 3; i++) {
                QDomElement domProp = domDoc.createElement("QtProperty");
                domObj.appendChild(domProp);
                QDomText domPropText = domDoc.createTextNode("propertyText");
                domProp.appendChild(domPropText);
            }

            XmlSubDocument subDoc = doc.createSubDocument(domDoc);

            TS_ASSERT(subDoc.getRootElement() != NULL);
            if (subDoc.getRootElement() != NULL) {

                XmlElement * obj = subDoc.getRootElement();
                TS_ASSERT_EQUALS(obj->getXmlData().getTagName(), "QtObject");

                QList<XmlElement *> sigs = subDoc.findElements("QtSignal");
                TS_ASSERT_EQUALS(sigs.size(), 2);
                for (int i = 0; i < 2; i++) {
                    XmlElement * sig = sigs[i];
                    TS_ASSERT_EQUALS(sig->getXmlData().getText(), "signalText");
                }

                QList<XmlElement *> props = subDoc.findElements("QtProperty");
                TS_ASSERT_EQUALS(props.size(), 3);
                for (int i = 0; i < 3; i++) {
                    XmlElement * prop = props[i];
                    TS_ASSERT_EQUALS(prop->getXmlData().getText(), "propertyText");
                }
            }

            TS_ASSERT(doc.addElement(subDoc.getRootElement()) != NULL);
            TS_ASSERT(doc.getRootElement()->getOwnerDocument() == &doc);

        }
    void test_createSubDocumentFromObservable()
        {
            TestXmlDocument doc;

            XmlElementInfo * qtObjInfo = doc.registerElement("QtObject", new QtObject, NULL);
            doc.registerElement("QtSignal", new QtSignal, qtObjInfo);
            doc.registerElement("QtProperty", new QtProperty, qtObjInfo);

            QObject * qObj = new QObject;
            qObj->setObjectName("Grinsekatze");
            Observable obsObj(qObj);

            XmlSubDocument subDoc = doc.createSubDocument(obsObj);
            TS_ASSERT(subDoc.getRootElement() != NULL);

            TS_ASSERT(doc.addElement(subDoc.getRootElement()) != NULL);
            TS_ASSERT(doc.getRootElement()->getOwnerDocument() == &doc);

        }

    void test_addRemoveElement()
        {
            TestXmlDocument doc;
            XmlElementInfo * root = doc.registerElement("QtObject", new TestXmlElement, NULL);
            doc.registerElement("QtSignal", new TestXmlElement(true), root);
            doc.registerElement("QtProperty", new TestXmlElement(true), root);

            XmlElement * obj = doc.getElement("QtObject");
            doc.setRootElement(obj);
            XmlElement * prop = doc.getElement("QtProperty");
            prop->setParent(obj);

            XmlElement * sig = doc.getElement("QtSignal");

            doc.addElement(sig);
            TS_ASSERT_EQUALS(obj->getChildCount(), 2);
            XmlElement * tmp;
            tmp = doc.findFirstElement("QtSignal");
            TS_ASSERT(tmp != NULL);
            if (tmp != NULL)
            {
                TS_ASSERT(tmp->getParent() == obj);
                TS_ASSERT(tmp->getOwnerDocument() == &doc);
            }

            tmp = doc.findFirstElement("QtProperty");
            TS_ASSERT(tmp != NULL);
            if (tmp != NULL)
            {
                TS_ASSERT(tmp->getOwnerDocument() == &doc);
                TS_ASSERT(tmp->getParent() == obj);
                tmp = doc.removeElement(tmp);
                TS_ASSERT(tmp->getOwnerDocument() == NULL);
                TS_ASSERT(tmp->getParent() == NULL);
                tmp = doc.findFirstElement("QtProperty");
                TS_ASSERT(tmp == NULL);
            }

            tmp = doc.removeElement(obj);
            TS_ASSERT(tmp != NULL);
            if (tmp != NULL)
            {
                TS_ASSERT(doc.getRootElement() == NULL);
                TS_ASSERT(tmp->getParent() == NULL);

                TS_ASSERT_EQUALS(tmp->getChildCount(), 1);
                tmp = tmp->findFirstElement("QtSignal");
                TS_ASSERT(tmp != NULL);

            }

            // adding NULL
            TS_ASSERT_THROWS(doc.addElement(NULL), std::runtime_error);

            // remove NULL
            TS_ASSERT_THROWS(doc.removeElement(NULL), std::runtime_error);

            // adding unknown element type
            TestXmlElement * xxx = new TestXmlElement(false);
            xxx->getXmlData().setTagName("XXX");
            TS_ASSERT_THROWS(doc.addElement(xxx), std::runtime_error);


//             // adding the same element twice
//             doc.addElement(obj);
//             TS_ASSERT(doc.addElement(obj), obj);

//             // adding duplicate elment
//             XmlElement * obj02 = doc.getElement("QtObject");
//             TS_ASSERT(obj02 != NULL);
//             TS_ASSERT_THROWS(doc.addElement(obj02), std::runtime_error);
//             delete obj02;

//            delete obj;
//            delete prop;


        }

    void test_QtObject()
        {
            QSlider qSlider1, qSlider2;
            qSlider1.setMinimum(0);
            qSlider2.setMinimum(1);
            qSlider1.setObjectName("XXX");
            qSlider2.setObjectName("YYY");

            QPushButton qButton;

            TestXmlDocument doc;
            XmlElementInfo * qtObjInfo = doc.registerElement("QtObject", new QtObject, NULL);
            doc.registerElement("QtProperty", new QtProperty, qtObjInfo);
            doc.registerElement("QtSignal", new QtSignal, qtObjInfo);

            XmlSubDocument subDoc = doc.createSubDocument(Observable(&qSlider1));
            XmlElement * xmlElem = subDoc.getRootElement();
            doc.addElement(xmlElem);

            xmlElem->connect(new Observer(&qSlider2));

            //// Test initial sync

            // QObject attribute: ObjectName SHOULD NOT be sync'ed (???). BUT: it IS
            // necessarily sync'ed as "objectName" is a property of class QObject and all
            // properties ARE sync'ed
//            TS_ASSERT_EQUALS(qSlider2.objectName(), "YYY");
            TS_ASSERT_EQUALS(qSlider2.objectName(), "XXX");

            // Property value
            TS_ASSERT_EQUALS(qSlider2.minimum(), 0);


            //// Test: Bind all QObject properties and signals

            xmlElem->bind();

            QtObject * qtObj = dynamic_cast<QtObject*>(xmlElem);

            QList<QtProperty*> qtPropList = qtObj->getProperties();
            TS_ASSERT(qtPropList.size() > 0);
            for (int i = 0; i < qtPropList.size(); i++) {
                TS_ASSERT(qtPropList[i]->isStateEnabled(XmlElement::BOUND_TO_OBSERVABLE));
            }

            QList<QtSignal*> qtSignalList = qtObj->getSignals();
            TS_ASSERT(qtSignalList.size() > 0);
            for (int i = 0; i < qtSignalList.size(); i++) {
                TS_ASSERT(qtSignalList[i]->isStateEnabled(XmlElement::BOUND_TO_OBSERVABLE));
            }

        }


    void test_QtProperty()
        {

            TestXmlDocument doc;
            doc.registerElement("QtProperty", new QtProperty, NULL);

            // We need a Qt property for the test
            QObject * qObj = new QObject;
            const QMetaObject * qMeta = qObj->metaObject();
            if (qMeta->propertyCount() == 0) {
                TS_FAIL("propertyCount() == 0");
                return;
            }

            // check the property
            QMetaProperty qProp = qMeta->property(0);
            if (!qProp.isReadable() || !qProp.isWritable()) {
                TS_FAIL("property not read/writable");
                return;
            }
            // set the test string
            QString value("Topfenstrudl");
            QVariant v(value);
            qProp.write(qObj, v);

            // create an Observable object
            Observable * obsObj = new Observable(qObj, qProp);

            // create the element and ..
            XmlElement * elem = doc.getElement(obsObj->type());
            QtProperty * qtProp = dynamic_cast<QtProperty*>(elem);
            TS_ASSERT(elem != NULL);
            if (elem == NULL)
                return;

            // INIT IT !!
            elem->init(obsObj);
//            elem->update(); // Does NOT work -> no QtObject -> no QObject

            TS_ASSERT_EQUALS(qtProp->getName(), qProp.name());
            TS_ASSERT_EQUALS(qtProp->getType(), qProp.typeName());
//             std::string text = qtProp->getXmlData().getText(true);
//             TS_ASSERT_EQUALS(text, "Topfenstrudl");
//             QVariant cmp(QString(text.c_str()));
//             TS_ASSERT_EQUALS(cmp, qProp.read(qObj));
//             TS_ASSERT_EQUALS("Topfenstrudl", qObj->property(qProp.name()).toString());
        }


//     void test_QtSignalParam()
//         {
//             TestXmlDocument doc;
//             doc.registerElement("QtSignal", new QtSignal, NULL);

//             // We need a Qt method signal (with at least one parameter) for the test
//             QObject * qObj = new QObject;
//             const QMetaObject * qMeta = qObj->metaObject();
//             QMetaMethod qSignal;
//             for (int i = 0; i < qMeta->methodCount(); i++) {
//                 qSignal = qMeta->method(i);
//                 if (qSignal.methodType() == QMetaMethod::Signal &&
//                     qSignal.parameterTypes().size() > 0)
//                 {
//                     // break as soon as we've found a signal
//                     break;
//                 }
//             }

//             QSignalParam qSignalParam(qSignal.parameterNames().operator[](0),
//                                       qSignal.parameterTypes().operator[](0));

//             // create an Observable object
//             Observable * obsObj = new Observable(&qSignalParam);

//             // create the element and ..
//             XmlElement * elem = doc.getElement(obsObj->type());
//             QtSignalParam * qtSignal = dynamic_cast<QtSignalParam*>(elem);

//             TS_ASSERT_EQUALS(qtSignal->getName(), "");
//             TS_ASSERT_EQUALS(qtSignal->getType(), "");

//             // INIT IT !!
//             elem->init(obsObj);

//             TS_ASSERT_EQUALS(qtSignal->getName(), qSignal.parameterNames().operator[](0));
//             TS_ASSERT_EQUALS(qtSignal->getType(), qSignal.parameterTypes().operator[](0));
//         }


    void test_QtSignal()
        {

            TestXmlDocument doc;
            doc.registerElement("QtSignal", new QtSignal, NULL);

            // We need a Qt method signal for the test
            QSlider * qObj = new QSlider;
            const QMetaObject * meta = qObj->metaObject();
            QMetaMethod qSignal = meta->method(meta->indexOfSignal("valueChanged(int)"));

            // create an Observable object
            Observable * obsObj = new Observable(qObj, qSignal);

            // create the element and ..
            XmlElement * elem = doc.getElement(obsObj->type());
            QtSignal * qtSignal = dynamic_cast<QtSignal*>(elem);
            TS_ASSERT(elem != NULL);
            if (elem == NULL)
                return;

            // INIT IT !!
            elem->init(obsObj);

            TS_ASSERT_EQUALS(qtSignal->getSignature(), qSignal.signature());
#if USE_SIGNALPARAM
            TS_ASSERT_EQUALS(elem->getChildCount(), qSignal.parameterTypes().size());
#endif
            TS_ASSERT_EQUALS(elem->getChildCount(), 0);
        }

    void test_createSubDocument_QObject()
        {

            TestXmlDocument doc;

            XmlElementInfo * qObjInfo = doc.registerElement("QtObject", new QtObject, NULL);
            doc.registerElement("QtSignal", new QtSignal, qObjInfo);
            doc.registerElement("QtProperty", new QtProperty, qObjInfo);

            // init qt objects (object, meta object etc)
            QObject * qObj = new QObject;
            qObj->setObjectName("objectName");


            // count the signals and properties of the object for later comparison
            const QMetaObject * qMeta = qObj->metaObject();
            int signalCount = 0;
            QList<QString> signalSigList;
            for (int i = 0; i < qMeta->methodCount(); i++) {
                QMetaMethod m = qMeta->method(i);
                if (m.methodType() == QMetaMethod::Signal && QtObjectUtils::isSerializable(m)) {
                    signalCount++;
                    signalSigList.push_back(m.signature());
                }
            }
            int propertyCount = 0;
            for (int i = 0; i < qMeta->propertyCount(); i++) {
                QMetaProperty p = qMeta->property(i);
                if (QtObjectUtils::isSerializable(p)) {
                    propertyCount++;
                }
            }

            // NOTE: Method actually under test (!)
            Observable obs(qObj);
            XmlSubDocument subDoc = doc.createSubDocument(obs);

            XmlElement * elem = subDoc.getRootElement();

            QtObject * qtObj = dynamic_cast<QtObject*>(elem);

            // test qobject and element child count
            TS_ASSERT_EQUALS(elem->getChildCount(), (signalCount + propertyCount));
            TS_ASSERT_EQUALS(elem->getXmlData().getTagName(), "QtObject");
            TS_ASSERT_EQUALS(qtObj->getClassName(), "QObject");
            TS_ASSERT_EQUALS(qtObj->getObjectName(), "objectName");

            TS_ASSERT_EQUALS(elem->getChildCount(), (signalCount + propertyCount));

            // test signals
            QList<XmlElement *> sigs = elem->findElements("QtSignal");
            TS_ASSERT_EQUALS(sigs.size(), signalCount);
            for (int i = 0; i < sigs.size(); i++) {
                QtSignal * sig = (QtSignal*)sigs[i];
                TS_ASSERT_EQUALS(sig->getSignature(), signalSigList[i]);
            }

            // test properties
            QList<XmlElement *> props = elem->findElements("QtProperty");
            TS_ASSERT_EQUALS(props.size(), propertyCount);
            for (int i = 0; i < props.size(); i++) {
                QtProperty * prop = (QtProperty*)props[i];
                TS_ASSERT_EQUALS(prop->getName(), qMeta->property(i).name());
                TS_ASSERT_EQUALS(prop->getType(), qMeta->property(i).typeName());
//                 QByteArray text = prop->getXmlData().getText(true);
//                 QDataStream s(&text, QIODevice::ReadOnly);
//                 QVariant cmp;
//                 s >> cmp;
//                 TS_ASSERT(cmp.isValid());
//                 TS_ASSERT_EQUALS(cmp.toString().toStdString(),
//                                  qObj->property(qMeta->property(i).name()).toString().toStdString());
            }

            TS_ASSERT(qtObj->getAdapter() != NULL);

            qDebug("DOM = \n%s", qtObj->getDom().toString().toStdString().c_str());

        }

    void test_findElementWithObservable()
        {
            TestXmlDocument doc;
            doc.registerElement("abc", new QtObject, NULL);

            // add element (init with observable)
//            QObject * qObj = new QObject; // NOTE: Seg-fault problems within XmlDocument dtor vanished !?!?!
            QObject qObj;
            Observable * obs = new Observable(&qObj);
            XmlElement * elem = doc.getElement("abc");
            TS_ASSERT(elem != NULL);
            elem->init(obs);
            elem = doc.addElement(elem);

            TS_ASSERT(doc.getRootElement() == elem);

            // find observable directly: with original object
            TS_ASSERT(doc.findElement(*obs) != NULL);
            // find observable directly: with "copy"
            Observable obsCopy(*obs);
            TS_ASSERT(doc.findElement(obsCopy) != NULL);

            // clear the document
            elem = doc.getRootElement();
            delete elem;
            doc.setRootElement(NULL);

            // add element (init with xml)
            QDomDocument domDoc;
            QDomElement domElem = domDoc.createElement("abc");
            domDoc.appendChild(domElem);
            domElem.setAttribute("class-name", "QObject");

            qObj.setObjectName("Katzenklo");
            domElem.setAttribute("object-name", "Katzenklo");

            XmlSubDocument subDoc = doc.createSubDocument(domDoc);
            XmlElement * e2 = subDoc.getRootElement();
            doc.addElement(e2);

            // find element with the help of an observable
            TS_ASSERT(doc.findElementExtended(Observable(&qObj)) != NULL);

            // find with copy of XmlData
            TS_ASSERT(e2->findFirstElement(e2->getXmlData()) == e2);


        }

};
