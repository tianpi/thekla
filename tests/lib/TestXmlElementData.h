// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/XmlElementData.h>
#include <common/Base64.h>

class XmlElementDataTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {
        }
    void tearDown()
        {
        }

    void testCtor( void )
        {
            XmlElementData d(true);

            d.setTagName("tag-name");
            d.setAttribute("name", "value");
            d.setText("text", false);

            TS_ASSERT_EQUALS(d.getTagName(), "tag-name");
            TS_ASSERT(d.hasAttribute("name"));
            TS_ASSERT_EQUALS(d.getAttribute("name"), "value");

            TS_ASSERT_EQUALS(d.getAttributeCount(), 1);
            TS_ASSERT_EQUALS(d.getAttributeName(0), "name");
            TS_ASSERT_EQUALS(d.getAttributeValue(0), "value");
            TS_ASSERT_EQUALS(d.getText(), "text");

            XmlElementData copy(d);
            TS_ASSERT_EQUALS(copy.getTagName(), "tag-name");
            TS_ASSERT_EQUALS(copy.getAttributeCount(), 1);
            TS_ASSERT_EQUALS(copy.getText(), "text");

            d.setTagName("have-separate-copies");
            TS_ASSERT_EQUALS(d.getTagName(), "have-separate-copies");
            TS_ASSERT_EQUALS(copy.getTagName(), "tag-name");

        }
    void testCtor2()
        {
            QDomDocument doc;
            QDomElement elem = doc.createElement("tag-name");
            elem.setAttribute("name", "value");

            XmlElementData d(elem);
            TS_ASSERT_EQUALS(d.getTagName(), "tag-name");
            TS_ASSERT_EQUALS(d.getAttributeCount(), 1);
            TS_ASSERT_EQUALS(d.getAttributeName(0), "name");
            TS_ASSERT_EQUALS(d.getAttributeValue(0), "value");
            TS_ASSERT_EQUALS(d.getText(), "");

            QDomElement elem2 = doc.createElement("xxx");
            QDomText text = doc.createTextNode("text");
            elem2.appendChild(text);

            XmlElementData d2(elem2);
            TS_ASSERT_EQUALS(d2.getText(), "text");
        }

    void test_getSetText()
        {
            XmlElementData d;
            TS_ASSERT_EQUALS(d.containsText(), false);
            d.setText("text", false);
            d.getText().clear();

            XmlElementData d2(true);
            d2.setText("text", false);
            TS_ASSERT_EQUALS(d2.getText(), "text");
            d2.setText("text", true);
            TS_ASSERT_EQUALS(d2.getText(), Base64::encode("text"));
            TS_ASSERT_EQUALS(d2.getText(true), "text");

        }

    void test_equalsOperator()
        {
            XmlElementData d1, d2;
            TS_ASSERT_EQUALS(d1, d2);

            d1.setAttribute("name", "value");
            d2.setAttribute("x", "y");
            d2.setAttribute("name", "value");
            d1.setAttribute("x", "y");
            TS_ASSERT_EQUALS(d1, d2);

            d1.setTagName("NO");
            TS_ASSERT_EQUALS(d1 == d2, false);
            d2.setTagName("NO");
            TS_ASSERT_EQUALS(d1, d2);

            d2.setText("text");
            TS_ASSERT_EQUALS(d1, d2);
            d1.setAttribute("x", "z");
            TS_ASSERT_DIFFERS(d1, d2);

            XmlElementData d3(true), d4(true);
            TS_ASSERT_EQUALS(d3, d4);
            d4.setText("text", false);
            TS_ASSERT_EQUALS(d3 == d4, false);
            d3.setText("text", false);
            TS_ASSERT_EQUALS(d3, d4);
        }
    void test_contains()
        {

            XmlElementData d1(true), d2;
            TS_ASSERT(d1.contains(d2));

            d1.setAttribute("name", "value");
            d1.setAttribute("x", "y");
            d2.setAttribute("x", "y");
            TS_ASSERT(d1.contains(d2));
            TS_ASSERT(!d2.contains(d1));

        }


    void testXPath()
        {
            XmlElementData d(true);
            d.setTagName("QtObject");
            d.setAttribute("class-name", "QSlider");
            d.setAttribute("object-name", "zAxisSlider");
            d.setText("20", false);
            TS_ASSERT_EQUALS(d.getXPath().toStdString(), "/QtObject[@class-name='QSlider'][@object-name='zAxisSlider']");
        }

    void testDom()
        {
            XmlElementData d(true);
            d.setTagName("QtObject");
            d.setAttribute("class-name", "QSlider");
            d.setAttribute("object-name", "zAxisSlider");
            d.setText("20", false);

            QDomDocument doc = d.getDom();
            QDomElement elem = doc.documentElement();

            TS_ASSERT_EQUALS(elem.tagName(), "QtObject");
            TS_ASSERT(elem.hasAttribute("class-name"));
            TS_ASSERT_EQUALS(elem.attribute("class-name"), "QSlider");
            TS_ASSERT_EQUALS(elem.text(), "20");

            XmlElementData cmp(elem);// doc.documentElement().cloneNode(true).toElement());
            TS_ASSERT_EQUALS(d, cmp);

            QDomDocument doc2 = cmp.getDom();
            QDomElement elem2 = doc2.documentElement();

            TS_ASSERT_EQUALS(elem2.tagName(), "QtObject");
            TS_ASSERT(elem2.hasAttribute("class-name"));
            TS_ASSERT_EQUALS(elem2.attribute("object-name"), "zAxisSlider");
            TS_ASSERT_EQUALS(elem2.text(), "20");

        }

    void test_ConfigFlag()
        {
            XmlElementData d;

            TS_ASSERT_EQUALS(d.getConfigFlag(XmlElementData::BIND), false);
            d.setConfigFlag(XmlElementData::BIND, true);
            TS_ASSERT_EQUALS(d.getConfigFlag(XmlElementData::BIND), true);
            d.setConfigFlag(XmlElementData::BIND, false);
            TS_ASSERT_EQUALS(d.getConfigFlag(XmlElementData::BIND), false);

        }

    void test_merge()
        {

            XmlElementData d1(true), d2(true);

            d1.setAttribute("one", "two");
            d2.setAttribute("two", "one");

            d1.setText("hello");
            d2.setText("world");

            d1.merge(d2);
            TS_ASSERT_EQUALS(d1.getAttribute("one"), "two");
            TS_ASSERT_EQUALS(d1.getAttribute("two"), "one");
            TS_ASSERT_EQUALS(d1.getText(), "world");

        }

    void test_operatorAssign()
        {

            XmlElementData * d1 = new XmlElementData(true);

            d1->setAttribute("one", "two");
            d1->setText("RFID-chicken-are-sad");

            XmlElementData d2(false);
            d2.setAttribute("two", "one");

            XmlElementData d3(true);
            d3 = d2;
            TS_ASSERT_EQUALS(d3.getAttribute("two"), "one");
            TS_ASSERT(!d3.containsText());

            d2 = *d1;

            delete d1;

            TS_ASSERT_EQUALS(d2.getAttribute("one"), "two");
            TS_ASSERT(!d2.hasAttribute("two"));
            TS_ASSERT_EQUALS(d2.getText(), "RFID-chicken-are-sad");


            XmlElementData d4 = d3;

            TS_ASSERT_EQUALS(d4.getAttribute("two"), "one");
            TS_ASSERT(!d4.containsText());

        }

    void test_removeAttrib()
        {
            XmlElementData d1;

            d1.setAttribute("heLLo", "world");
            d1.setAttribute("Donald", "Duck");

            d1.removeAttribute("heLLo");
            TS_ASSERT(d1.hasAttribute("Donald"));
            TS_ASSERT(!d1.hasAttribute("heLLo"));

            d1.removeAttribute("Daisy");
            TS_ASSERT(d1.hasAttribute("Donald"));

        }

    void test_clear()
        {
            XmlElementData d1(true);

            d1.setTagName("Entenhausen");
            d1.setAttribute("Donald", "Duck");
            d1.setText("Hmmmm");

            d1.clear();

            TS_ASSERT_EQUALS(d1.getTagName(), "");
            TS_ASSERT(!d1.hasAttribute("Donald"));
            TS_ASSERT_EQUALS(d1.getText(), "");
            TS_ASSERT(d1.containsText());

            XmlElementData d2(false);

            d2.setTagName("Entenhausen");
            d2.setAttribute("Donald", "Duck");

            d2.clear();

            TS_ASSERT_EQUALS(d2.getTagName(), "");
            TS_ASSERT(!d2.hasAttribute("Donald"));
            TS_ASSERT_EQUALS(d2.getText(), "");
            TS_ASSERT(!d2.containsText());


        }



};
