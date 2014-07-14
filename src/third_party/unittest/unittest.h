#ifndef UNITEST_UNITTEST_H_
#define UNITEST_UNITTEST_H_

#include <string>
#include <vector>
#include <iostream>
#include "base/Singleton.h"
#include "unittest/unittest_util.h"
using namespace std;

#define TEST_START(TEST_NAME) \
    int main(){ \
        ::lightdis::unittest::SuiteManager::get()->run(#TEST_NAME);\
    }

#define TEST(SUITE_NAME, TEST_NAME) \
    class _TEST_TYPE_NAME(SUITE_NAME, TEST_NAME) : public ::lightdis::unittest::TestHolder{ \
        public:                                        \
                                                       _TEST_TYPE_NAME(SUITE_NAME, TEST_NAME)(const string& name) : ::lightdis::unittest::TestHolder(name){}                       \
        virtual void doTest();                      \
        static const RegistrationAgent<_TEST_TYPE_NAME(SUITE_NAME, TEST_NAME)> _agent; \
    };\
const lightdis::unittest::TestHolder::RegistrationAgent<_TEST_TYPE_NAME(SUITE_NAME, TEST_NAME)> _TEST_TYPE_NAME(SUITE_NAME, TEST_NAME)::_agent(#SUITE_NAME, #TEST_NAME);\
void _TEST_TYPE_NAME(SUITE_NAME, TEST_NAME)::doTest()


#define _TEST_TYPE_NAME(SUITE_NAME, TEST_NAME) UnitTest_##SUITE_NAME##_##TEST_NAME


#define SUITE_TRIGGER(SUITE_NAME, TYPE) \
    class _SUITE_AGENT_NAME(SUITE_NAME, TYPE) : public ::lightdis::unittest::SuiteHolder::SuiteHandlerAgent { \
        virtual void handle(); \
        static const ::lightdis::unittest::SuiteHolder::HandlerRegistrationAgent<_SUITE_AGENT_NAME(SUITE_NAME, TYPE)> _agent; \
    }; \
const ::lightdis::unittest::SuiteHolder::HandlerRegistrationAgent<_SUITE_AGENT_NAME(SUITE_NAME, TYPE)> _SUITE_AGENT_NAME(SUITE_NAME, TYPE)::_agent(#SUITE_NAME, #TYPE); \
void _SUITE_AGENT_NAME(SUITE_NAME, TYPE)::handle()

#define _SUITE_AGENT_NAME(SUITE_NAME, AGENT_TYPE) \
    SuiteHandlerAgent_##SUITE_NAME##_##AGENT_TYPE

/**
 * Assertions
 */

/**
 * Binary comparison assertions.
 */
#define ASSERT_EQUALS(a,b) _ASSERT_COMPARISON(Equal, a, b)
#define ASSERT_NOT_EQUALS(a,b) _ASSERT_COMPARISON(NotEqual, a, b)
#define ASSERT_LESS_THAN(a,b) _ASSERT_COMPARISON(LessThan, a, b)
#define ASSERT_GREATER_THAN(a,b) _ASSERT_COMPARISON(GreaterThan, a, b)
#define ASSERT_NOT_LESS_THAN(a,b) _ASSERT_COMPARISON(NotLessThan, a, b)
#define ASSERT_NOT_GREATER_THAN(a,b) _ASSERT_COMPARISON(NotGreaterThan, a, b)

#define _ASSERT_COMPARISON(COMPARISON, a, b) ::lightdis::unittest::ComparisonAssertion( \
#a, #b, __FILE__, __LINE__, this).assert##COMPARISON( (a), (b))



namespace lightdis {
    namespace unittest {
        struct FailInfo {
            FailInfo(unsigned int line, const string& file, const string& msg):line(line),
                file(file),
                msg(msg) {}

            unsigned int line;
            string file;
            string msg;
        };

        class TestHolder {
        public:
            TestHolder(const string& name):testName(name) {}
            virtual void doTest() = 0;
            void addFailInfo(unsigned int line, const string& file, const string& msg);
            vector<FailInfo> getFailInfos() const {
                return _fail_info_list;
            }

        private:
            string testName;
            vector<FailInfo> _fail_info_list;

        protected:
            template <typename T>
            class RegistrationAgent {
            public:
                RegistrationAgent(const string& suite_name, const string& test_name);
            };

        };

        class SuiteHolder {
        public:
            class SuiteHandlerAgent;
            SuiteHolder(const string& name):
                _suite_name(name),
                _before(new SuiteHandlerAgent()),
                _after(new SuiteHandlerAgent()),
                _setup(new SuiteHandlerAgent()),
                _tear_down(new SuiteHandlerAgent()) {}

            void registeTestHolder(TestHolder* holder);
            void run();
            void setBefore(SuiteHandlerAgent* agent) {
                _before = agent;
            }
            void setAfter(SuiteHandlerAgent* agent) {
                _after = agent;
            }
            void setSetup(SuiteHandlerAgent* agent) {
                _setup = agent;
            }
            void setTeardown(SuiteHandlerAgent* agent) {
                _tear_down = agent;
            }
            string getName() {
                return _suite_name;
            }
            int getSize() {
                return _test_holder_list.size();
            }
            int getFailNum();
            vector<TestHolder*> getTestHolders() const {
                return _test_holder_list;
            }
            class SuiteHandlerAgent {
            public:
                virtual void handle() {}
            };

            template <typename T>
            class HandlerRegistrationAgent {
            public:
                HandlerRegistrationAgent(const string& suite_name, const string& type);
            };
        private:
            string _suite_name;
            vector<TestHolder*> _test_holder_list;
            SuiteHandlerAgent* _before;
            SuiteHandlerAgent* _after;
            SuiteHandlerAgent* _setup;
            SuiteHandlerAgent* _tear_down;
        };


        class SuiteManager {
        public:
            SuiteHolder& getSuiteHolder(const string& suite_name);
            void run(const string& test_name);
            static SuiteManager* get() {
                return _instance;
            }

        private:
            void report();
            vector<SuiteHolder*> _suite_holder_list;
            static SuiteManager* _instance;

        };

        SuiteManager* SuiteManager::_instance = new SuiteManager();

        class TestAssertion {
        protected:
            TestAssertion(const char* file, unsigned line, TestHolder* test_holder):_test_holder(test_holder),_file(file),_line(line) {}
            void fail(const string& msg);
            TestHolder* _test_holder;
        private:
            const char* _file;
            const unsigned _line;
        };

        class ComparisonAssertion : public TestAssertion {
        public:
            ComparisonAssertion(const char* aexp, const char* bexp,
                                const char* file, unsigned line, TestHolder* test_holder):TestAssertion(file, line, test_holder),_aexp(aexp),_bexp(bexp) {}

            template<typename A, typename B>
            void assertEqual(const A& a, const B& b) {
                if (a == b)
                    return;
                fail(getFailMessage("==", a, b));
            }

            template<typename A, typename B>
            void assertNotEqual(const A& a, const B& b) {
                if (a != b)
                    return;
                fail(getFailMessage("!=", a, b));
            }

            template<typename A, typename B>
            void assertLessThan(const A& a, const B& b) {
                if (a < b)
                    return;
                fail(getFailMessage("<", a, b));
            }

            template<typename A, typename B>
            void assertGreaterThan(const A& a, const B& b) {
                if (a > b)
                    return;
                fail(getFailMessage(">", a, b));
            }

            template<typename A, typename B>
            void assertNotLessThan(const A& a, const B& b) {
                if (a >= b)
                    return;
                fail(getFailMessage(">=", a, b));
            }

            template<typename A, typename B>
            void assertNotGreaterThan(const A& a, const B& b) {
                if (a <= b)
                    return;
                fail(getFailMessage("<=", a, b));
            }


        private:
            template<typename A, typename B>
            string getFailMessage(const string& operation, A& a, B& b);
            const char* _aexp;
            const char* _bexp;
        };

    }
}

#include "unittest/unittest_inl.h"
#endif

