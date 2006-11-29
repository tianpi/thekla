// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include <common/DatabaseConn.h>
#include <common/DatabaseConnManager.h>
#include <xml/XmlElement.h>

class DatabaseConnTest : public CxxTest::TestSuite
{
private:

    class TestXmlElement : public XmlElement
    {
    public:
        TestXmlElement()
            : XmlElement(false)
            {};
        TestXmlElement(const TestXmlElement & copy)
            : XmlElement(copy)
            {};
        virtual ~TestXmlElement()
            {};
    public:
        virtual XmlElement* clone() const { return new TestXmlElement(*this); };

    };

private:
    DatabaseConn * conn_;

public:
    void setUp()
        {
            conn_ = new DatabaseConn("localhost", 20000, 20001);
        }
    void tearDown()
        {
        }

    void test_Manager()
        {
            TS_ASSERT_EQUALS(DatabaseConnManager::getConnCount(), 0);

            DatabaseConn::Info dbConnInfo("localhost", 20000, 20001);

            // Acquire connection
            DatabaseConn * dbConn = DatabaseConnManager::acquire(dbConnInfo);
            TS_ASSERT(dbConn != NULL);
            TS_ASSERT_EQUALS(DatabaseConnManager::getConnCount(), 1);
            TS_ASSERT_EQUALS(DatabaseConnManager::getUserCount(dbConn), 1);

            // Acquire connection to existant server
            DatabaseConn * dbConn2 = DatabaseConnManager::acquire(dbConnInfo);
            TS_ASSERT(dbConn == dbConn2);
            TS_ASSERT_EQUALS(DatabaseConnManager::getConnCount(), 1);
            TS_ASSERT_EQUALS(DatabaseConnManager::getUserCount(dbConn), 2);
            TS_ASSERT_EQUALS(DatabaseConnManager::getUserCount(dbConn2), 2);

            // Release connection
            DatabaseConnManager::release(dbConn);
            TS_ASSERT(dbConn == NULL);
            TS_ASSERT_EQUALS(DatabaseConnManager::getUserCount(dbConn2), 1);

            DatabaseConnManager::release(dbConn2);
            TS_ASSERT_EQUALS(DatabaseConnManager::getConnCount(), 0);
        }
};
