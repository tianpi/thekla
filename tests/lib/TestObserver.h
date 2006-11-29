// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/Observer.h>
#include <common/Observable.h>
#include <common/QtObjectUtils.h>

class ObserverTest : public CxxTest::TestSuite
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

            QSlider obj;
            const QMetaObject * meta = obj.metaObject();

            // Construct

            Observer o0;
            TS_ASSERT_EQUALS(o0.type(), Observer::INVALID);

            Observer o1(&obj);
            TS_ASSERT_EQUALS(o1.type(), Observer::QOBJECT);

            QMetaMethod slot = meta->method(meta->indexOfSlot("setValue(int)"));
            Observer o2(&obj, slot);
            TS_ASSERT_EQUALS(o2.type(), Observer::QOBJECTSLOT);

            QMetaProperty prop = meta->property(0);
            Observer o3(&obj, prop);
            TS_ASSERT_EQUALS(o3.type(), Observer::QOBJECTPROPERTY);

            SoSFInt32 oivField;
            Observer o4(&oivField);
            TS_ASSERT_EQUALS(o4.type(), Observer::SOFIELD);

            SoFieldList oivFieldList;
            oivFieldList.append(&oivField);
            Observer o5(&oivFieldList);
            TS_ASSERT_EQUALS(o5.type(), Observer::SOFIELDLIST);


            // getObserver

            QObject * objRet;
            QMetaMethod * slotRet;
            o1.getObserver(objRet, slotRet);
            TS_ASSERT(objRet == NULL);
            TS_ASSERT(slotRet == NULL);

            o2.getObserver(objRet, slotRet);
            TS_ASSERT_EQUALS(&obj, objRet);
//            TS_ASSERT_EQUALS(&slot, slotRet);

            QMetaProperty * propRet;
            o3.getObserver(objRet, propRet);
            TS_ASSERT_EQUALS(&obj, objRet);
//            TS_ASSERT_EQUALS(&prop, propRet);

            SoField * oivFieldRet;
            o4.getObserver(oivFieldRet);
            TS_ASSERT_EQUALS(&oivField, oivFieldRet);

            SoFieldList * oivFieldListRet;
            o5.getObserver(oivFieldListRet);
            TS_ASSERT_EQUALS(&oivFieldList, oivFieldListRet);


            // operator ==

            // NOTE: actual parameter eg QObject using implicitely Observer(QObject *) to
            // fullfill the operator==(const Observer&) interface (!)

            TS_ASSERT(o2 == o2);
            TS_ASSERT(!(o2 == o3));

            TS_ASSERT(o2 == Observer(&obj, slot));
            TS_ASSERT(o3 == Observer(&obj, prop));

            TS_ASSERT(o4 == &oivField);
            TS_ASSERT(o5 == &oivFieldList);

        }

    void test_isConnectable()
        {

            ////////// (II)  QSlider

            QList<QVariant::Type> vList;
            QVariant::Type v1 = QVariant::Int;
            vList.push_back(v1);

            QSlider obj;
            const QMetaObject * meta = obj.metaObject();

            Observer o1(&obj);
            TS_ASSERT(o1.isConnectable());

            QMetaMethod slot = meta->method(meta->indexOfSlot("setValue(int)"));
            Observer o2(&obj, slot);
            TS_ASSERT(o2.isConnectable());
            TS_ASSERT(o2.isConnectable(v1));
            TS_ASSERT(o2.isConnectable(vList));

            QMetaProperty prop = meta->property(meta->indexOfProperty("minimum"));
            Observer o3(&obj, prop);
            TS_ASSERT(o3.isConnectable());
            TS_ASSERT(o3.isConnectable(v1));
            TS_ASSERT(!o3.isConnectable(vList));

            SoSFInt32 oivField;
            Observer o4(&oivField);
            TS_ASSERT(o4.isConnectable());
            TS_ASSERT(o4.isConnectable(v1));
            TS_ASSERT(!o4.isConnectable(vList));

            SoFieldList oivFieldList;
            oivFieldList.append(&oivField);
            Observer o5(&oivFieldList);
            TS_ASSERT(o5.isConnectable());
            TS_ASSERT(o5.isConnectable(v1));
            TS_ASSERT(o5.isConnectable(vList));


            ////////// (II)  QPushButton

            QPushButton obj2;
            const QMetaObject * meta2 = obj2.metaObject();

            QList<QVariant::Type> vList2;
            QVariant::Type v21 = QVariant::Size;
            vList2.push_back(v21);


            QMetaMethod slot2 = meta2->method(meta2->indexOfSlot(QMetaObject::normalizedSignature("setIconSize(const QSize &)")));
            Observer o22(&obj, slot2);
            TS_ASSERT(!o22.isConnectable(v1));
            TS_ASSERT(!o22.isConnectable(vList));

            // positive example (with QSize!)
            TS_ASSERT(o22.isConnectable(v21));
            TS_ASSERT(o22.isConnectable(vList2));

            // .. and again negative: no conversion from qt to oiv possible!
            TS_ASSERT(!o4.isConnectable(v21));
            TS_ASSERT(!o4.isConnectable(vList2));

            TS_ASSERT(!o5.isConnectable(v21));
            TS_ASSERT(!o5.isConnectable(vList2));


            ////////// (III) SoSFInt32


//            QList<SoType> oivTypeList;
            SoType t1 = SoSFInt32::getClassTypeId();
//            oivTypeList.push_back(t1);


            // ok with field (SoSFInt32)
            TS_ASSERT(o4.isConnectable(t1));
            // nok with field (SoSFString)
            SoMFString oivField2;
            Observer o42(&oivField2);
            TS_ASSERT(!o42.isConnectable(t1));

            // nok with field list (in any case)
            TS_ASSERT(!o5.isConnectable(t1));

            // ok with property (int)
            TS_ASSERT(o3.isConnectable(t1));
            // nok with property (String)

            int index = meta->indexOfProperty("objectName");
            TS_ASSERT(index >= 0);
            QMetaProperty prop2 = meta->property(index);
            Observer o32(&obj2, prop2);
            TS_ASSERT(o32.isConnectable(t1));

            // not with slot (in any case)
            TS_ASSERT(!o2.isConnectable(t1));


            ////////// (III) SoFieldList (int,int)

            {
                SoFieldList fieldList;
                SoSFInt32 i1, i2;
                fieldList.append(&i1);
                fieldList.append(&i2);

                Observer oX(&fieldList);
                QList<QVariant::Type> typeListX;
                typeListX.push_back(QVariant::Int);
                typeListX.push_back(QVariant::Int);

                TS_ASSERT(oX.isConnectable(typeListX));
            }


            ////////// SoX -> QString

            {
                SoType t1 = SoSFVec3f::getClassTypeId();

                QLineEdit qLineEdit;
                QMetaProperty qProp = QtObjectUtils::getMetaProperty(qLineEdit, "text");
                Observer oX(&qLineEdit, qProp);
                TS_ASSERT(oX.isConnectable(t1));



            }

        }

    void test_CopyCtor_Observable()
        {
            QSlider obj;
            const QMetaObject * meta = obj.metaObject();
            QMetaProperty prop = meta->property(meta->indexOfProperty("minimum"));

            // Construct Observer from Observable

            Observable observable(&obj, prop);
            Observer o1(observable);
            TS_ASSERT_EQUALS(o1.type(), Observer::QOBJECTPROPERTY);
        }

};
