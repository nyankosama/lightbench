#include "unittest/unittest.h"
#include <iostream>
#include <sstream>

using namespace std;

namespace lightdis {
    namespace unittest {

        template <typename T>
        TestHolder::RegistrationAgent<T>::RegistrationAgent(const string& suite_name,const string& test_name) {
            T* testObj = new T(test_name);
            SuiteManager::get()->getSuiteHolder(suite_name).registeTestHolder(testObj);
        }

        void TestHolder::addFailInfo(unsigned int line, const string& file, const string& msg) {
            _fail_info_list.push_back(FailInfo(line, file, msg));
        }

        template <typename T>
        SuiteHolder::HandlerRegistrationAgent<T>::HandlerRegistrationAgent(const string& suite_name, const string& type) {
            T* handler = new T();
            SuiteHolder& suite_holder = SuiteManager::get()->getSuiteHolder(suite_name);
            //TODO ugly implement!!!
            if (type == "BEFORE") {
                suite_holder.setBefore(handler);
            } else if(type == "AFTER") {
                suite_holder.setAfter(handler);
            } else if(type == "SETUP") {
                suite_holder.setSetup(handler) ;
            } else if(type == "TEAR_DOWN") {
                suite_holder.setTeardown(handler);
            }

        }

        void SuiteHolder::registeTestHolder(TestHolder* holder) {
            _test_holder_list.push_back(holder);
        }

        void SuiteHolder::run() {
            cout << "--------Suite name:" << _suite_name  << endl;
            _setup->handle();
            for(vector<TestHolder*>::iterator iter = _test_holder_list.begin(); iter != _test_holder_list.end(); ++iter) {
                _before->handle();
                (*iter)->doTest();
                _after->handle();
            }
            _tear_down->handle();
        }


        SuiteHolder& SuiteManager::getSuiteHolder(const string& suite_name) {
            SuiteHolder* holder = NULL;
            for (vector<SuiteHolder*>::iterator iter = _suite_holder_list.begin(); iter != _suite_holder_list.end(); ++iter) {
                if ((*iter)->getName() == suite_name) {
                    holder = *iter;
                    break;
                }
            }
            if (holder == NULL) {
                holder = new SuiteHolder(suite_name);
                _suite_holder_list.push_back(holder);
            }
            return *holder;
        }

        void SuiteManager::run(const string& test_name) {
            cout << endl;
            cout << "----Test start:  " << test_name << endl;
            for (vector<SuiteHolder*>::iterator iter = _suite_holder_list.begin(); iter != _suite_holder_list.end(); ++iter) {
                (*iter)->run();
            }
            report();
            cout << "----Test end" << endl;
        }

        void TestAssertion::fail(const string& msg) {
            _test_holder->addFailInfo(_line, _file, msg);
            cout << "------------Assertion fail at line=" << _line << ", file=" << _file
                 << ", msg=" << msg;

            cout << endl;
        }

        void SuiteManager::report() {
            int suite_total = _suite_holder_list.size();
            int test_fail = 0;
            int test_total = 0;
            int assertion_fail = 0;
            for (vector<SuiteHolder*>::iterator iter = _suite_holder_list.begin(); iter != _suite_holder_list.end(); ++iter) {
                vector<TestHolder*> test_holders = (*iter)->getTestHolders();
                test_total += test_holders.size();
                for (vector<TestHolder*>::iterator iter = test_holders.begin(); iter != test_holders.end(); ++iter) {
                    if ((*iter)->getFailInfos().size() != 0) {
                        test_fail += 1;
                        assertion_fail += (*iter)->getFailInfos().size();
                    }
                }
            }

            cout << "--------Report: " << (test_fail == 0 ? "Pass" : "Fail") << endl;
            cout << "------------" << "Suite total: " << suite_total << endl;
            cout << "------------" << "Test total: " << test_total << endl;
            cout << "------------" << "Test fail total: "  << test_fail << endl;
            cout << "------------" << "Assertion fail total: "  << assertion_fail << endl;
        }

        template<typename A, typename B>
        string ComparisonAssertion::getFailMessage(const string& operation, A& a, B& b) {
            ostringstream os;
            os << "Expected " << _aexp << " " << operation << " " << _bexp
               << " (" << a << " " << operation << " " << b << ")";
            return os.str();
        }
    }
}

