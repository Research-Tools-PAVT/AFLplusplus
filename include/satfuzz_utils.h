#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

#define GAP 5
#define BASE_OFFSET 50
#define COUNTER_WRITES 1000

#define PRINT_FREQ 8192

#ifdef FUZZMAX
#define update_heuristic(cond, id, counter, __afl_area_ptr, current_hit_bv)                                            \
    counter += (cond);                                                                                                 \
    __afl_area_ptr[id] = (cond);                                                                                       \
    current_hit_bv[id] = (cond);
#else
#define update_heuristic(cond, id, counter, __afl_area_ptr, current_hit_bv)                                            \
    counter += (cond);                                                                                                 \
    current_hit_bv[id] = (cond);
#endif

void update_two_predicate_map(uint8_t *__afl_area_ptr, uint8_t *current_hit_bv, uint8_t NUM_PREDS) {
    uint64_t offset = BASE_OFFSET + 4 * (NUM_PREDS + 1) + GAP;
    for (int i = 0; i < NUM_PREDS; i++) {
        if (!current_hit_bv[i])
            continue;
        for (int j = 0; j < NUM_PREDS; j++) {
            if (!current_hit_bv[j])
                continue;
            uint64_t index = ((i * NUM_PREDS) + j);
            __afl_area_ptr[offset + index] = 1;
        }
    }
}

void update_n_1_map(uint8_t *__afl_area_ptr, uint8_t *current_hit_bv, uint64_t index, uint8_t NUM_PREDS) {
    uint64_t offset = BASE_OFFSET + 2 * NUM_PREDS + 1 + GAP;
    __afl_area_ptr[index + offset] = 1;
    return;
}

void update_n_2_map(uint8_t *__afl_area_ptr, uint8_t *current_hit_bv, uint64_t index, uint8_t NUM_PREDS) {
    uint64_t offset = BASE_OFFSET + 3 * NUM_PREDS + 2 + GAP;
    __afl_area_ptr[index + offset] = 1;
    return;
}
