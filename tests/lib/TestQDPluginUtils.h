// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/QDPluginUtils.h>
#include <codegen/CodeGenerator.h>

class QDPluginUtilsTest : public CxxTest::TestSuite
{
public:
public:
    void setUp()
        {
        }
    void tearDown()
        {
        }
    void test_getConfigFilePath()
        {
            QString in, out, ret;

            in = "form.ui";
            out = "form-thekla.xml";
            ret = QDPluginUtils::getConfigFilePath(in);
            TS_ASSERT_EQUALS(ret, out);

            in = "Di da tralala.ui";
            out = "Di da tralala-thekla.xml";
            ret = QDPluginUtils::getConfigFilePath(in);
            TS_ASSERT_EQUALS(ret, out);

            in = "hicks";
            out = "hicks-thekla.xml";
            ret = QDPluginUtils::getConfigFilePath(in);
            TS_ASSERT_EQUALS(ret, out);

            in = "c:\\windows\\program files\\form.ui";
            out = "c:\\windows\\program files\\form-thekla.xml";
            ret = QDPluginUtils::getConfigFilePath(in);
            TS_ASSERT_EQUALS(ret, out);

            in = "/unix/share/form.ui";
            out = "/unix/share/form-thekla.xml";
            ret = QDPluginUtils::getConfigFilePath(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());


        }
    void test_getUiFilePath()
        {
            QString in, out, ret;

            in = "/unix/share/form.ui";
            out = "/unix/share/ui_form.h";
            ret = QDPluginUtils::getUiHeaderFilePath(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());
        }

    void test_getUiWrapperFilePath()
        {
            QString in, out, ret;

            in = "form.ui";
            out = "thekla_form.h";
            ret = QDPluginUtils::getUiWrapperFilePath(in);
            TS_ASSERT_EQUALS(ret, out);

            in = "Di da tralala.ui";
            out = "thekla_Di da tralala.h";
            ret = QDPluginUtils::getUiWrapperFilePath(in);
            TS_ASSERT_EQUALS(ret, out);

            in = "hicks";
            out = "thekla_hicks.h";
            ret = QDPluginUtils::getUiWrapperFilePath(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());

            in = "c:\\windows\\program files\\form.ui";
            out = "c:\\windows\\program files\\thekla_form.h";
            ret = QDPluginUtils::getUiWrapperFilePath(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());

            in = "/unix/share/form.ui";
            out = "/unix/share/thekla_form.h";
            ret = QDPluginUtils::getUiWrapperFilePath(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());


        }

    void test_truncate()
        {
            QString in, out, ret;

            in = "c:\\windows\\program files\\form.ui";
            out = "form.ui";
            ret = QDPluginUtils::getFileName(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());

            in = "c:\\windows\\program files\\form.ui";
            out = "c:\\windows\\program files\\";
            ret = QDPluginUtils::getPathName(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());

            in = "/unix/share/form.ui";
            out = "form.ui";
            ret = QDPluginUtils::getFileName(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());

            in = "/unix/share/form.ui";
            out = "/unix/share/";
            ret = QDPluginUtils::getPathName(in);
            TS_ASSERT_EQUALS(ret.toStdString(), out.toStdString());
        }

    void test_getModTime()
        {
            // NOTE: This is a bad test, because it relies on other (untested) API
            // methods.

            QString fileName(QtObjectUtils::getUniqueIdentifier());

            // file does not exist yet -> modTime == 0
            TS_ASSERT_EQUALS(QFile::exists(fileName), false);
            TS_ASSERT_EQUALS(QDPluginUtils::getModifyTime(fileName), -1);

            // create file -> modTime >= 0
            CodeGenerator::saveToFile(fileName, "Hello");
            TS_ASSERT_EQUALS(QFile::exists(fileName), true);
            TS_ASSERT(QDPluginUtils::getModifyTime(fileName) >= 0);


            // modify file: compare result1 '<=' results2 (enable sleep() to tests with
            // strict operator '<')

            time_t before = -1, after = -1;
            before = QDPluginUtils::getModifyTime(fileName);

//            log4cplus::helpers::sleep(1, 100);
            CodeGenerator::saveToFile(fileName, "World");

            after = QDPluginUtils::getModifyTime(fileName);
            TS_ASSERT(before <= after);
//            TS_ASSERT(before < after);

            QFile::remove(fileName);
        }

};
