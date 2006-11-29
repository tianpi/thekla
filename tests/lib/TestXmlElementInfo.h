// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/XmlElementInfo.h>
#include <xml/XmlElement.h>
#include <common/Observable.h>


class XmlElementInfoTest : public CxxTest::TestSuite
{
private:

    class TestXmlElementInfo : public XmlElementInfo
    {
    private:
        friend class XmlElementInfoTest;
    public:
        TestXmlElementInfo(const QString & tagName, XmlElement * impl, XmlElementInfo * parentInfo = NULL)
            : XmlElementInfo(tagName, impl, parentInfo)
            {};
    };

    class TestXmlElement : public XmlElement
    {
    public:
        TestXmlElement()
            : XmlElement(false)
            {};
    public:
        virtual bool canInit(Observable::Type obsType) const {
            switch (obsType) {
            case Observable::QOBJECT:
                return true;
            default:
                return false;
            }
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
    void test_Basics()
        {
            TestXmlElementInfo parent("QtObject", NULL, NULL);
            TestXmlElementInfo child("QtSignal", NULL, &parent);

            TS_ASSERT_EQUALS(parent.getTagName(), "QtObject");
            TS_ASSERT_EQUALS(child.getParentInfo(), &parent);
            TS_ASSERT_EQUALS(child.getParentInfo()->getTagName(), "QtObject");

            TestXmlElementInfo root("THEKLA", NULL, NULL);
            TS_ASSERT(root.getParentInfo() == NULL);
            parent.setParentInfo(&root);
            TS_ASSERT_EQUALS(parent.getParentInfo()->getTagName(), "THEKLA");

        }
    void test_EqualOp()
        {
            TestXmlElementInfo info("QtObject", NULL, NULL);
            TestXmlElementInfo child("QtSignal", NULL, &info);

            // tag name
            TS_ASSERT(info == QString("QtObject"));
            TS_ASSERT(info == info);
            TS_ASSERT(!(info == child));

//             // element implementation
//             TestXmlElementInfo i1("QtObject", new TestXmlElement, NULL);
//             TestXmlElementInfo i2("QtSignal", new TestXmlElement, NULL);
//             TS_ASSERT(i1 == i2.getImpl());
//             TS_ASSERT(!(i1 == i2));

            // Observable
            TestXmlElement * e1 = new TestXmlElement;
            TestXmlElementInfo i3("QtObject", e1, NULL);
            QObject obj;
            Observable obs(&obj);
            TS_ASSERT(e1->canInit(obs.type()));
            TS_ASSERT(i3 == obs.type());
        }
};
