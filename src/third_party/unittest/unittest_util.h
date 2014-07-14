#ifndef UNITTEST_UNITTEST_UTIL_H_
#define UNITTEST_UNITTEST_UTIL_H_

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <string>

using namespace std;

namespace lightdis {
    namespace unittest {

        /*
         * print out the msg with format time infomation
         */
        inline void echo(const string& msg) {
            time_t t = time(0);
            char tmp[64];
            strftime(tmp, sizeof(tmp), "%H:%M:%S", localtime(&t));
            struct timeval tv;
            gettimeofday(&tv, NULL);
            cout << tmp << "." << tv.tv_usec / 1000 << " [" << msg << "]" << endl;
        }

    }
}

#endif
