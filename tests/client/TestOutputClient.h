// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <client/Client.h>

#include <QSlider>

#include <Inventor/fields/SoFields.h>

class OutputClientTest : public CxxTest::TestSuite
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

            OutputClient first(Client::NON_QT_APP);
            TS_ASSERT(!first.isInitialized());
            first.init("Beirut", "localhost", 20000, 20001);
            TS_ASSERT(first.isInitialized());

            //// test database document

            XmlDatabaseOutputDocument * docOut = first.getDatabaseDoc();
            TS_ASSERT(docOut != NULL);
            if (docOut == NULL)  return;

            XmlElement * uiDataRoot = docOut->getUIDataRoot();
            TS_ASSERT(uiDataRoot != NULL);
            if (uiDataRoot == NULL)
                return;
            TS_ASSERT(docOut->getDatabaseConn().exists(*uiDataRoot));

        }

    void test_select()
        {

            OutputClient client(Client::NON_QT_APP);
            client.init("Beirut", "localhost", 20000, 20001);

            //// QtObject

            QtObject * qtObj1 = client.selectQtObject("QSlider", "Asterix");
            TS_ASSERT(qtObj1 != NULL);
            QtObject * qtObj2 = client.selectQtObject("QSlider", "HellIsRoundTheCorner");
            TS_ASSERT(qtObj2 != NULL);

            //// QtSignal

            QtSignal * qtSignal11 = client.selectQtSignal(qtObj1, "valueChanged(int)");
            TS_ASSERT(qtSignal11 != NULL);
            QtSignal * qtSignal21 = client.selectQtSignal(qtObj2, "valueChanged(int)");
            TS_ASSERT(qtSignal21 != NULL);

            TS_ASSERT_EQUALS(qtObj1->getSignals().size(), 1);
            TS_ASSERT_EQUALS(qtObj2->getSignals().size(), 1);


            QtSignal * qtSignal12 = client.selectQtSignal(qtObj1, "WhereIShelter");
            TS_ASSERT(qtSignal12 != NULL);
            QtSignal * qtSignal22 = client.selectQtSignal(qtObj2, "WhereIShelter");
            TS_ASSERT(qtSignal22 != NULL);


            TS_ASSERT_EQUALS(qtObj1->getSignals().size(), 2);
            TS_ASSERT_EQUALS(qtObj2->getSignals().size(), 2);

            //// QtProperty

            QtProperty * qtProperty11 = client.selectQtProperty(qtObj1, "minimum", "int");
            TS_ASSERT(qtProperty11 != NULL);
            QtProperty * qtProperty21 = client.selectQtProperty(qtObj2, "minimum", "int");
            TS_ASSERT(qtProperty21 != NULL);

            TS_ASSERT_EQUALS(qtObj1->getProperties().size(), 1);
            TS_ASSERT_EQUALS(qtObj2->getProperties().size(), 1);


            QtProperty * qtProperty12 = client.selectQtProperty(qtObj1, "IsmsAndSkisms", "int");
            TS_ASSERT(qtProperty12 != NULL);
            QtProperty * qtProperty22 = client.selectQtProperty(qtObj2, "IsmsAndSkisms", "int");
            TS_ASSERT(qtProperty22 != NULL);

            TS_ASSERT_EQUALS(qtObj1->getProperties().size(), 2);
            TS_ASSERT_EQUALS(qtObj2->getProperties().size(), 2);


            //// OivField

            OivField * oivField1 = client.selectOivField("SFInt32", "integerValue");
            TS_ASSERT(oivField1 != NULL);
        }

    void test_connect()
        {

            OutputClient client(Client::NON_QT_APP);
            client.init("Beirut", "localhost", 20000, 20001);

            //// Signal -> Slot

            QSlider * qObj = new QSlider;

            QtObject * qtObject1 = client.selectQtObject("QSlider", "Asterix");
            QtSignal * qtSignal1 = client.selectQtSignal(qtObject1, "valueChanged(int)");

            client.connectQtSignalToQtSlot(qtSignal1,
                                           qObj, "setValue(int)");

            TS_ASSERT(qtSignal1 != NULL);
            if (qtSignal1 == NULL) return;
            TS_ASSERT_EQUALS(qtSignal1->getObservers().size(), 1);


            QByteArray a = qtSignal1->getXmlData().getText(true);
            QDataStream s(&a, QIODevice::ReadOnly);
            QVariant v;
            s >> v;
            int i;
            i = v.value<int>();
            TS_ASSERT_EQUALS(i, 456);

//             log4cplus::helpers::sleep(0, 500);
//             qApp->processEvents();
//            TS_ASSERT_EQUALS(qObj->value(), 456);

            //// Signal -> Slot :: element does not exist in DB


            QtObject * qtObject2 = client.selectQtObject("QSlider", "Verleihnix");
            QtSignal * qtSignal2 = client.selectQtSignal(qtObject2, "valueChanged(int)");
            client.connectQtSignalToQtSlot(qtSignal2,
                                           qObj, "setValue(int)");

            TS_ASSERT(qtSignal2 != NULL);
            if (qtSignal2 == NULL) return;


            //// Property -> Property :: good case

            QtProperty * qtProp1 = client.selectQtProperty(qtObject1, "maximum", "int");
            client.connectQtPropertyToQtProperty(qtProp1, qObj, "maximum");

            TS_ASSERT(qtProp1 != NULL);
            if (qtProp1 == NULL) return;
            TS_ASSERT_EQUALS(qtProp1->getObservers().size(), 1);

            TS_ASSERT_EQUALS(qtProp1->getXmlData().getText(true), QtObjectUtils::toStream(QVariant(789)));

            qtProp1->sync();
            TS_ASSERT_EQUALS(qObj->maximum(), 789);

            //// Property -> Property :: element does not exist in DB

            QtProperty * qtProp2 = client.selectQtProperty(qtObject1, "DieSpaceTasteIstImZentrum", "xxx");
            TS_ASSERT_THROWS(client.connectQtPropertyToQtProperty(qtProp2, qObj, "minimum"), Exception);
            TS_ASSERT_EQUALS(qtProp2->getObservers().size(), 0);


            //// OivField -> Property :: good case

            OivField * oivField1 = client.selectOivField("SFInt32", "integerValue");
            client.connectOivFieldToQtProperty(oivField1,
                                               qObj, "minimum");
            TS_ASSERT_EQUALS(oivField1->getObservers().size(), 1);

            //// OivField -> Property :: element does not exist in DB

            OivField * oivField2 = client.selectOivField("SFInt32", "Dagobert");
            client.connectOivFieldToQtProperty(oivField2,
                                               qObj, "maximum");
            TS_ASSERT_EQUALS(oivField2->getObservers().size(), 1);

            //// OivField -> OivField

            SoSFInt32 field2;
            client.connectOivFieldToOivField(oivField1, &field2);
            TS_ASSERT_EQUALS(oivField1->getObservers().size(), 2);



            //// Property -> SoField

            SoSFInt32 field;
            client.connectQtPropertyToOivField(qtProp1, &field);
            TS_ASSERT_EQUALS(qtProp1->getObservers().size(), 2);



            //// Signal -> SoFieldList

            SoFieldList fieldList1;
            SoSFInt32 i1, i2;
            fieldList1.append(&i1);
            fieldList1.append(&i2);

            QtSignal * qtSignal3 = client.selectQtSignal(qtObject1, "rangeChanged(int,int)");
            client.connectQtSignalToOivFieldList(qtSignal3, &fieldList1);
            TS_ASSERT_EQUALS(qtSignal3->getObservers().size(), 1);



        }

};
