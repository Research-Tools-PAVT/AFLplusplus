
#include "fuzzmax.h"

#include "types.h"
#include "debug.h"
#include "alloc-inl.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/shm.h>

void update_fuzzmax_shmem(fuzzmax_shmem_t *__afl_fuzzmax_ptr, u32* T, u32 npreds) {
  __afl_fuzzmax_ptr->fuzzmax_counter = 0;
  for (u32 rid = 0; rid < npreds; ++rid) {
    if (T[rid] == 0) continue;
    __afl_fuzzmax_ptr->fuzzmax_counter += T[rid];
    __afl_fuzzmax_ptr->histogram[rid] += T[rid];
    
    for (u32 cid = 0; cid <= rid; ++cid)
      __afl_fuzzmax_ptr->bpp_predicates_map[rid * npreds + cid] = T[rid] && T[cid];
    
  }
}

fuzzmax_shmem_t  *afl_fuzzmax_shm_init(fuzzmax_shmem_t * shm) {
  
  shm->shm_id = shmget(IPC_PRIVATE, sizeof(fuzzmax_shmem_t),
                       IPC_CREAT | IPC_EXCL | DEFAULT_PERMISSION);
  
  if (shm->shm_id < 0) {
    PFATAL("shmget() failed, try running afl-system-config");
  }
  
  shm->map = (struct fuzzmax_shmem *) shmat(shm->shm_id, NULL, 0);
  
  if (shm->map == (void *)-1 || !shm->map) {
    shmctl(shm->shm_id, IPC_RMID, NULL);  // do not leak shmem
    PFATAL("shmat() failed");
  }
  
  u8 *shm_fuzzmax_str = alloc_printf("%d", shm->shm_id);
  setenv(SHM_FUZZMAX_ENV_VAR, shm_fuzzmax_str, 1);
  ck_free(shm_fuzzmax_str);
  
  //    shm->map_size = map_size;
  //    list_append(&shm_list, shm);

  shm->fuzzmax_counter = 0;
  memset(shm->histogram, 0, sizeof(shm->histogram));
  memset(shm->bpp_predicates_map, 0, sizeof(shm->bpp_predicates_map));
  return shm->map;
}


void afl_fuzzmax_shm_deinit(fuzzmax_shmem_t *shm) {
  unsetenv(SHM_FUZZMAX_ENV_VAR);
  shmctl(shm->shm_id, IPC_RMID, NULL);  // do not leak shmem
  shm->map = NULL;
}
