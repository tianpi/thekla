// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/DatabaseConn.h>
#include <common/DatabaseEventDispatcher.h>
#include <xml/XmlDatabaseDocument.h>
#include <xml/UIDataRoot.h>
#include <xml/DBRootElement.h>
#include <xml/XmlConfigFileDocument.h>
#include <xml/OivField.h>
#include <xml/XmlElementData.h>

#include <QSlider>

class XmlDatabaseDocumentTest : public CxxTest::TestSuite
{
private:
    DatabaseConn * dbConn_;
public:
    void setUp()
        {
            dbConn_ = new DatabaseConn("localhost", 20000, 20001);
            DatabaseEventDispatcher::nonQtAppEventDispatcher->addTargetDbConn(dbConn_);
        }
    void tearDown()
        {
            DatabaseEventDispatcher::nonQtAppEventDispatcher->removeTargetDbConn(dbConn_);
            delete dbConn_;
        }

    void test_Ctor()
        {
            XmlDatabaseInputDocument doc(*dbConn_);

            TS_ASSERT_THROWS(doc.init("0815", "//THEKLA"), Exception);
            TS_ASSERT_THROWS(doc.init("0815", "/THEKLA/"), Exception);
            doc.init("0815", "/THEKLA");

            XmlElement * root = doc.getRootElement();
            TS_ASSERT(root != NULL);
            TS_ASSERT_EQUALS(root->getChildCount(), 1);

            UIDataRoot * uiDataRoot = dynamic_cast<UIDataRoot*>(root->getChild(0));
            TS_ASSERT(uiDataRoot != NULL);
            TS_ASSERT_EQUALS(uiDataRoot->getAppId(), "0815");

            TS_ASSERT(dbConn_->exists(*uiDataRoot));

            TS_ASSERT(doc.getUIDataRoot() == uiDataRoot);

        }

    void test_initFromConfigFileDoc()
        {

            XmlConfigFileDocument fileDoc;
            fileDoc.init();

            QSlider qObj;
            qObj.setObjectName("Dagobert");
            XmlSubDocument subDoc1 = fileDoc.createSubDocument(Observable(&qObj));
            QtObject * qtObj1 = dynamic_cast<QtObject*>(subDoc1.getRootElement());
            fileDoc.addElement(qtObj1);

            qtObj1->getSignals()[0]->getXmlData().setConfigFlag(XmlElementData::BIND, true);
            qtObj1->getSignals()[1]->getXmlData().setConfigFlag(XmlElementData::USE_AS_TRIGGER, true);
            qtObj1->getProperties()[0]->getXmlData().setConfigFlag(XmlElementData::UPDATE, true);

            UIDataRoot * uiDataRoot = dynamic_cast<UIDataRoot*>(fileDoc.getRootElement());
            uiDataRoot->setAppId("SonicYouth");


            XmlDatabaseInputDocument dbDoc(*dbConn_);
            dbDoc.init(fileDoc);

            UIDataRoot * uiDataRoot2 = dynamic_cast<UIDataRoot*>(dbDoc.getRootElement()->getChild(0));
            TS_ASSERT_EQUALS(uiDataRoot2->getAppId(), "SonicYouth");
            TS_ASSERT_EQUALS(uiDataRoot2->getChildCount(), 1);

            QtObject * qtObj2 = dynamic_cast<QtObject*>(uiDataRoot2->getChild(0));

//             TS_ASSERT_EQUALS(qtObj2->getSignals().size(), qtObj1->getSignals().size());
//             TS_ASSERT_EQUALS(qtObj2->getProperties().size(), qtObj1->getProperties().size());
            TS_ASSERT_EQUALS(qtObj2->getSignals().size(), 2);
            TS_ASSERT_EQUALS(qtObj2->getProperties().size(), 1);

        }

