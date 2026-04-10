#pragma once

#include <stdint.h>

#include "els_ui_types.h"

void bsp_ui_inputs_init(void);

/** Викликати часто (напр. з app_loop). Оновлює дебаунс. */
void bsp_ui_inputs_poll(uint32_t now_ms);

/** Стабільний знімок після дебаунсу (для меню / логіки). */
void bsp_ui_inputs_get_stable(bsp_ui_stable_t *out);

/** Остання зміна стабільного знімку (для перемальовки LCD). */
uint32_t bsp_ui_inputs_last_change_ms(void);
