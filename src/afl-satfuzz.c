#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "debug.h"
#include "heuristics.h"

int util_init(int xfce) {

    char *id = getenv("AFL_ENABLE_SATFUZZ");

    if (id) {
        int x = (int)atoi(id);
        DEBUGF("[PROCESS ===================================] %d\n", x);
    }

    return xfce;
}