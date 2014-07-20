#include <time.h>
#include <sys/time.h>
#include "lightbench/bench_utils.h"

int lightbench::getCurrentTimeUsec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000000 + t.tv_usec;
}


int lightbench::getSplitReqNum(int index, int totalReqNum, int coreNum) {
    if (index == coreNum - 1) {
        return  (totalReqNum % coreNum) + (totalReqNum / coreNum);
    } else {
        return  totalReqNum / coreNum;
    }
}

