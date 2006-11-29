// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <plugin/QDFormWindowUiFileObserver.h>
#include <QFile>

class UiFileObserverTest : public CxxTest::TestSuite
{
public:
    void setUp()
        {

        }
    void tearDown()
        {
        }
    void test_XXX()
        {

            QDFormWindowUiFileObserver uiFileObs;
            uiFileObs.start("");

            QString fileName("form.ui");
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            QByteArray text("Hello World");
            file.write(text);

            uiFileObs.setUiFileName(fileName);
            // emit signal

            file.write(text);
            // emit signal

            // .. but cannot test ..

            file.close();
            QFile::remove(fileName);
        }
};
