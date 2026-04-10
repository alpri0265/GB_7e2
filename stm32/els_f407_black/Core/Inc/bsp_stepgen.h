#pragma once

#include <stdint.h>

typedef enum {
  AXIS_Z = 0,
  AXIS_X = 1,
} bsp_axis_t;

void bsp_stepgen_init(void);
void bsp_stepgen_set_enable(bsp_axis_t axis, int enable);
void bsp_stepgen_set_dir(bsp_axis_t axis, int dir_cw);
void bsp_stepgen_set_freq_hz(bsp_axis_t axis, uint32_t hz);
void bsp_stepgen_start(bsp_axis_t axis);
void bsp_stepgen_stop(bsp_axis_t axis);

