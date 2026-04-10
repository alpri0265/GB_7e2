#include "bsp_ui_inputs.h"

#include <string.h>

#include "main.h"

#ifndef BSP_UI_DEBOUNCE_MS
#define BSP_UI_DEBOUNCE_MS 25U
#endif

static void hw_gpio_init(void) {
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef g = {0};
  g.Mode = GPIO_MODE_INPUT;
  g.Pull = GPIO_PULLUP;
  g.Speed = GPIO_SPEED_FREQ_LOW;

  /* PC0..PC5 — перемикач режиму (PC6/PC7 — TIM3 encoder, не чіпаємо). */
  g.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOC, &g);

  /* PF0..PF4 — меню + Select. */
  g.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
  HAL_GPIO_Init(GPIOF, &g);

  /* PG0..PG7 — джойстик, Rapid, підрежим (PG13 — TACHO out, окремо в gpio.c). */
  g.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
          GPIO_PIN_7;
  HAL_GPIO_Init(GPIOG, &g);

  /* PB0,1,2,6 — кнопки лімітів (PB7,8,9,12 — LED, не чіпаємо). */
  g.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOB, &g);
}

static void read_raw_snapshot(bsp_ui_stable_t *o) {
  memset(o, 0, sizeof(*o));

  /* PC0..PC5: один активний LOW = режим (див. els_ui_types.h). */
  uint32_t c = GPIOC->IDR & 0x3FU;
  uint32_t inv = (~c) & 0x3FU;
  if (__builtin_popcount((unsigned)inv) == 1) {
    unsigned bit = (unsigned)__builtin_ctz((unsigned)inv);
    static const els_mode_t map[6] = {
        ELS_MODE_SPHERE, /* колишній PC2 → наш PC0 */
        ELS_MODE_THREAD,
        ELS_MODE_CONE_R,
        ELS_MODE_CONE_L,
        ELS_MODE_AFEED,
        ELS_MODE_FEED,
    };
    if (bit < 6U) {
      o->mode = map[bit];
    }
  } else {
    o->mode = ELS_MODE_UNKNOWN;
  }

  /* PG5..PG7 підрежим: як у Arduino (PK7..PK5). */
  uint32_t g = GPIOG->IDR;
  uint32_t sub = (g >> 5) & 7U;
  if (sub == 6U) {
    o->submode = ELS_SUBMODE_INT;
  } else if (sub == 5U) {
    o->submode = ELS_SUBMODE_MAN;
  } else if (sub == 3U) {
    o->submode = ELS_SUBMODE_EXT;
  } else {
    o->submode = ELS_SUBMODE_UNKNOWN;
  }

  o->joy = (uint8_t)(g & 0x0FU);
  o->rapid_high = (g & GPIO_PIN_4) ? 1U : 0U;

  uint32_t f = GPIOF->IDR;
  o->menu_keys = (uint8_t)(f & 0x0FU);
  o->select_high = (f & GPIO_PIN_4) ? 1U : 0U;

  uint32_t b = GPIOB->IDR;
  uint8_t lim = 0;
  if (b & GPIO_PIN_0) {
    lim |= (1U << 0);
  }
  if (b & GPIO_PIN_1) {
    lim |= (1U << 1);
  }
  if (b & GPIO_PIN_2) {
    lim |= (1U << 2);
  }
  if (b & GPIO_PIN_6) {
    lim |= (1U << 6);
  }
  o->limits = lim;
}

static int stable_same(const bsp_ui_stable_t *a, const bsp_ui_stable_t *b) {
  return memcmp(a, b, sizeof(*a)) == 0;
}

static bsp_ui_stable_t s_stable;
static bsp_ui_stable_t s_pending;
static uint32_t s_pending_since_ms;
static uint32_t s_last_change_ms;
static uint8_t s_inited;

void bsp_ui_inputs_init(void) {
  hw_gpio_init();
  memset(&s_stable, 0, sizeof(s_stable));
  memset(&s_pending, 0, sizeof(s_pending));
  s_pending_since_ms = 0;
  s_last_change_ms = 0;
  read_raw_snapshot(&s_stable);
  s_pending = s_stable;
  s_inited = 1;
}

void bsp_ui_inputs_poll(uint32_t now_ms) {
  if (!s_inited) {
    return;
  }

  bsp_ui_stable_t raw;
  read_raw_snapshot(&raw);

  if (!stable_same(&raw, &s_pending)) {
    s_pending = raw;
    s_pending_since_ms = now_ms;
    return;
  }

  if ((now_ms - s_pending_since_ms) < BSP_UI_DEBOUNCE_MS) {
    return;
  }

  if (!stable_same(&s_pending, &s_stable)) {
    s_stable = s_pending;
    s_last_change_ms = now_ms;
  }
}

void bsp_ui_inputs_get_stable(bsp_ui_stable_t *out) {
  if (!out) {
    return;
  }
  *out = s_stable;
}

uint32_t bsp_ui_inputs_last_change_ms(void) {
  return s_last_change_ms;
}
