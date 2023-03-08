
#ifndef _AFL_FUZZMAX_H
#define _AFL_FUZZMAX_H

#include "types.h"


typedef struct fuzzmax_shmem {
  s32 shm_id;
  struct fuzzmax_shmem *map;
//  size_t map_size;
  
  u32 fuzzmax_counter;
  u8 histogram[256]; // TODO: update the max predicate allowed to large value later
  u32 bpp_predicates_map[65536];

} fuzzmax_shmem_t;

void update_fuzzmax_shmem(fuzzmax_shmem_t *__afl_fuzzmax_ptr, u32* T, u32 npreds);
fuzzmax_shmem_t  *afl_fuzzmax_shm_init();
void afl_fuzzmax_shm_deinit(fuzzmax_shmem_t *shm);

#endif  // _AFL_FUZZMAX_H
