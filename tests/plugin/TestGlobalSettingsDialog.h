// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <plugin/GlobalSettingsDialog.h>
#include <xml/XmlConfigFileDocument.h>
#include <xml/UISettings.h>

class GlobalSettingsDialogTest : public CxxTest::TestSuite
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
            XmlConfigFileDocument doc;
            doc.init();

            GlobalSettingsDialog dialog(doc.getUISettings());

        }
};
