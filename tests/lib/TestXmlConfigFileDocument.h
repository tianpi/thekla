// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/XmlConfigFileDocument.h>
#include <xml/UIDataRoot.h>
#include <xml/UISettings.h>

#include <QSlider>

class XmlConfigFileDocumentTest : public CxxTest::TestSuite
{
private:
    DatabaseConn * dbConn_;
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

            UIDataRoot * uiDataRoot = dynamic_cast<UIDataRoot*>(doc.getRootElement());
            TS_ASSERT(uiDataRoot != NULL);
            TS_ASSERT_EQUALS(uiDataRoot->getAppId(), "");
            TS_ASSERT_EQUALS(uiDataRoot->getChildCount(), 1);

            UISettings * settings = dynamic_cast<UISettings*>(uiDataRoot->getChild(0));
            TS_ASSERT_EQUALS(settings->getServerHost(), "localhost");
            TS_ASSERT_EQUALS(settings->getServerPort(), 20000);
            TS_ASSERT_EQUALS(settings->getServerCallbackPort(), 20001);

            TS_ASSERT(doc.getUISettings() == settings);
        }

    void test_saveLoad()
        {
            QString filename = "form.thekla.xml";
            QFile file(filename);

            XmlConfigFileDocument save;
            save.init();
            save.save(file);
            QDomDocument saveDoc = save.getDom();

            XmlConfigFileDocument load;
            load.init();
            load.load(file);
            QDomDocument loadDoc = load.getDom();

            TS_ASSERT_EQUALS(saveDoc.toString().toStdString(), loadDoc.toString().toStdString());

            QFile::remove(filename);
        }
    void test_RealCase1()
        {
            ////////// new form -> new config file from observable

            XmlConfigFileDocument doc;
            doc.init();

            QSlider qObj;
            Observable qObjObs(&qObj);

            XmlSubDocument subDoc = doc.createSubDocument(qObjObs);
            doc.addElement(subDoc.getRootElement());

            QString filename = "form.thekla.xml";
            QFile file(filename);

            doc.save(file);

            ////////// load form -> load configuration from file

            XmlConfigFileDocument doc2;
            doc2.init();
            doc2.load(file);
            doc2.getUIDataRoot()->setAppId("Form Window");

            XmlElement * elem = doc2.mergeSubDocument(qObjObs);
            TS_ASSERT(elem != NULL);

            qObj.setObjectName("Spaces allowed");
            qObj.setProperty("maximum", QVariant(999));

            elem->update();
            doc2.save(file);

            QFile::remove(filename);
        }
};
