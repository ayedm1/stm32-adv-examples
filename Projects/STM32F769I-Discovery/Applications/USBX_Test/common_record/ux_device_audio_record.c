/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio_record.c
  * @author  MCD Application Team
  * @brief   USBX Device Video applicative source file
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

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Includes ------------------------------------------------------------------*/
#include "ux_device_audio_record.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  USBD_AUDIO_RecordingStreamChange
  *         This function is invoked to inform application that the
  *         alternate setting are changed.
  * @param  audio_record_stream: Pointer to audio recording class stream instance.
  * @param  alternate_setting: interface alternate setting.
  * @retval none
  */
VOID USBD_AUDIO_RecordingStreamChange(UX_DEVICE_CLASS_AUDIO_STREAM *audio_record_stream,
                                      ULONG alternate_setting)
{
  /* USER CODE BEGIN USBD_AUDIO_RecordingStreamChange */
  UX_PARAMETER_NOT_USED(audio_record_stream);
  UX_PARAMETER_NOT_USED(alternate_setting);
  /* USER CODE END USBD_AUDIO_RecordingStreamChange */

  return;
}

/**
  * @brief  USBD_AUDIO_RecordingStreamFrameDone
  *         This function is invoked whenever a USB packet (audio frame) is received
  *         from the host.
  * @param  audio_record_stream: Pointer to audio recodring class stream instance.
  * @param  length: transfer length.
  * @retval none
  */
VOID USBD_AUDIO_RecordingStreamFrameDone(UX_DEVICE_CLASS_AUDIO_STREAM *audio_record_stream,
                                        ULONG length)
{
  /* USER CODE BEGIN USBD_AUDIO_RecordingStreamFrameDone */
  UX_PARAMETER_NOT_USED(audio_record_stream);
  UX_PARAMETER_NOT_USED(length);
  /* USER CODE END USBD_AUDIO_RecordingStreamFrameDone */

  return;
}

/**
  * @brief  USBD_AUDIO_RecordingStreamGetMaxFrameBufferNumber
  *         This function is invoked to Set audio recording stream max Frame buffer number.
  * @param  none
  * @retval max frame buffer number
  */
ULONG USBD_AUDIO_RecordingStreamGetMaxFrameBufferNumber(VOID)
{
  ULONG max_frame_buffer_number = 0U;

  /* USER CODE BEGIN USBD_AUDIO_RecordingStreamGetMaxFrameBufferNumber */

  /* USER CODE END USBD_AUDIO_RecordingStreamGetMaxFrameBufferNumber */

  return max_frame_buffer_number;
}

/**
  * @brief  USBD_AUDIO_RecordingStreamGetMaxFrameBufferSize
  *         This function is invoked to Set audio recording stream max Frame buffer size.
  * @param  none
  * @retval max frame buffer size
  */
ULONG USBD_AUDIO_RecordingStreamGetMaxFrameBufferSize(VOID)
{
  ULONG max_frame_buffer_size = 0U;

  /* USER CODE BEGIN USBD_AUDIO_RecordingStreamGetMaxFrameBufferSize */

  /* USER CODE END USBD_AUDIO_RecordingStreamGetMaxFrameBufferSize */

  return max_frame_buffer_size;
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
