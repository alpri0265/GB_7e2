#include "bsp_encoder.h"

#include "main.h"

extern TIM_HandleTypeDef htim3; // handwheel (Encoder mode)
extern TIM_HandleTypeDef htim4; // spindle  (Encoder mode)

void bsp_encoder_init(void) {
  (void)HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  (void)HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

int32_t bsp_encoder_get_count(bsp_encoder_id_t id) {
  switch (id) {
    case ENCODER_HANDWHEEL:
      return (int32_t)__HAL_TIM_GET_COUNTER(&htim3);
    case ENCODER_SPINDLE:
    default:
      return (int32_t)__HAL_TIM_GET_COUNTER(&htim4);
  }
}

