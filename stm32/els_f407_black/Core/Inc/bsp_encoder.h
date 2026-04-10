#pragma once

#include <stdint.h>

typedef enum {
  ENCODER_SPINDLE = 0,
  ENCODER_HANDWHEEL = 1,
} bsp_encoder_id_t;

void bsp_encoder_init(void);
int32_t bsp_encoder_get_count(bsp_encoder_id_t id);

