#include <assert.h>
#include "utils.h"
#include "debug.h"
#include "heuristics.h"

int util_init(int xfce) {
#ifdef SATFUZZ_DEBUG
    DEBUGF("[satfuzz] Function Called: %d", xfce);
#endif
    return xfce;
}