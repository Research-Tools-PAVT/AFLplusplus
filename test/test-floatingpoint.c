#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <sys/shm.h>
#define SHM_CFREQ_ENV_VAR "__AFL_SHM_CFREQ_ID"

__AFL_FUZZ_INIT();

int main(void) {

  ssize_t bytes_read;

  __AFL_INIT();
  float *magic = (float *)__AFL_FUZZ_TESTCASE_BUF;

#ifdef FUZZMAX
  char    *id_cfreq_str = getenv(SHM_CFREQ_ENV_VAR);
  uint32_t shm_cfreq_id = atoi(id_cfreq_str);
  uint8_t *__afl_cfreq_ptr = (uint8_t *)shmat(shm_cfreq_id, NULL, 0);

  if (!__afl_cfreq_ptr) abort();
#endif

  while (__AFL_LOOP(INT_MAX)) {

    int len = __AFL_FUZZ_TESTCASE_LEN;
    if (len < sizeof(float)) return 1;

    /* 15 + 1/2                      = 15.5  */
    /* 15 + 1/2 + 1/8                = 15.625  */
    /* 15 + 1/2 + 1/8 + 1/32         = 15.65625  */
    /* 15 + 1/2 + 1/8 + 1/32 + 1/128 = 15.6640625  */
    if ((*magic >= 15.0 + 0.5 + 0.125 + 0.031) &&
        (*magic <= 15.0 + 0.5 + 0.125 + 0.031 + 2.0))
      abort();
    #ifdef FUZZMAX
      else { *__afl_cfreq_ptr = 5; }
    #endif
  }

  return 0;

}
