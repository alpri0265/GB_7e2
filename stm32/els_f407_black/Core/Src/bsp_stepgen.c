#include "bsp_stepgen.h"

#include "main.h"

extern TIM_HandleTypeDef htim1; // PWM for STEP

// DIR/ENA pins (з розпіновки 2.6)
#define Z_ENA_GPIO_Port GPIOE
#define Z_ENA_Pin       GPIO_PIN_8
#define X_ENA_GPIO_Port GPIOE
#define X_ENA_Pin       GPIO_PIN_7
#define Z_DIR_GPIO_Port GPIOE
#define Z_DIR_Pin       GPIO_PIN_10
#define X_DIR_GPIO_Port GPIOE
#define X_DIR_Pin       GPIO_PIN_12

static uint32_t tim1_get_clk_hz(void) {
  // TIM1 on APB2. If APB2 prescaler != 1 then timer clock = 2*PCLK2.
  uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
  uint32_t presc = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;
  // presc encoding: 0xxx => /1, 100 =>/2, 101=>/4, 110=>/8, 111=>/16
  if (presc >= 4) return pclk2 * 2U;
  return pclk2;
}

void bsp_stepgen_init(void) {
  // ensure outputs default
  bsp_stepgen_set_enable(AXIS_Z, 0);
  bsp_stepgen_set_enable(AXIS_X, 0);
  bsp_stepgen_set_dir(AXIS_Z, 1);
  bsp_stepgen_set_dir(AXIS_X, 1);

  // start PWM channels (outputs will toggle when enabled)
  (void)HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // Z_STEP
  (void)HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // X_STEP
}

void bsp_stepgen_set_enable(bsp_axis_t axis, int enable) {
  GPIO_TypeDef *port = (axis == AXIS_Z) ? Z_ENA_GPIO_Port : X_ENA_GPIO_Port;
  uint16_t pin = (axis == AXIS_Z) ? Z_ENA_Pin : X_ENA_Pin;
  HAL_GPIO_WritePin(port, pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void bsp_stepgen_set_dir(bsp_axis_t axis, int dir_cw) {
  GPIO_TypeDef *port = (axis == AXIS_Z) ? Z_DIR_GPIO_Port : X_DIR_GPIO_Port;
  uint16_t pin = (axis == AXIS_Z) ? Z_DIR_Pin : X_DIR_Pin;
  HAL_GPIO_WritePin(port, pin, dir_cw ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void bsp_stepgen_set_freq_hz(bsp_axis_t axis, uint32_t hz) {
  if (hz == 0) return;

  // We run TIM1 with PSC chosen so ARR fits 16-bit.
  const uint32_t timclk = tim1_get_clk_hz();

  // target: update rate = hz, PWM output = hz, so period = timclk/(psc+1)/(arr+1)
  // choose psc so arr <= 65535
  uint32_t psc = 0;
  uint32_t arr = (timclk / hz) - 1U;
  while (arr > 65535U) {
    psc++;
    arr = (timclk / (hz * (psc + 1U))) - 1U;
    if (psc > 65535U) break;
  }

  __HAL_TIM_DISABLE(&htim1);
  __HAL_TIM_SET_PRESCALER(&htim1, psc);
  __HAL_TIM_SET_AUTORELOAD(&htim1, arr);

  // 50% duty
  uint32_t ccr = (arr + 1U) / 2U;
  if (axis == AXIS_Z) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr);
  } else {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ccr);
  }

  __HAL_TIM_SET_COUNTER(&htim1, 0);
  __HAL_TIM_ENABLE(&htim1);
}

void bsp_stepgen_start(bsp_axis_t axis) {
  // PWM already started; keep enabled pin on
  bsp_stepgen_set_enable(axis, 1);
}

void bsp_stepgen_stop(bsp_axis_t axis) {
  bsp_stepgen_set_enable(axis, 0);
}

