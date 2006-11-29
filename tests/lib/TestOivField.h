// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <xml/OivField.h>
#include <common/Observable.h>

class OivFieldTest : public CxxTest::TestSuite
{
private:
    class TestOivField : public OivField
    {
    public:
        TestOivField()
            : OivField()
            {};
        TestOivField(const TestOivField & copy)
            : OivField(copy)
            {};
        ~TestOivField()
            {};
    public:
        virtual XmlElement * clone() const { return new TestOivField(*this); };
    };

public:
    void setUp()
        {
        }
    void tearDown()
        {
        }

    void test_Ctor()
        {
            TestOivField * oivObj = new TestOivField;
            TS_ASSERT_EQUALS(oivObj->getName(), "");
            TS_ASSERT_EQUALS(oivObj->getType(), "");

            oivObj->setName("Hilde");
            TS_ASSERT_EQUALS(oivObj->getName(), "Hilde");

            delete oivObj;
        }
    void test_canInit()
        {
            TestOivField * oivObj = new TestOivField;

            TS_ASSERT(oivObj->canInit(Observable::SOFIELD));

            delete oivObj;
        }
    void test_initObservable()
        {
            TestOivField * oivObj = new TestOivField;

            TS_ASSERT(oivObj->canInit(Observable::SOFIELD));

            SoSFInt32 * field = new SoSFInt32;
            Observable * obs = new Observable(field);
            oivObj->init(obs);
            oivObj->setName("Hilde");

            TS_ASSERT_EQUALS(oivObj->getName(), "Hilde");
            TS_ASSERT_EQUALS(oivObj->getType().toStdString(), "SFInt32");

            TS_ASSERT_EQUALS(oivObj->getChildCount(), 0);

            // NOTE: see TestXmlDocument (!): need an XmlDocument to access the
            // AdapterFactory -> create an QtAdapter without bind() is not possible
//            TS_ASSERT(oivObj->canBind());

            delete oivObj;
        }

    void test_update()
        {
            TestOivField oivObj;

            SoSFInt32 field;
            oivObj.init(new Observable(&field));

            // Observable changes value and Element updates
            field.setValue(888);
            oivObj.update();

            // Check the element text
            SbString text;
            field.get(text);
            QByteArray textCmp = oivObj.getXmlData().getText();
            TS_ASSERT_EQUALS(std::string(textCmp.data(), textCmp.size()), std::string(text.getString()));
            TS_ASSERT_EQUALS(oivObj.getXmlData().getText(), text.getString());

        }

    void test_merge()
        {
            TestOivField oivObj;

            // Observable: init
            SoSFInt32 field;
            field.setValue(666);

            // Element: init
            oivObj.init(new Observable(&field));

            // Create another Element from Xml representation
            TestOivField * oivObjFromXml = new TestOivField;
            ((XmlElement*)oivObjFromXml)->init(oivObj.getDom().documentElement().cloneNode(true).toElement());

            // before ..
            TS_ASSERT_EQUALS(oivObjFromXml->getXmlData().getText(), "666");

            // Observable: change value
            field.setValue(777);
            oivObjFromXml->merge(new Observable(&field));

            // and after "merge"
            TS_ASSERT_EQUALS(oivObjFromXml->getXmlData().getText(), "777");
         }

    void test_bind()
        {
            TestOivField oivField;
            SoSFInt32 field;
            Observable fieldObs(&field);

            // Observable: set initial value
            field.setValue(111);

            // Element: init
            oivField.init(new Observable(fieldObs));
            TS_ASSERT_EQUALS(oivField.getXmlData().getText(), "111");

            // Bind to the Observable
            oivField.bind();

            // Check the (initialized) FieldSensor
            SoFieldSensor * sensor = oivField.getFieldSensor();
            TS_ASSERT(sensor != NULL);
            if (sensor == NULL) return;

            // Observable: set other value
            field.setValue(222);

            // NOTE: For NOW call the sensor callback function MANUALLY !
            sensor->trigger();

            // Element: got the new value (!)
            TS_ASSERT_EQUALS(oivField.getXmlData().getText(), "222");

        }

    void test_connect()
        {

            // Observable element
            TestOivField oivField;
            SoSFInt32 fieldFrom;
            Observable from(&fieldFrom);
            oivField.init(new Observable(from));
            // Observer
            SoSFInt32 fieldTo;
            Observer to(&fieldTo);

            // Connect Element with Observer
            oivField.connect(new Observer(to));
            TS_ASSERT_EQUALS(oivField.getObservers().size(), 1);

            // Observable: set another value and (manually) update
            fieldFrom.setValue(123);
            oivField.update();

            // Sync Element with Observers
            oivField.sync();

            // Observer: check value (equals)
            TS_ASSERT_EQUALS(fieldTo.getValue(), 123);
        }

    void test_syncObservable()
        {
            // Observable element
            TestOivField oivField;
            SoSFInt32 field;
            oivField.init(new Observable(&field));

            // set observable initial value
            field.setValue(111);
            TS_ASSERT_EQUALS(field.getValue(), 111);

            // set element value
            oivField.getXmlData().setText("222");
            // SYNC OBSERVABLE
            oivField.syncObservable();
            // test updated value
            TS_ASSERT_EQUALS(field.getValue(), 222);

        }
};
