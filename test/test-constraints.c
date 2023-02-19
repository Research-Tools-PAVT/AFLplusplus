#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

#include <sys/shm.h>

#define SHM_CFREQ_ENV_VAR "__AFL_SHM_CFREQ_ID"

__AFL_FUZZ_INIT();

int main(void) {
  ssize_t bytes_read;

#ifdef FUZZMAX
  char    *id_cfreq_str = getenv(SHM_CFREQ_ENV_VAR);
  uint32_t shm_cfreq_id = atoi(id_cfreq_str);
  uint8_t *__afl_cfreq_ptr = (uint8_t *)shmat(shm_cfreq_id, NULL, 0);

  if (!__afl_cfreq_ptr) abort();
#endif

  __AFL_INIT();
  int *magic = (int *)__AFL_FUZZ_TESTCASE_BUF;

  while (__AFL_LOOP(INT_MAX)) {
    int len = __AFL_FUZZ_TESTCASE_LEN;
    if (len < 2 * sizeof(int)) return 1;

    int x = magic[0];
    int y = magic[1];

    uint8_t counter = 0;
    if (x > y) ++counter;
    if (x == 10 && y < 20) ++counter;
    
#ifdef FUZZMAX
    *__afl_cfreq_ptr = counter;
#endif

    if (counter == 2) abort();
  }

  return 0;
}