    void test_QtObject_bind()
        {
            // init document (NOTE: registerDefaultElements() is called within init())
            XmlDatabaseInputDocument doc(*dbConn_);
            doc.init("0815");
            doc.getDatabaseConn().insert(*(doc.getUIDataRoot()));

            // create QObject under test and specify object name (important!)
            QSlider * qObj = new QSlider;
            qObj->setObjectName("Dradiwaberl");
            Observable obsObj(qObj);

            // needed for the xpath in the "sync" variant
            XmlSubDocument subDoc0 = doc.createSubDocument(obsObj);
            XmlElement * elem = subDoc0.getRootElement();
            // add element to document
            doc.addElement(elem);

            // (!) Testing the DatabaseConn methods

            if (doc.getDatabaseConn().exists(*elem)) {
                TS_ASSERT_EQUALS(doc.getDatabaseConn().deleteFrom(*elem), 1);
                TS_ASSERT_EQUALS(doc.getDatabaseConn().exists(*elem), false);
            }

            TS_ASSERT_EQUALS(doc.getDatabaseConn().insert(*elem), true);
            TS_ASSERT_EQUALS(doc.getDatabaseConn().exists(*elem), true);

            TS_ASSERT_EQUALS(doc.getDatabaseConn().update(*elem), true);


            ////////// Get Qt objects

            QtObject * qtObj = dynamic_cast<QtObject*>(elem);
            TS_ASSERT(qtObj != NULL);
            if (qtObj == NULL)
                return;

            // picking one signal
            QtSignal * qtSignal = qtObj->getSignal("valueChanged(int)");
            TS_ASSERT(qtSignal != NULL);
            if (qtSignal == NULL)
                return;

            // picking one property
//            qtObj->getProperty("minimum")->getXmlData().setConfigFlag(XmlElementData::BIND, true);
            QtProperty * qtProp = qtObj->getProperty("maximum");
//            qtProp->getXmlData().setConfigFlag(XmlElementData::BIND, true);

//            qtSignal->getXmlData().setConfigFlag(XmlElementData::BIND, true);
//            qtSignal->getXmlData().setConfigFlag(XmlElementData::USE_AS_TRIGGER, true);
            qtSignal->bind();


            //////////  Update Observable

            qObj->setMinimum(100);
            qObj->setMaximum(999);
            qObj->setValue(666);

            // update Database
            doc.getDatabaseConn().update(*qtSignal);

            ////////// verify locally

            // the signal
            QByteArray a;
            QDataStream s(&a, QIODevice::WriteOnly);
            s << QVariant(666);
            QByteArray text = qtSignal->getXmlData().getText(true);
            TS_ASSERT_EQUALS(text, a);
            QVariant v = QtObjectUtils::fromStream(text)[0];
            int v0 = v.value<int>();
            TS_ASSERT_EQUALS(v0, 666);

            // the property
//             QByteArray a2;
//             QDataStream s2(&a2, QIODevice::WriteOnly);
//             s2 << QVariant(999);
//            TS_ASSERT_EQUALS(qtProp->getXmlData().getText(true), QtObjectUtils::toStream(QVariant((int)999)));

            ////////// verify database update

            // the signal
            XmlSubDocument subDoc2 = doc.createSubDocument(doc.getDatabaseConn().select(qtProp->getXPath()));
            QtProperty * qtProp2 = dynamic_cast<QtProperty*>(subDoc2.getRootElement());
            log4cplus::helpers::sleep(0, 100);
            TS_ASSERT_EQUALS(qtProp->getXmlData().getText(true), qtProp2->getXmlData().getText(true));
            delete qtProp2;

            // the property
            XmlSubDocument subDoc3 = doc.createSubDocument(doc.getDatabaseConn().select(qtSignal->getXPath()));
            XmlElement * qtSignal2 = subDoc3.getRootElement();
            TS_ASSERT_EQUALS(qtSignal->getXmlData().getText(true), qtSignal2->getXmlData().getText(true));
            delete qtSignal2;

            // the object
            XmlSubDocument subDoc4 = doc.createSubDocument(doc.getDatabaseConn().select(qtObj->getXPath()));
            XmlElement * qtObj2 = subDoc4.getRootElement();
            delete qtObj2;


            // remove the entire shit
            doc.getDatabaseConn().deleteFrom(*(doc.getRootElement()->getChild(0)));
        }

    void test_Watchdogs()
        {

            ////////// Prepare the document

            XmlDatabaseInputDocument doc(*dbConn_);
            doc.init("WatchdogTests");

            QSlider qObj;
            qObj.setObjectName("Asterix");
            Observable qObjObs(&qObj);

            XmlSubDocument subDoc = doc.createSubDocument(qObjObs);
            XmlElement * qtObj = subDoc.getRootElement();
            doc.addElement(qtObj);


            ////////// Performs tests on QtObject ["QSlider", "Asterix" ]

            doc.getDatabaseConn().insert(*(qtObj->getParent()));

            // (1) register watchdog
            qtObj->registerWatchdog();
            TS_ASSERT_EQUALS(qtObj->getWatchdogId(), DatabaseConn::MIN_WATCHDOG_ID);
            TS_ASSERT(qtObj->hasWatchdog());

            // (1a) called twice -> nothing happens
            qtObj->registerWatchdog();
            TS_ASSERT_EQUALS(qtObj->getWatchdogId(), DatabaseConn::MIN_WATCHDOG_ID);
            TS_ASSERT(qtObj->hasWatchdog());

            // (2) unregister watchdog
            qtObj->unregisterWatchdog();
            TS_ASSERT_EQUALS(qtObj->getWatchdogId(), DatabaseConn::MIN_WATCHDOG_ID - 1);
            TS_ASSERT(!qtObj->hasWatchdog());


            // (3) re-register watchdog
            qtObj->registerWatchdog();
            TS_ASSERT_EQUALS(qtObj->getWatchdogId(), DatabaseConn::MIN_WATCHDOG_ID + 1);
            TS_ASSERT(qtObj->hasWatchdog());


            // (4) change xpath
            QString xPath1 = qtObj->getXPath();
            qObj.setObjectName("Obelix");
            qtObj->update();
            QString xPath2 = qtObj->getXPath();
            TS_ASSERT_DIFFERS(xPath1.toStdString(), xPath2.toStdString());

            // (4a) re-register watchdog -> unregister -> register
            // WARNING (-> Daniel): registering watchdog on non-existant element possible (!)
//             qtObj->registerWatchdog();
//             TS_ASSERT_EQUALS(qtObj->getWatchdogId(), DatabaseConn::MIN_WATCHDOG_ID + 2);
//             TS_ASSERT(qtObj->hasWatchdog());

            // (4b) update database; NOTE: does not work because the XPath of the DB and
            // the XmlElement does not match -> switch to insert
//             doc.getDatabaseConn().insert(*qtObj);

        }

