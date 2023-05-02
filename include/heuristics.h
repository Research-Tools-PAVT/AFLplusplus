/**
 * Declear utilites for SATFUZZ implementation.
*/

#ifndef __HEURISTICS_H
#define __HEURISTICS_H

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include "debug.h"

int util_init(int xfce) {

    char *id = getenv("AFL_ENABLE_SATFUZZ");

    if (id) {
        int x = (int)atoi(id);
        DEBUGF("[PROCESS ===================================] %d\n", x);
    }

    return xfce;
}

void min_max_scaler(double *arr, size_t size) {
//     double x_min = arr[0];
//     double x_max = arr[0];
//     for (size_t i = 0; i < size; i++) {
//         if (x_min >= arr[i])
//             x_min = arr[i]; // min
//         if (x_max <= arr[i])
//             x_max = arr[i]; // max
// #ifdef DEBUG
//         printf("[arr] %lf\n", arr[i]);
// #endif
//     }
// #ifdef DEBUG
//     printf("[min] %lf\n", x_min);
//     printf("[max] %lf\n", x_max);
// #endif
//     for (size_t i = 0; i < size; i++) {
//         arr[i] = (x_max - arr[i]) / (x_max - x_min);
// #ifdef DEBUG
//         printf("[minmax] arr[%u]=%lf\n", i, arr[i]);
// #endif
//     }
    return;
}

double energy_f2(uint8_t *heuf, uint64_t NUM_PREDS) {
    // TODO: Normalize histogram values
//     double arr[NUM_PREDS];
//     memcpy(arr, histogram, NUM_PREDS * sizeof(uint64_t));
//     min_max_scaler(arr, NUM_PREDS);

// #ifdef DEBUG
//     for (size_t i = 0; i < NUM_PREDS; i++) {
//         printf("[after_norm] arr[%u]=%lf\n", i, arr[i]);
//     }
// #endif

//     double histogram_energy = 0.00, run_energy = (double)histogram[0];

//     for (size_t i = 0; i < NUM_PREDS; i++) {
//         if (get_bv_bit(current_coverage, i) && (histogram[i] <= run_energy)) {
//             run_energy = histogram[i];
//             histogram_energy = (double)arr[i];
// #ifdef DEBUG
//             printf("[energy] arr[%u]=%lf\n", i, histogram_energy);
// #endif
//         }
//     }

// #ifdef DEBUG
//     printf("[energy_F2] %lf\n", histogram_energy);
// #endif
    return 2.76;
}

/**
 * @brief Print the state of the heuristics.
 * to a file in FILE *fptr.
 * @param heuf
 */
void print_heuristics(uint8_t *heuf, uint64_t 
                        LOOP_COUNTER, uint64_t MAX_COUNTER) {
    return;
}

#endif
