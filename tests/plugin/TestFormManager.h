// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <plugin/QDFormManager.h>

class FormManager : public CxxTest::TestSuite
{
private:
    QDesignerFormEditorInterface * formEditor_;
public:
    void setUp()
        {
            formEditor_ = new QDesignerFormEditorInterface;
//            QDFormManager::init(formEditor_);
        }
    void tearDown()
        {
        }
    void test_XXX()
        {
//            QDesignerFormWindowManagerInterface * formWindowManager = formEditor_->formWindowManager();
//            QDesignerFormWindowInterface * formManager = get
        }
};
