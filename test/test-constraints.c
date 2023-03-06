#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include <sys/shm.h>

#ifdef FUZZMAX
  #include "fuzzmax.h"
#endif

#define SHM_FUZZMAX_ENV_VAR "__AFL_SHM_FUZZMAX_ID"

__AFL_FUZZ_INIT();

int main(void) {
#ifdef FUZZMAX
  char            *id_fuzzmax_str = getenv(SHM_FUZZMAX_ENV_VAR);
  uint32_t         shm_fuzzmax_id = atoi(id_fuzzmax_str);
  fuzzmax_shmem_t *__afl_fuzzmax_ptr =
      (fuzzmax_shmem_t *)shmat(shm_fuzzmax_id, NULL, 0);

  if (!__afl_fuzzmax_ptr) {
    printf("__afl_fuzzmax_ptr shmat failed\n");
    abort();
  }
#endif

  __AFL_INIT();
  int *magic = (int *)__AFL_FUZZ_TESTCASE_BUF;

  while (__AFL_LOOP(INT_MAX)) {
    ssize_t len = __AFL_FUZZ_TESTCASE_LEN;
    if (len < 3 * (ssize_t)sizeof(int)) return 1;

    int x = magic[0];
    int y = magic[1];
    int z = magic[2];

    uint8_t counter = 0;
    if (x > y) ++counter;
    if (x + y == z) ++counter;
    if (z > 5 * y) ++counter;

    if (counter == 3) {
      printf("x = %d, y = %d, z = %d\n", x, y, z);
      assert(0);
    }

#ifdef FUZZMAX
    __afl_fuzzmax_ptr->fuzzmax_counter = counter;
#endif
  }

  return 0;
}
