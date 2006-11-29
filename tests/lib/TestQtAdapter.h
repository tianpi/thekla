// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <adapter/QtAdapter.h>
#include <adapter/QtAdapterClassList.h>

#include <QApplication>
#include <QSlider>

class QtAdapterTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {
        }
    void tearDown()
        {
        }
//     void test_QObjectAdapter()
//         {
//             QObjectAdapter * adapter = dynamic_cast<QObjectAdapter*>(QObjectAdapter::create(NULL));

//             QObject * obj = new QObject;
//             QObject::connect(obj, SIGNAL(destroyed()), adapter, SLOT(destroyedSlot()));

//             // TEST: signal from observable arrives at adapter slot
//             delete obj;

//             QObject::connect(adapter, SIGNAL(destroyedSignal()), adapter, SLOT(propertyUpdateTrigger()));

//             // TEST: adapter signal arrives at connected (adapter) slot
//             QByteArray a;
//             adapter->signalEmit("destroyed()", a);
//         }
//     void test_QSliderAdapter()
//         {
//             QSliderAdapter * adapter = dynamic_cast<QSliderAdapter*>(QSliderAdapter::create(NULL));
//             QSlider * obj = new QSlider;

//             QObject::connect(obj, SIGNAL(valueChanged(int)), adapter, SLOT(valueChangedSlot(int)));

//             // TEST: signal from QSlider arrives at adapter slot
//             obj->setMinimum(0);
//             obj->setMaximum(100);
//             obj->setValue(50);
//         }
};
