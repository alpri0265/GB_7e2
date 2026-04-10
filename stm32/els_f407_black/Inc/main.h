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
#define X_ENA_Pin GPIO_PIN_7
#define X_ENA_GPIO_Port GPIOE
#define Z_ENA_Pin GPIO_PIN_8
#define Z_ENA_GPIO_Port GPIOE
/* PE9 = TIM1_CH1 = Z_STEP, PE11 = TIM1_CH2 = X_STEP (див. STM32F407_PORT.md, п. 2.6) */
#define Z_Step_Pin GPIO_PIN_9
#define Z_Step_GPIO_Port GPIOE
#define Z_DIR_Pin GPIO_PIN_10
#define Z_DIR_GPIO_Port GPIOE
#define X_Step_Pin GPIO_PIN_11
#define X_Step_GPIO_Port GPIOE
#define X_DIR_Pin GPIO_PIN_12
#define X_DIR_GPIO_Port GPIOE
#define I2C2_SCL_Pin GPIO_PIN_10
#define I2C2_SCL_GPIO_Port GPIOB
#define I2C2_SDA_Pin GPIO_PIN_11
#define I2C2_SDA_GPIO_Port GPIOB
#define Limit_LEDs_Pin GPIO_PIN_12
#define Limit_LEDs_GPIO_Port GPIOB
#define TACHO_OUT_Pin GPIO_PIN_13
#define TACHO_OUT_GPIO_Port GPIOG
#define Limit_LEDsB7_Pin GPIO_PIN_7
#define Limit_LEDsB7_GPIO_Port GPIOB
#define Limit_LEDsB8_Pin GPIO_PIN_8
#define Limit_LEDsB8_GPIO_Port GPIOB
#define Limit_LEDsB9_Pin GPIO_PIN_9
#define Limit_LEDsB9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
