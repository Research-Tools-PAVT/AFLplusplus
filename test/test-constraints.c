#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include <sys/shm.h>

#ifdef FUZZMAX
  #include "fuzzmax.h"
  #define SHM_FUZZMAX_ENV_VAR "__AFL_SHM_FUZZMAX_ID"

#endif

__AFL_FUZZ_INIT();

int main(void) {
#ifdef FUZZMAX

  __AFL_COVERAGE();
  __AFL_COVERAGE_START_OFF();

  char            *id_fuzzmax_str = getenv(SHM_FUZZMAX_ENV_VAR);
  uint32_t         shm_fuzzmax_id = atoi(id_fuzzmax_str);
  fuzzmax_shmem_t *__afl_fuzzmax_ptr =
      (fuzzmax_shmem_t *)shmat(shm_fuzzmax_id, NULL, 0);

  if (!__afl_fuzzmax_ptr) {
    printf("__afl_fuzzmax_ptr shmat failed\n");
    abort();
  }
#endif

  uint32_t npreds = 9;
  uint8_t  T[9] = {0};

  __AFL_INIT();
  int *magic = (int *)__AFL_FUZZ_TESTCASE_BUF;

  while (__AFL_LOOP(INT_MAX)) {
    ssize_t len = __AFL_FUZZ_TESTCASE_LEN;
    if (len < 3 * (ssize_t)sizeof(int)) return 1;

    int x = magic[0];
    int y = magic[1];
    int z = magic[2];

    T[0] = (x > y);       // ++counter;
    T[1] = (x + y == z);  // ++counter;
    T[2] = (z > 5 * y);   //  ++counter;
    T[3] = (x > y);
    T[4] = (x > 100);
    T[5] = (y < 2000);
    T[6] = (y > 110);
    T[7] = (2 * x + 3 * y > 600);
    T[8] = (x + y == 1000);

#ifdef FUZZMAX
    __afl_fuzzmax_ptr->fuzzmax_counter = 0;
    for (u32 rid = 0; rid < npreds; ++rid) {
      if (T[rid] == 0) continue;
      __afl_fuzzmax_ptr->fuzzmax_counter += T[rid];
      __afl_fuzzmax_ptr->histogram[rid] = T[rid];

      for (u32 cid = 0; cid <= rid; ++cid)
        __afl_fuzzmax_ptr->bpp_predicates_map[rid * npreds + cid] =
            T[rid] && T[cid];
    }

    if (__afl_fuzzmax_ptr->fuzzmax_counter == npreds) assert(0);

#else

    uint32_t counter = 0;
    for (uint32_t rid = 0; rid < npreds; ++rid) {
      counter += T[rid];
    }

    if (counter == npreds) {
      printf("x = %d, y = %d, z = %d\n", x, y, z);
      assert(0);
    }

#endif
  }

  return 0;
}
