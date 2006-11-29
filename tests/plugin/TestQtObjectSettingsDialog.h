// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <plugin/QtObjectSettingsDialog.h>
#include <xml/XmlConfigFileDocument.h>
#include <common/Observable.h>
#include <xml/XmlElement.h>
#include <xml/QtObject.h>

#include <QSlider>

class QtObjectSettingsDialogTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {
        }
    void tearDown()
        {
        }
    void test_init()
        {
            XmlConfigFileDocument doc;
            doc.init();

            QSlider qObj;
            qObj.setObjectName("Idefix");

            XmlSubDocument subDoc = doc.createSubDocument(Observable(&qObj));
            QtObject * qtObj = dynamic_cast<QtObject*>(subDoc.getRootElement());

            QtObjectSettingsDialog dialog(qtObj);
            dialog.init();

        }

};
