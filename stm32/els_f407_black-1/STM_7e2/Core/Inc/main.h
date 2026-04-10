/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_Pin GPIO_PIN_0
#define BTN_GPIO_Port GPIOF
#define BTNF1_Pin GPIO_PIN_1
#define BTNF1_GPIO_Port GPIOF
#define BTNF2_Pin GPIO_PIN_2
#define BTNF2_GPIO_Port GPIOF
#define BTNF3_Pin GPIO_PIN_3
#define BTNF3_GPIO_Port GPIOF
#define BTN_SEL_Pin GPIO_PIN_4
#define BTN_SEL_GPIO_Port GPIOF
#define MODE_Pin GPIO_PIN_0
#define MODE_GPIO_Port GPIOC
#define MODEC1_Pin GPIO_PIN_1
#define MODEC1_GPIO_Port GPIOC
#define MODEC2_Pin GPIO_PIN_2
#define MODEC2_GPIO_Port GPIOC
#define MODEC3_Pin GPIO_PIN_3
#define MODEC3_GPIO_Port GPIOC
#define ADC_POT_Pin GPIO_PIN_4
#define ADC_POT_GPIO_Port GPIOA
#define MODEC4_Pin GPIO_PIN_4
#define MODEC4_GPIO_Port GPIOC
#define MODEC5_Pin GPIO_PIN_5
#define MODEC5_GPIO_Port GPIOC
#define LIM_Pin GPIO_PIN_0
#define LIM_GPIO_Port GPIOB
#define LIMB1_Pin GPIO_PIN_1
#define LIMB1_GPIO_Port GPIOB
#define LIMB2_Pin GPIO_PIN_2
#define LIMB2_GPIO_Port GPIOB
#define JOY_Pin GPIO_PIN_0
#define JOY_GPIO_Port GPIOG
#define JOYG1_Pin GPIO_PIN_1
#define JOYG1_GPIO_Port GPIOG
#define X_ENA_Pin GPIO_PIN_7
#define X_ENA_GPIO_Port GPIOE
#define Z_STEP_Pin GPIO_PIN_9
#define Z_STEP_GPIO_Port GPIOE
#define Z_Dir_Pin GPIO_PIN_10
#define Z_Dir_GPIO_Port GPIOE
#define X_STEP_Pin GPIO_PIN_11
#define X_STEP_GPIO_Port GPIOE
#define I2C2_SCL_Pin GPIO_PIN_10
#define I2C2_SCL_GPIO_Port GPIOB
#define I2C2_SDA_Pin GPIO_PIN_11
#define I2C2_SDA_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_12
#define LED_GPIO_Port GPIOB
#define Spindle_A_Pin GPIO_PIN_12
#define Spindle_A_GPIO_Port GPIOD
#define Spindle_B_Pin GPIO_PIN_13
#define Spindle_B_GPIO_Port GPIOD
#define DIVIDER_OUT_Pin GPIO_PIN_14
#define DIVIDER_OUT_GPIO_Port GPIOD
#define BUZZER_Pin GPIO_PIN_15
#define BUZZER_GPIO_Port GPIOD
#define JOYG2_Pin GPIO_PIN_2
#define JOYG2_GPIO_Port GPIOG
#define JOYG3_Pin GPIO_PIN_3
#define JOYG3_GPIO_Port GPIOG
#define RAPID_Pin GPIO_PIN_4
#define RAPID_GPIO_Port GPIOG
#define SUBMODE_Pin GPIO_PIN_5
#define SUBMODE_GPIO_Port GPIOG
#define SUBMODEG6_Pin GPIO_PIN_6
#define SUBMODEG6_GPIO_Port GPIOG
#define SUBMODEG7_Pin GPIO_PIN_7
#define SUBMODEG7_GPIO_Port GPIOG
#define Hand_A_Pin GPIO_PIN_6
#define Hand_A_GPIO_Port GPIOC
#define Hand_B_Pin GPIO_PIN_7
#define Hand_B_GPIO_Port GPIOC
#define USART1_TX_Pin GPIO_PIN_9
#define USART1_TX_GPIO_Port GPIOA
#define USART1_RX_Pin GPIO_PIN_10
#define USART1_RX_GPIO_Port GPIOA
#define LIMB6_Pin GPIO_PIN_6
#define LIMB6_GPIO_Port GPIOB
#define LEDB7_Pin GPIO_PIN_7
#define LEDB7_GPIO_Port GPIOB
#define LEDB8_Pin GPIO_PIN_8
#define LEDB8_GPIO_Port GPIOB
#define LEDB9_Pin GPIO_PIN_9
#define LEDB9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
