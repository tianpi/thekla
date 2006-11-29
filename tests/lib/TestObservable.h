// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/Observable.h>
#include <common/Observer.h>

#include <QSlider>
#include <QLineEdit>
#include <QPushButton>
#include <Inventor/fields/SoFields.h>

class ObservableTest : public CxxTest::TestSuite
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
            // Construct

            Observable o1;
            TS_ASSERT_EQUALS(o1.type(), Observable::INVALID);

            QObject obj;
            Observable o2(&obj);
            TS_ASSERT_EQUALS(o2.type(), Observable::QOBJECT);

            const QMetaObject * meta = obj.metaObject();
            QMetaMethod method = meta->method(0);
            Observable o4(&obj, method);
            TS_ASSERT_EQUALS(o4.type(), Observable::QOBJECTSIGNAL);

            QMetaProperty prop = meta->property(0);
            Observable o5(&obj, prop);
            TS_ASSERT_EQUALS(o5.type(), Observable::QOBJECTPROPERTY);

            // getObservable

            QObject * objRet;
            o1.getObservable(objRet);
            TS_ASSERT(objRet == NULL);

            o2.getObservable(objRet);
            TS_ASSERT_EQUALS(&obj, objRet);

            QMetaMethod * methodRet;
            o4.getObservable(objRet, methodRet);
            TS_ASSERT_EQUALS(&obj, objRet);
//            TS_ASSERT_EQUALS(&method, methodRet);

            QMetaProperty * propRet;
            o5.getObservable(objRet, propRet);
            TS_ASSERT_EQUALS(&obj, objRet);
//            TS_ASSERT_EQUALS(&prop, propRet);

            // operator ==

            // NOTE: actual parameter eg QObject using implicitely Observer(QObject *) to
            // fullfill the operator==(const Observer&) interface (!)

            TS_ASSERT(o2 == o2);

            TS_ASSERT(o2 == Observable(&obj));
            TS_ASSERT(o4 == Observable(&obj, method));
            TS_ASSERT(o5 == Observable(&obj, prop));

            // with copy ctor
            TS_ASSERT(o5 == Observable(o5));
        }
    void test_isNull()
        {
            Observable o1;
            TS_ASSERT(!o1.isNull());

            Observable o2((QObject*)NULL);
            TS_ASSERT(o2.isNull());

//             QObject qObj;
//             Observable o3(&qObj, (QMetaProperty*)NULL);
//             TS_ASSERT(o3.isNull());

        }

        void test_CopyCtor_Observable()
        {
            QSlider obj;
            const QMetaObject * meta = obj.metaObject();
            QMetaProperty prop = meta->property(meta->indexOfProperty("minimum"));

            // Construct Observer from Observable

            Observer observer(&obj, prop);
            Observable o1(observer);
            TS_ASSERT_EQUALS(o1.type(), Observer::QOBJECTPROPERTY);
        }

};
