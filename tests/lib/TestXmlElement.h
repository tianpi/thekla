// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/XmlElement.h>
#include <common/Observable.h>

class XmlElementTest : public CxxTest::TestSuite
{
private:
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

    void testDefaultCtor()
        {
            TestXmlElement e;
            TS_ASSERT(e.getOwnerDocument() == NULL);
            TS_ASSERT(e.getParent() == NULL);
//            TS_ASSERT(e.getXmlData() != NULL);
            TS_ASSERT_EQUALS(e.getChildCount(), 0);
            TS_ASSERT(e.getObservable() == NULL);
            TS_ASSERT(e.getObservers().isEmpty());
            TS_ASSERT(e.isLeaf());
            TS_ASSERT_EQUALS(e.isStateEnabled(XmlElement::BOUND_TO_OBSERVABLE), false);
        }

        void testAddParent()
        {
            TestXmlElement p;
            TestXmlElement c1;
            XmlElement * ret;

            ret = c1.setParent(NULL);
            TS_ASSERT(ret == NULL);

            ret = c1.setParent(&p);
            TS_ASSERT(ret == NULL);
            TS_ASSERT_EQUALS(c1.getParent(), &p);
            TS_ASSERT_EQUALS(p.getChildCount(), 1);
            TS_ASSERT_EQUALS(p.getChild(0), &c1);

            TS_ASSERT_EQUALS(p.isLeaf(), false);
            TS_ASSERT_EQUALS(c1.isLeaf(), true);

            ret = c1.setParent(&c1);
            TS_ASSERT(ret == NULL);
            ret = c1.setParent(&p);
            TS_ASSERT(ret == &p);

            ret = c1.setParent(NULL);
            TS_ASSERT_EQUALS(ret, &p);
            TS_ASSERT(c1.getParent() == NULL);
            TS_ASSERT_EQUALS(p.getChildCount(), 0);

        }

    void testDtor()
        {
            // as good as it gets ;-)

            TestXmlElement * p = new TestXmlElement;
            TestXmlElement * c1 = new TestXmlElement;
            TestXmlElement * c2 = new TestXmlElement;

            c1->setParent(p);
            c2->setParent(p);
            TS_ASSERT_EQUALS(p->getChildCount(), 2);

            // element removes itself from parent child list
            delete c2;
            TS_ASSERT_EQUALS(p->getChildCount(), 1);

            delete p;
            // NOTE: Result yields NO seg-fault. Thanx to very tolerant Linux OS ;)
//            p->getParent(); // SEG-FAULT !
//            TS_ASSERT(c1->getParent() == NULL); // SEG-FAULT !

        }
    void testXPath()
        {
            TestXmlElement p;
            p.getXmlData().setTagName("QtObject");
            p.getXmlData().setAttribute("class", "QSlider");
            TestXmlElement c1;
            c1.getXmlData().setTagName("QtSignal");
            c1.getXmlData().setAttribute("signature", "valueChanged(int)");
            TestXmlElement c2;
            c2.getXmlData().setTagName("QtProperty");
            c2.getXmlData().setAttribute("name", "minimum");
            c2.getXmlData().setAttribute("type", "int");

            c1.setParent(&p);
            c2.setParent(&p);

            TS_ASSERT_EQUALS(c1.getXPath().toStdString(), "/QtObject[@class='QSlider']/QtSignal[@signature='valueChanged(int)']");
        }