    QtProperty* initDbDoc(XmlDatabaseDocument * doc)
        {
            doc->init("AsyncWatchdogTest");

            QSlider * qObj = new QSlider;
            qObj->setObjectName("Obelix");
            Observable qObjObs(qObj);

            XmlSubDocument subDoc = doc->createSubDocument(qObjObs);
            XmlElement * qtObj = subDoc.getRootElement();
            doc->addElement(qtObj);

            QtProperty * qtProp = (dynamic_cast<QtObject*>(qtObj))->getProperty("maximum");
            return qtProp;
        }
    void test_asyncWatchdogReply()
        {
            DatabaseConn dbConn1("localhost", 20000, 20001);
            DatabaseEventDispatcher::nonQtAppEventDispatcher->addTargetDbConn(&dbConn1);
            DatabaseConn dbConn2("localhost", 20000, 20001);
            DatabaseEventDispatcher::nonQtAppEventDispatcher->addTargetDbConn(&dbConn2);

            XmlDatabaseInputDocument docIn(dbConn1);
            XmlDatabaseOutputDocument docOut(dbConn2);

            QtProperty * qtPropIn = initDbDoc(&docIn);
            QtProperty * qtPropOut = initDbDoc(&docOut);
//            docIn.getDatabaseConn().insert(*(qtPropIn->getParent()));

            // (1a) register Watchdog on -1-
            qtPropOut->registerWatchdog();

            // (1b) change TEXT of property -2-
            QVariant qVar;
            for (int i = 111; i <= 888; i += 111) {
                qVar = QVariant((int)i);
                qtPropIn->getXmlData().setText(QtObjectUtils::toStream(qVar), true);
                // (1c) update DB of -2-
                docIn.getDatabaseConn().update(*qtPropIn);
            }


            // TEST: Owner of watchdog must have been updated
            log4cplus::helpers::sleep(0, 200);
            TS_ASSERT_EQUALS(qtPropOut->getXmlData().getText(true),
                             qtPropIn->getXmlData().getText(true));

            DatabaseEventDispatcher::nonQtAppEventDispatcher->removeTargetDbConn(&dbConn1);
            DatabaseEventDispatcher::nonQtAppEventDispatcher->removeTargetDbConn(&dbConn2);
        }

//     void test_createSubDocument_OutputDoc()
//         {
//             XmlDatabaseInputDocument docIn(*dbConn_);
//             docIn.init("createSubDocumentTest");
//             XmlDatabaseOutputDocument docOut(*dbConn_);
//             docOut.init("createSubDocumentTest");

//             QSlider qObj;
//             qObj.setObjectName("Miraculix");
//             Observable qObjObs(&qObj);

//             XmlSubDocument * subDocIn = docIn.createSubDocument(qObjObs);
//             docIn.addElement(subDocIn->getRootElement());
//             delete subDocIn;

//             QtObject * qtObjTmp = QtObject::create(qObj.metaObject()->className(), qObj.objectName());
//             XmlSubDocument * subDocOut = docOut.createSubDocument("QtObject", qtObjTmp->getXmlData());
//             delete qtObjTmp;

//             XmlElement * qtObj = subDocOut->getRootElement();
//             delete subDocOut;

//             TS_ASSERT(qtObj != NULL);
//             if (qtObj == NULL) return;

//         }

    void test_OivField()
        {
            // Document
            XmlDatabaseInputDocument docIn(*dbConn_);
            docIn.init("OivFieldTest");

            // Observable
            SoSFInt32 field;
            field.setValue(111);

            // OivElement
            XmlElement * oivElem = docIn.getElement("OivField");
            TS_ASSERT(oivElem != NULL);
            if (oivElem == NULL) return;
            oivElem->init(new Observable(&field));
            oivElem->getXmlData().setAttribute("name", "Automatix"); // FRIENDLYerweise do NOT forget to do this (!)

            // OivField: add to Document (-> to database)
            docIn.addElement(oivElem);

            // bind to Observable
            oivElem->bind();

            // Observable: another value
            field.setValue(555);
            // OivField: update (-> database)
            oivElem->update();

            // OivField: retrieve from database
            XmlSubDocument subDoc = docIn.createSubDocument(docIn.getDatabaseConn().select(oivElem->getXPath()));
            XmlElement * oivElemCmp = subDoc.getRootElement();
            TS_ASSERT(oivElemCmp != NULL);
            if (oivElemCmp == NULL) return;

            // COMPARE latest value with current database represenation value
            TS_ASSERT_EQUALS(oivElemCmp->getXmlData().getText(), "555");
        }

};
