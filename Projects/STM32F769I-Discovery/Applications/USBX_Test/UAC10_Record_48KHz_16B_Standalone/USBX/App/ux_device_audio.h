/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio.h
  * @author  MCD Application Team
  * @brief   USBX Device Audio applicative header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __UX_DEVICE_AUDIO_H__
#define __UX_DEVICE_AUDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_audio.h"
#include "ux_device_class_audio10.h"
#include "ux_device_audio_record.h"

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
#define USBD_AUDIO_MICROPHONE_MUTED                  0
#define USBD_AUDIO_VOLUME_MICROPHONE_MIN_DB_256      -8192 /* -32db == -32*256 = -8192 db */
#define USBD_AUDIO_VOLUME_MICROPHONE_MAX_DB_256      8192  /* 32db == 32*256 = 8192 db */
#define USBD_AUDIO_VOLUME_MICROPHONE_RES_DB_256      1     /* 1db == 1*256 = 256 db */
#define USBD_AUDIO_VOLUME_MICROPHONE_DEFAULT_DB_256  0     /* 0db */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
VOID USBD_AUDIO_Activate(VOID *audio_instance);
VOID USBD_AUDIO_Deactivate(VOID *audio_instance);
UINT USBD_AUDIO_ControlProcess(UX_DEVICE_CLASS_AUDIO *audio_instance,
                               UX_SLAVE_TRANSFER *transfer);
VOID USBD_AUDIO_SetControlValues(VOID);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

#ifndef USBD_AUDIO_STREAM_NMNBER
#define USBD_AUDIO_STREAM_NMNBER  1
#endif

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

#ifdef __cplusplus
}
#endif
#endif  /* __UX_DEVICE_AUDIO_H__ */
