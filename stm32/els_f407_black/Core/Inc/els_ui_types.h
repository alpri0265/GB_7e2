#pragma once

#include <stdint.h>

/**
 * Режими як у Arduino Menu.ino (enum Mode).
 * На STM32 перемикач фізично на PC0..PC5 (PC6/PC7 — енкодер ручки): один активний LOW.
 * Порядок пінів відповідає переносу ліній з Mega: колишній PC7→PC5 … PC2→PC0.
 * Режими Tachometr / Reserve поки не на цих шести лініях — додайте окремі піни пізніше.
 */
typedef enum {
  ELS_MODE_UNKNOWN = 0,
  ELS_MODE_FEED,
  ELS_MODE_AFEED,
  ELS_MODE_CONE_L,
  ELS_MODE_CONE_R,
  ELS_MODE_THREAD,
  ELS_MODE_SPHERE,
  ELS_MODE_TACHO,
  ELS_MODE_RESERVE,
} els_mode_t;

/** Підрежим Int / Man / Ext (як Sub_Mode_* у Arduino). */
typedef enum {
  ELS_SUBMODE_UNKNOWN = 0,
  ELS_SUBMODE_INT,
  ELS_SUBMODE_MAN,
  ELS_SUBMODE_EXT,
} els_submode_t;

typedef struct {
  els_mode_t mode;
  els_submode_t submode;
  /** PG0..PG3 джойстик: біти 0..3, 1 = pin HIGH (відпущено), 0 = LOW. */
  uint8_t joy;
  uint8_t rapid_high; /* PG4 */
  /** PF0..PF3 меню, PF4 Select — як на Mega (активний LOW). */
  uint8_t menu_keys;
  uint8_t select_high;
  /** PB0,PB1,PB2,PB6 — ліміти, біти 0,1,2,6 у молодшому байті. */
  uint8_t limits;
} bsp_ui_stable_t;
