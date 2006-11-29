// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/XmlTrashDocument.h>
#include <xml/QtObject.h>

#include <QSlider>

class XmlTrashDocumentTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {
        }
    void tearDown()
        {
        }

    void test_Ctor()
        {
            XmlTrashDocument doc;

            XmlElement * rootElem = doc.getRootElement();
            TS_ASSERT(rootElem != NULL);
            if (rootElem == NULL) return;
            TS_ASSERT_EQUALS(rootElem->getChildCount(), 0);
        }
    void test_addRemove_QtObject()
        {
            // create document

            XmlTrashDocument doc;
            XmlElement * rootElem = doc.getRootElement();

            // create some QtObject elements

            QSlider qObj1, qObj2, qObj3;
            QtObject qtObj1, qtObj2, qtObj3;
            qtObj1.getXmlData().setTagName("QtObject");
            qtObj2.getXmlData().setTagName("QtObject");
            qtObj3.getXmlData().setTagName("QtObject");
            qtObj1.init(new Observable(&qObj1));
            qtObj2.init(new Observable(&qObj2));
            qtObj3.init(new Observable(&qObj3));

            // add first element (each gets a "trash-id")

            doc.push(&qtObj1);
            TS_ASSERT_EQUALS(rootElem->getChildCount(), 1);
            TS_ASSERT(qtObj1.getXmlData().hasAttribute("trash-id"));

            // add second element (duplicate of first one)

            doc.push(&qtObj2);
            TS_ASSERT_EQUALS(rootElem->getChildCount(), 2);
            TS_ASSERT(qtObj2.getXmlData().hasAttribute("trash-id"));

            // remove first element

            QtObject * qtObj1Ret = doc.get(&qObj1);
            TS_ASSERT(qtObj1Ret != NULL);
            if (qtObj1Ret == NULL) return;
            TS_ASSERT_EQUALS(rootElem->getChildCount(), 2);
            TS_ASSERT(!qtObj1Ret->getXmlData().hasAttribute("trash-id"));

            // add another element (-> found "trash-id" assignment bug, from that time on
            // EACH element gets a trash-id)

            doc.push(&qtObj3);
            TS_ASSERT_EQUALS(rootElem->getChildCount(), 3);

            // remove non-existant element
            qtObj1Ret = doc.get(&qObj1);
            TS_ASSERT(qtObj1Ret != NULL);
            TS_ASSERT_EQUALS(rootElem->getChildCount(), 3);

        }
};
