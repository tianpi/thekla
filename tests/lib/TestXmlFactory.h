// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/XmlFactory.h>
#include <xml/XmlElementInfo.h>
#include <common/Observable.h>
#include <xml/XmlElement.h>
#include <xml/QtObject.h>
#include <xml/QtSignal.h>
#include <xml/QtProperty.h>


class XmlFactoryTest : public CxxTest::TestSuite
{

private:

    class TestXmlFactory : public XmlFactory
    {
    private:
        friend class XmlFactoryTest;
    public:
        TestXmlFactory()
            : XmlFactory()
            {};
    };

    class TestXmlElement : public XmlElement
    {
    private:
        friend class XmlFactoryTest;
    public:
        TestXmlElement(bool b = false)
            : XmlElement(b)
            {};
        TestXmlElement(const TestXmlElement & copy)
            : XmlElement(copy)
            {};
    public:
        virtual XmlElement * clone() const { return new TestXmlElement(*this); }
        virtual bool canInit(Observable::Type obsType) {
            if (obsType == Observable::QOBJECT)
                return true;
            return false;
        }
    };

    class TestXmlElement02 : public TestXmlElement
    {
    private:
        friend class XmlFactoryTest;
    public:
        TestXmlElement02(bool b = false)
            : TestXmlElement(b)
            {};
        TestXmlElement02(const TestXmlElement02 & copy)
            : TestXmlElement(copy)
            {};
    public:
        virtual XmlElement * clone() const { return new TestXmlElement02(*this); }
        virtual bool supports(const Observable & observable) {
            if (observable.type() == Observable::QOBJECTSIGNAL)
                return true;
            return false;
        }
    };



public:
    void setUp()
        {
        }
    void tearDown()
        {
        }

    void test_getInfo()
        {
            TestXmlFactory f;

            XmlElementInfo * root = f.registerElement("THEKLA", NULL, NULL);
            XmlElementInfo * qtObj = f.registerElement("QtObject", new QtObject, root);

             // tag name
            TS_ASSERT_EQUALS(f.getInfo("THEKLA")->getTagName(), "THEKLA");
            TS_ASSERT_EQUALS(f.getInfo("QtObject")->getTagName(), "QtObject");
            TS_ASSERT(*(f.getInfo("QtObject")->getParentInfo()) == *root);
            TS_ASSERT(f.getInfo("XXX") == NULL);

            // TODO: observable
            QObject * obj = new QObject;
            Observable o(obj);
            XmlElementInfo * i1 = f.getInfo(o.type());
            TS_ASSERT(i1 != NULL);
            TS_ASSERT(i1->operator==(*qtObj));

            // element implementation
//            TS_ASSERT(*(f.getInfo(new XmlElement)) == *qtObj);
        }

    void test_registerElements()
        {
            TestXmlFactory f;

            // register default elements such as TheklaUIData, QtObject, ..
            f.registerDefaultElements();
            TS_ASSERT(f.getElementCount() > 0)
            // unregister all elements
            f.unregisterAllElements();
            TS_ASSERT_EQUALS(f.getElementCount(), 0)

            // add root element
            XmlElementInfo * root = f.registerElement("THEKLA", NULL, NULL);
            // add QtObject with THEKLA as root
            XmlElementInfo * qtObj = f.registerElement("QtObject", new TestXmlElement, root);

            TS_ASSERT_EQUALS(qtObj->getImpl()->getXmlData().getTagName(), "QtObject");
            // NOTE: Test with XmlDocument + XmlFactory(!)
            TS_ASSERT(qtObj->getImpl()->getOwnerDocument() == NULL);


            // registering additional
            XmlElementInfo * qtSig = f.registerElement("QtSignal", NULL, qtObj);
            XmlElementInfo * qtProp = f.registerElement("QtProperty", NULL, qtObj);

            QList<XmlElementInfo*> childList = f.getChildInfos(qtObj);
            TS_ASSERT_EQUALS(childList.size(), 2);

            // unregister QtObject -> reset THEKLA as parent element of QtSignal,
            // QtProperty (that are the actual children of QtObject)
            f.unregisterElement(qtObj);
            TS_ASSERT(f.getInfo("THEKLA") != NULL);
            childList = f.getChildInfos(root);
            TS_ASSERT_EQUALS(childList.size(), 2);
            TS_ASSERT(*(qtSig->getParentInfo()) == *root);
            TS_ASSERT(*(qtProp->getParentInfo()) == *root);

            // add another element with THEKLA as parent
            XmlElementInfo * oivField = f.registerElement("OivField", NULL, root);
            childList = f.getChildInfos(root);
            TS_ASSERT_EQUALS(childList.size(), 3);
            TS_ASSERT(*(oivField->getParentInfo()) == *root);

            // re-register QtObject
            qtObj = f.registerElement("QtObject", NULL, root);

            // re-set QtObject as parent of QtSignal, QtProperty
            qtSig->setParentInfo(qtObj);
            qtProp->setParentInfo(qtObj);
            childList = f.getChildInfos(root);
            TS_ASSERT_EQUALS(childList.size(), 2);
            TS_ASSERT(*(qtObj->getParentInfo()) == *root);

            // unregister QtSignal (from parent QtObject)
            f.unregisterElement(qtProp);
            childList = f.getChildInfos(qtObj);
            TS_ASSERT_EQUALS(childList.size(), 1);
            TS_ASSERT(*(qtSig->getParentInfo()) == *qtObj);

        }
    void test_Dtor()
        {
            TestXmlFactory * f = new TestXmlFactory;
            XmlElementInfo * i;
            i = f->registerElement("QtObject", NULL, NULL);
            i = f->registerElement("QtSignal", NULL, i);
            i = f->registerElement("QtProperty", NULL, i);
            f->unregisterElement(i);

            delete f;

        }
    void test_getElement()
        {
            TestXmlFactory f;

            XmlElementInfo * rootInfo = f.registerElement("THEKLA", NULL, NULL);
            XmlElementInfo * qtObjInfo = f.registerElement("QtObject", new QtObject, rootInfo);
            XmlElementInfo * qtSigInfo = f.registerElement("QtSignal", new QtSignal, qtObjInfo);
            XmlElementInfo * qtPropInfo = f.registerElement("QtProperty", new QtProperty, qtObjInfo);

            Q_UNUSED(qtSigInfo);
            Q_UNUSED(qtPropInfo);

            // tag name
            XmlElement * sig = f.getElement("QtSignal");
            TS_ASSERT(sig != NULL);
            TS_ASSERT_EQUALS(sig->getXmlData().getTagName(), "QtSignal");
            delete sig;

            XmlElement * obj;

            // observable
            QObject qObj;
            Observable obs(&qObj);
            obj = f.getElement(obs.type());
            TS_ASSERT(obj != NULL);
            TS_ASSERT_EQUALS(obj->getXmlData().getTagName().toStdString(), "QtObject");
            delete obj;


//             // element implementation
//             obj = f.getElement(new TestXmlElement);
//             TestXmlElement * cmp = new TestXmlElement;
//             TS_ASSERT(obj != NULL);
//             TS_ASSERT_EQUALS(obj->getXmlData().getTagName().toStdString(), "QtObj");
//             TS_ASSERT_EQUALS(std::string(typeid(obj).name()), std::string(typeid(cmp).name()));
//             delete cmp;
//             delete obj;

        }

};
