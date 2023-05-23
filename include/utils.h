/*
    american fuzzy lop++ - afl-proxy skeleton example
    ---------------------------------------------------

    Written by Marc Heuse <mh@mh-sec.de>

    Copyright 2019-2023 AFLplusplus Project. All rights reserved.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at:

    http://www.apache.org/licenses/LICENSE-2.0


    HOW-TO
    ======

    You only need to change the while() loop of the main() to send the
    data of buf[] with length len to the target and write the coverage
    information to __afl_area_ptr[__afl_map_size]


 */

#ifdef __ANDROID__
  #include "android-ashmem.h"
#endif
#include "config.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>

#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#ifdef __ANDROID__
u32 afl_map_size = MAP_SIZE;
#else
__thread u32 afl_map_size = MAP_SIZE;
#endif

/* SHM setup. */

u8 *get_fm_ptr(void) {
  char *id_str = getenv(FM_SHM_ENV_VAR);
  u32   shm_id = atoi(id_str);
  return shmat(shm_id, 0, 0);
}

u8 *get_afl_area_ptr(void) {
  char *id_str = getenv(SHM_ENV_VAR);
  char *ptr;

  u8 *afl_area_ptr = NULL;

  /* NOTE TODO BUG FIXME: if you want to supply a variable sized map then
     uncomment the following: */

  /*
  if ((ptr = getenv("AFL_MAP_SIZE")) != NULL) {

    u32 val = atoi(ptr);
    if (val > 0) afl_map_size = val;

  }

  */

  if (afl_map_size > MAP_SIZE) {
    if (afl_map_size > FS_OPT_MAX_MAPSIZE) {
      fprintf(stderr,
              "Error: AFL++ tools *require* to set AFL_MAP_SIZE to %u to "
              "be able to run this instrumented program!\n",
              afl_map_size);
      if (id_str) { return NULL; }

    } else {
      fprintf(stderr,
              "Warning: AFL++ tools will need to set AFL_MAP_SIZE to %u to "
              "be able to run this instrumented program!\n",
              afl_map_size);
    }
  }

  if (id_str) {
#ifdef USEMMAP
    const char    *shm_file_path = id_str;
    int            shm_fd = -1;
    unsigned char *shm_base = NULL;

    /* create the shared memory segment as if it was a file */
    shm_fd = shm_open(shm_file_path, O_RDWR, 0600);
    if (shm_fd == -1) {
      fprintf(stderr, "shm_open() failed\n");
      return NULL;
    }

    /* map the shared memory segment to the address space of the process */
    shm_base =
        mmap(0, afl_map_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shm_base == MAP_FAILED) {
      close(shm_fd);
      shm_fd = -1;

      fprintf(stderr, "mmap() failed\n");
      return NULL;
    }

    afl_area_ptr = shm_base;
#else
    u32 shm_id = atoi(id_str);
    afl_area_ptr = shmat(shm_id, 0, 0);
#endif
  }

  return afl_area_ptr;
}

void check_sat(u8 *T, u32 npreds, u8 *afl_area_ptr, u8 *fm_map) {
  u32 fuzzmax_counter = 0;

#ifndef CRASH_VALIDATION
  u32 hid = 1;
  for (u32 rid = 0; rid < npreds; ++rid) {
    fuzzmax_counter += T[rid];

    for (u32 cid = 0; cid <= rid; ++cid)
      afl_area_ptr[hid++] = T[rid] && T[cid];
  }

  afl_area_ptr[0] = 1;
  fm_map[0] = fuzzmax_counter;
  fm_map[1] = npreds;

#else
  for (u32 rid = 0; rid < npreds; ++rid)
    fuzzmax_counter += T[rid];
#endif

  if (fuzzmax_counter == npreds) assert(0);
}
