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

#include "afl-fuzz.h"
#include "debug.h"

void min_max_scaler(double *arr, size_t size) {
  double x_min = arr[0];
  double x_max = arr[0];

  for (size_t i = 1; i < size; i++) {
    if (x_min >= arr[i]) x_min = arr[i];  // min
    if (x_max <= arr[i]) x_max = arr[i];  // max
  }

  for (size_t i = 0; i < size; i++) {
    arr[i] = (x_max - arr[i]) / (x_max - x_min);
  }

  return;
}

double energy_f2(uint8_t *trace_bits, afl_state_t *afl, uint64_t NUM_PREDS, size_t offset) {
  // TODO: Normalize histogram values
  double arr[NUM_PREDS];

  // Update the histogram and the array for normalization.
  for (uint64_t k = 0; k < NUM_PREDS; k++) {
    arr[k] = afl->shm_fm_extra.map[offset + k];
  }

  // Perform min-max like normalization on the array.
  min_max_scaler(arr, NUM_PREDS);

  double histogram_energy = 0.00;
  uint32_t run_energy = afl->shm_fm_extra.map[offset + 0];

  for (uint64_t i = 0; i < NUM_PREDS; i++) {
    if (trace_bits[i + 1] > 0 && (afl->shm_fm_extra.map[offset + i] <= run_energy)) {
      run_energy = afl->shm_fm_extra.map[offset + i];
      histogram_energy = (double)arr[i];
    }
  }

  return histogram_energy;
}


#endif