    void testDom()
        {
            TestXmlElement p;
            p.getXmlData().setTagName("QtObject");
            p.getXmlData().setAttribute("class", "QSlider");
            TestXmlElement c1(true);
            c1.getXmlData().setTagName("QtSignal");
            c1.getXmlData().setAttribute("signature", "valueChanged(int)");
            c1.getXmlData().setText("20", false);
            TestXmlElement c2(true);
            c2.getXmlData().setTagName("QtProperty");
            c2.getXmlData().setAttribute("name", "minimum");
            c2.getXmlData().setAttribute("type", "int");
            c2.getXmlData().setText("0", false);

            c1.setParent(&p);
            c2.setParent(&p);

            QDomDocument doc = p.getDom();
            QDomElement rootElem = doc.documentElement();
            TS_ASSERT_EQUALS(rootElem.tagName(), "QtObject");
            TS_ASSERT_EQUALS(rootElem.attribute("class"), "QSlider");
            QDomNodeList children = rootElem.childNodes();
            TS_ASSERT_EQUALS(children.count(), 2);
            QDomElement childElem[children.count()];
            for (int i=0; i < children.count(); i++) {
                QDomNode child = children.item(i);
                TS_ASSERT(child.isElement());
                childElem[i] = child.toElement();
                if (childElem[i].tagName() != "QtSignal") {
                    TS_ASSERT_EQUALS(childElem[i].tagName(), "QtProperty");
                    TS_ASSERT_EQUALS(childElem[i].attribute("name"), "minimum");
                    TS_ASSERT_EQUALS(childElem[i].text(), "0");
                } else {
                    TS_ASSERT_EQUALS(childElem[i].tagName(), "QtSignal");
                    TS_ASSERT_EQUALS(childElem[i].attribute("signature"), "valueChanged(int)");
                    TS_ASSERT_EQUALS(childElem[i].text(), "20");
                }
            }
        }
    void testInitWithDomElem()
        {
            QDomDocument doc;
            QDomElement elem = doc.createElement("QtSignal");
            doc.appendChild(elem);
            elem.setAttribute("signature", "valueChanged(int)");
            QDomText text = doc.createTextNode("20");
            elem.appendChild(text);

            // Test :: containsText == true == QDomElement contains Text

            TestXmlElement e(true);
            e.init(elem);

            QDomDocument cmpDoc = e.getDom();
            QDomElement cmpElem = cmpDoc.documentElement();
            TS_ASSERT_EQUALS(cmpElem.tagName(), "QtSignal");
            TS_ASSERT_EQUALS(cmpElem.attribute("signature"), "valueChanged(int)");
            TS_ASSERT_EQUALS(cmpElem.text(), "20");

            // Test: containsText == true != QDomElement contains -> see init(QDomElement)

            TestXmlElement e2(false);
            e2.init(elem);

            QDomDocument cmpDoc2 = e2.getDom();
            QDomElement cmpElem2 = cmpDoc2.documentElement();
            TS_ASSERT_EQUALS(cmpElem2.text(), "");
        }
//     void testInitWithDomDoc()
//         {
//             QDomDocument doc;
//             QDomElement elem = doc.createElement("QtSignal");
//             doc.appendChild(elem);
//             elem.setAttribute("signature", "valueChanged(int)");
//             QDomText text = doc.createTextNode("20");
//             elem.appendChild(text);

//             TestXmlElement e;
//             e.init(doc);

//             QDomDocument cmpDoc = e.getDom();
//             QDomElement cmpElem = cmpDoc.documentElement();
//             TS_ASSERT_EQUALS(cmpElem.tagName(), "QtSignal");
//             TS_ASSERT_EQUALS(cmpElem.attribute("signature"), "valueChanged(int)");
//             TS_ASSERT_EQUALS(cmpElem.text(), "20");

//             // TODO: see TestXmlElement.cpp
//         }
    void test_hasObservable_etc()
        {
            TestXmlElement e;

            QObject qObj;
            Observable o(&qObj);

            TS_ASSERT(!e.hasObservable());
            TS_ASSERT(!(e == o));

            // TODO: more tests
        }
    void test_CopyCtor()
        {
            TestXmlElement e;
            e.getXmlData().setTagName("tag-name");
            TestXmlElement c1, c2;
            c1.setParent(&e);
            c2.setParent(&e);

            TestXmlElement copy(e);
            TS_ASSERT(copy.getParent() == NULL);
            TS_ASSERT_EQUALS(copy.getChildCount(), 0);
            TS_ASSERT_EQUALS(e.getXmlData(), copy.getXmlData());
            TS_ASSERT(copy.getObservable() == NULL);
            TS_ASSERT(copy.getObservers().isEmpty());
        }
};
