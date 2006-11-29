// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/XmlSubDocument.h>
#include <xml/XmlElement.h>

class XmlSubDocumentTest : public CxxTest::TestSuite
{
private:
    class TestXmlSubDocument : public XmlSubDocument
    {
    private:
        friend class XmlSubDocumentTest;
    public:
        TestXmlSubDocument()
            : XmlSubDocument()
            {};
        TestXmlSubDocument(XmlElement * rootElem)
            : XmlSubDocument(rootElem)
            {};
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
    void test_setRootElement()
        {
            TestXmlElement e;
            TestXmlSubDocument d(&e);

            TS_ASSERT_EQUALS(d.getRootElement(), &e);

            XmlElement * tmp;

            TestXmlElement e2;
            tmp = d.setRootElement(&e2);
            TS_ASSERT_EQUALS(d.getRootElement(), &e2);
            TS_ASSERT_EQUALS(tmp, &e);
        }

    void test_findElement()
        {
            TestXmlSubDocument d;

            TestXmlElement e;
            e.getXmlData().setTagName("e");
            TestXmlElement e1;
            e1.getXmlData().setTagName("e1");
            e1.getXmlData().setAttribute("name", "value");
            e1.setParent(&e);
            TestXmlElement e1a;
            e1a.getXmlData().setTagName("e1");
            e1a.setParent(&e);
            TestXmlElement e2;
            e2.getXmlData().setTagName("e2");
            e2.setParent(&e);

//            d.setRootElement(e);

            XmlElement * elem;
            QList<XmlElement*> elemList;

            // NOTE: no root element (!)
            elem = d.findFirstElement("e1");
            TS_ASSERT(elem == NULL);

            // IMPORTANT: set root element (!)
            d.setRootElement(&e);

            elem = d.findFirstElement("e1");
            TS_ASSERT(elem == &e1);

            elemList = d.findElements("e1");
            TS_ASSERT_EQUALS(elemList.size(), 2);

            XmlElementData key;
            key.setTagName("e1");
            key.setAttribute("name", "value");
            elem = d.findFirstElement(key);
            TS_ASSERT(elem == &e1);

        }

    void test_getDom()
        {
            TestXmlSubDocument d;

            TestXmlElement e;
            d.setRootElement(&e);
            e.getXmlData().setTagName("e");
            TestXmlElement e1(true);
            e1.getXmlData().setTagName("e1");
            e1.getXmlData().setAttribute("name", "value");
            e1.getXmlData().setText("text", false);
            e1.setParent(&e);

            QDomDocument doc = d.getDom();
            QDomElement elem = doc.documentElement();
            TS_ASSERT(!elem.isNull());
            TS_ASSERT_EQUALS(elem.tagName(), "e");
            QDomElement elem2 = elem.firstChildElement("e1");
            TS_ASSERT(!elem2.isNull());
            TS_ASSERT_EQUALS(elem2.tagName(), "e1");
            TS_ASSERT_EQUALS(elem2.attribute("name"), "value");
            TS_ASSERT_EQUALS(elem2.text(), "text");

        }

};
