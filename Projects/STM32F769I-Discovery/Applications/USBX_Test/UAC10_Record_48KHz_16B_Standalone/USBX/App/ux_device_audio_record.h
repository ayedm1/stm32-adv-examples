/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio_record.h
  * @author  MCD Application Team
  * @brief   USBX Device audio recording applicative header file
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
#ifndef __UX_DEVICE_AUDIO_RECORD_H__
#define __UX_DEVICE_AUDIO_RECORD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_audio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "audio_user.h"
#include "ux_device_descriptors.h"
#include "stm32f769i_discovery_audio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define AUDIO_MIC_SCRATCH_SIZE  (AUDIO_SAMPLE_COUNT_LENGTH(USBD_AUDIO_RECORD_DEFAULT_FREQ))* \
                                (USBD_AUDIO_RECORD_CHANNEL_COUNT) * \
                                 USBD_AUDIO_RECORD_RES_BYTE
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
VOID USBD_AUDIO_RecordingStreamChange(UX_DEVICE_CLASS_AUDIO_STREAM *audio_record_stream,
                                     ULONG alternate_setting);
VOID USBD_AUDIO_RecordingStreamFrameDone(UX_DEVICE_CLASS_AUDIO_STREAM *audio_record_stream,
                                        ULONG length);
ULONG USBD_AUDIO_RecordingStreamGetMaxFrameBufferNumber(VOID);
ULONG USBD_AUDIO_RecordingStreamGetMaxFrameBufferSize(VOID);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* USER CODE BEGIN 2 */


typedef enum
{
  AUDIO_RECORDING_OFF,
  AUDIO_RECORDING_INITIALIZED,
  AUDIO_RECORDING_STARTED,
  AUDIO_RECORDING_STOPPED,
  AUDIO_RECORDING_ERROR
}AUDIO_RECORDING_STATE;

typedef enum
{
  AUDIO_MICROPHONE_OFF,
  AUDIO_MICROPHONE_INITIALIZED,
  AUDIO_MICROPHONE_STARTED,
  AUDIO_MICROPHONE_STOPPED,
  AUDIO_MICROPHONE_ERROR
}AUDIO_MICROPHONE_STATE;


typedef struct AUDIO_MICROPHPNE_PARMETER_STRUCT
{
  uint16_t usb_packet_length; /* Packet maximal length */
  uint16_t mic_packet_length; /* Packet maximal length */
  int32_t scratch[AUDIO_MIC_SCRATCH_SIZE];
  uint8_t* alt_buff;            /* Zero filled buffer , to send to the host when required data not ready */
  uint16_t max_packet_length;   /* Packet to read each time from buffer */
  uint16_t writing_step;
  uint32_t packet_sample_count;
  uint8_t packet_sample_size;
  uint8_t pcm_used;     /* begin of play */
  uint8_t cmd;          /* cmd to execute in interruption routine */
}AUDIO_MICROPHPNE_PARMETER;

UINT USBD_AUDIO_RecordingInit(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer);
UINT USBD_AUDIO_RecordingInitializeDataBuffer(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer);

UINT AUDIO_MicrophoneInit(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer, AUDIO_MICROPHPNE_PARMETER *microphone_specific);
UINT AUDIO_MicrophoneStart(VOID);
UCHAR AUDIO_IN_Get_PcmBuffer(uint8_t* pbuf, uint16_t sample_count, uint16_t ScratchOffset, uint8_t res);

/* USER CODE END 2 */

#ifdef __cplusplus
}
#endif
#endif  /* __UX_DEVICE_AUDIO_RECORD_H__ */
