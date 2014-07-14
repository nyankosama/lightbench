#include <iostream>
#include "unittest/unittest.h"
#include "base/string_data.h"

using namespace std;
using namespace lightdis::base;

SUITE_TRIGGER(SUITE_CASE_1, BEFORE) {
    cout << "before!" << endl;
}

SUITE_TRIGGER(SUITE_CASE_1, AFTER) {
    cout << "after!!" << endl;
}

SUITE_TRIGGER(SUITE_CASE_1, SETUP) {
    cout << "setup!" << endl;
}

SUITE_TRIGGER(SUITE_CASE_1, TEAR_DOWN) {
    cout << "tearDown!" << endl;
}

TEST(SUITE_CASE_1, TEST_1) {
    cout << "hello unittest!" << endl;
    StringData str1("123");
    StringData str2("12");
    ASSERT_EQUALS(str1, str2);
}

TEST_START()

