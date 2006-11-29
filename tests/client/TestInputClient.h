// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <client/Client.h>
#include <common/QtObjectUtils.h>
#include <common/Base64.h>

#include <QSlider>
#include <QMainWindow>

#include <Inventor/fields/SoFields.h>

class InputClientTest : public CxxTest::TestSuite
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
            //// construct and init

            InputClient first(Client::NON_QT_APP);
            TS_ASSERT(!first.isInitialized());
            first.init("Beirut", "localhost", 20000, 20001);
            TS_ASSERT(first.isInitialized());

            //// test database document

            XmlDatabaseInputDocument * docIn = first.getDatabaseDoc();
            TS_ASSERT(docIn != NULL);
            if (docIn == NULL)  return;

            XmlElement * uiDataRoot = docIn->getUIDataRoot();
            TS_ASSERT(uiDataRoot != NULL);
            if (uiDataRoot == NULL)
                return;
            TS_ASSERT(docIn->getDatabaseConn().exists(*uiDataRoot));

        }

    void test_QtObject()
        {

            InputClient client(Client::NON_QT_APP);
            client.init("Beirut", "localhost", 20000, 20001);

            QSlider * qObj = new QSlider;
            qObj->setObjectName("Asterix");

            //// create objects and bindings


            // create QtObject
            QtObject * qtObj = client.insertQtObject(qObj);
            TS_ASSERT(qtObj != NULL);
            if (qtObj == NULL) return;
            TS_ASSERT_EQUALS(qtObj->getObjectName(), "Asterix");

            // bind Signal
            QtSignal * qtSignal = client.bindQtSignal(qtObj, "valueChanged(int)");
            TS_ASSERT(qtSignal != NULL);
            if (qtSignal == NULL) return;

            // bind Property
            QtProperty * qtProp = client.bindQtProperty(qtObj, "maximum");
            TS_ASSERT(qtProp != NULL);
            if (qtProp == NULL) return;


            //// update Observable

            QVariant qVarMin((int)123);
            QVariant qVarVal((int)456);
            QVariant qVarMax((int)789);

            // first update properties
            qObj->setProperty("maximum", qVarMax);
            qObj->setProperty("minimum", qVarMin);

            // then update "signal" -> via the trigger slot both elements (signal +
            // property) should be updated
            qObj->setValue(456);


            //// verify the values within the database

            XmlDatabaseInputDocument * doc = client.getDatabaseDoc();

            // the object
            XmlSubDocument subDoc4 = doc->createSubDocument(doc->getDatabaseConn().select(qtObj->getXPath()));
            XmlElement * qtObj2 = subDoc4.getRootElement();
            delete qtObj2;


            // the property
            XmlSubDocument subDoc2 = doc->createSubDocument(doc->getDatabaseConn().select(qtProp->getXPath()));
            QtProperty * qtProp2 = dynamic_cast<QtProperty*>(subDoc2.getRootElement());
            TS_ASSERT_EQUALS(qtProp2->getXmlData().getText(true), QtObjectUtils::toStream(qVarMax));
            delete qtProp2;

            // the signal
            XmlSubDocument subDoc3 = doc->createSubDocument(doc->getDatabaseConn().select(qtSignal->getXPath()));
            XmlElement * qtSignal2 = subDoc3.getRootElement();

            QByteArray a;
            QDataStream s(&a, QIODevice::WriteOnly);
            s << QVariant(456);

            TS_ASSERT_EQUALS(qtSignal->getXmlData().getText(true), a);
            TS_ASSERT_EQUALS(qtSignal2->getXmlData().getText(true), a);
            delete qtSignal2;


            //// add an OivField with appid "Beirut"

            SoSFInt32 field;
            field.setValue(111);
            OivField * oivField = client.insertOivField(&field, "integerValue");
            TS_ASSERT(oivField != NULL);

            OivField * oivField2 = client.insertOivField(&field, "integerValue");
            TS_ASSERT(oivField2 == oivField);

        }

    void test_OivField()
        {
            InputClient client(Client::NON_QT_APP);
            client.init("AphexTwin", "localhost", 20000, 20001);

            SoSFInt32 field;
            field.setValue(111);

            OivField * oivField = client.insertOivField(&field, "Automatix");
            TS_ASSERT(oivField != NULL);
            if (oivField == NULL) return;
            TS_ASSERT_EQUALS(oivField->getXmlData().getText(), "111");

            oivField->bind();

            field.setValue(222);

            oivField->getFieldSensor()->trigger();
        }
    void test_Exception()
        {
            InputClient client(Client::NON_QT_APP);
            client.init("AphexTwin2", "localhost", 20000, 20001);

            TS_ASSERT_THROWS(THROW_THEKLA_EXCEPTION("This is an Exception"), Exception);
        }

    void test_RecoverFromDb()
        {
            // open input connection
            InputClient client(Client::NON_QT_APP);
            client.init("Beirut", "localhost", 20000, 20001);

            // already inserted QtObject (see above): latest known max == 789
            QSlider * qObj = new QSlider;
            qObj->setObjectName("Asterix");
            qObj->setMaximum(111);
            TS_ASSERT_EQUALS(qObj->maximum(), 111);

            // add to DOM (but recover from DB!!)
            client.insertQtObject(qObj, true);
            TS_ASSERT_EQUALS(qObj->maximum(), 789);

            InputClient client2(Client::NON_QT_APP);
            client2.init("AphexTwin", "localhost", 20000, 20001);

            SoSFInt32 field;
            field.setValue(111);
            TS_ASSERT_EQUALS(field.getValue(), 111);

            // add to DOM (but recover from DB!!)
            client2.insertOivField(&field, "Automatix", true);
            TS_ASSERT_EQUALS(field.getValue(), 222);

        }

    void test_ConfigFileItself()
        {
            QFile file("form-thekla.xml");

            XmlConfigFileDocument docFile;
            docFile.init();
            docFile.load(file);

            // verify value of "maximum" property within the config file

            QtObject * qtObject = dynamic_cast<QtObject*>(docFile.findFirstElement("QtObject"));
            QtProperty * qtPropMax = qtObject->getProperty("maximum");
            TS_ASSERT_EQUALS(qtPropMax->getXmlData().getText(true), QtObjectUtils::toStream(QVariant((int)789)));

        }


    void test_InitFromFile()
        {
            QFile file("form-thekla.xml");
            InputClient client(Client::NON_QT_APP);

            QMainWindow mainWindow;
            mainWindow.setObjectName("Entenhausen");

            QSlider qSlider(&mainWindow);

            qSlider.setMaximum(987);
            qSlider.setObjectName("Dagobert");

            client.init(file, &mainWindow);

            //// paranoid document testing


            XmlDatabaseInputDocument * docIn = client.getDatabaseDoc();
            XmlElement * rootElem = docIn->getRootElement();
            TS_ASSERT(rootElem != NULL);
            if (rootElem == NULL) return;
            UIDataRoot * uiDataRoot = docIn->getUIDataRoot();
            TS_ASSERT(uiDataRoot != NULL);
            if (uiDataRoot == NULL) return;
            TS_ASSERT_EQUALS(uiDataRoot->getChildCount(), 1);

            //// check the (single) QtObject

            QtObject * qtObject = dynamic_cast<QtObject*>(docIn->findFirstElement("QtObject"));
            TS_ASSERT(qtObject != NULL);
            if (qtObject == NULL) return;

            //// check maximum property: must be equal to the from the QObject maximum (as set above)

            // synchronized are: (1) QObject (2) local XML representation (3) DB XML representation

            // (1)
            TS_ASSERT_EQUALS(qSlider.maximum(), 987);
            // (2): NOTE: "maximum" property value was differnt within the config file
            // originally (see test above)
            QtProperty * qtPropMax = qtObject->getProperty("maximum");
            TS_ASSERT_EQUALS(qtPropMax->getXmlData().getText(true), QtObjectUtils::toStream(QVariant((int)987)));
            // (3)
            QDomElement qtPropMaxDbElem = docIn->getDatabaseConn().select(qtPropMax->getXPath()).documentElement();
            TS_ASSERT_EQUALS(QtObjectUtils::fromStream(Base64::decode(qtPropMaxDbElem.text().toAscii())).at(0),
                             QVariant((int)987));

            // check signal and properties (all non-configured have been filtered)
            TS_ASSERT_EQUALS(qtObject->getSignals().size(), 3);
            TS_ASSERT_EQUALS(qtObject->getProperties().size(), 2);

            //// create and bind signal (currently NOT within DOM)

            QtSignal * qtSignal = client.bindQtSignal(qtObject, "rangeChanged(int,int)");
            TS_ASSERT(qtSignal != NULL);
            if (qtSignal == NULL) return;
            TS_ASSERT(docIn->getDatabaseConn().exists(*qtSignal));

            TS_ASSERT_THROWS(client.bindQtSignal(qtObject, "XXX"), Exception);

            //// create and bind property (currently NOT within DOM)

            QtProperty * qtProperty = client.bindQtProperty(qtObject, "value");
            TS_ASSERT(qtProperty != NULL);
            if (qtProperty == NULL) return;
            TS_ASSERT(docIn->getDatabaseConn().exists(*qtProperty));

            TS_ASSERT_THROWS(client.bindQtProperty(qtObject, "XXX"), Exception);

        }

        void test_InitFromFile_RecoverFromDb()
        {
            QFile file("form-thekla.xml");
            InputClient client(Client::NON_QT_APP);

            QMainWindow mainWindow;
            mainWindow.setObjectName("Entenhausen");

            QSlider qSlider(&mainWindow);
            qSlider.setObjectName("Dagobert");
            qSlider.setMaximum(111);

            client.init(file, &mainWindow, true);

            //// get the (single) QtObject

            XmlDatabaseInputDocument * docIn = client.getDatabaseDoc();
            QtObject * qtObject = dynamic_cast<QtObject*>(docIn->findFirstElement("QtObject"));

            //// check maximum property: must be equal to the from the ConfigFile (recover from db! -> see above)

            // synchronized are: (1) QObject (2) local XML representation (3) DB XML representation

            // (1)
            TS_ASSERT_EQUALS(qSlider.maximum(), 987);
            QtProperty * qtPropMax = qtObject->getProperty("maximum");
            // (2)
            TS_ASSERT_EQUALS(qtPropMax->getXmlData().getText(true), QtObjectUtils::toStream(QVariant((int)987)));
            // (3)
            QDomElement qtPropMaxDbElem = docIn->getDatabaseConn().select(qtPropMax->getXPath()).documentElement();
            TS_ASSERT_EQUALS(QtObjectUtils::fromStream(Base64::decode(qtPropMaxDbElem.text().toAscii())).at(0),
                             QVariant((int)987));

        }


};
