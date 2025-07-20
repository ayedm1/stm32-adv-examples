/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio_play.h
  * @author  MCD Application Team
  * @brief   USBX Device audio plyaback applicative header file
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
#ifndef __UX_DEVICE_AUDIO_PLAY_H__
#define __UX_DEVICE_AUDIO_PLAY_H__

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
#include "stm32f769i_discovery_audio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define USBD_AUDIO_SPEAKER_MUTED                  0
#define USBD_AUDIO_VOLUME_SPEAKER_MIN_DB_256      -6400 /* -25db == -25*256 = -6400 db */
#define USBD_AUDIO_VOLUME_SPEAKER_MAX_DB_256      1536  /* 6db == 6*256 = 1536 db */
#define USBD_AUDIO_VOLUME_SPEAKER_RES_DB_256      1     /* 1db == 1*256 = 256 db */
#define USBD_AUDIO_VOLUME_SPEAKER_DEFAULT_DB_256  256   /* 1db */

#define SPEAKER_CMD_STOP                1
#define SPEAKER_CMD_EXIT                2
#define SPEAKER_CMD_CHANGE_FREQUENCE    4

#define USBD_AUDIO_BEGIN_OF_STREAM              0x01
#define USBD_AUDIO_RESTART_REQUIRED             0x40
#define USBD_AUDIO_THRESHOLD_REACHED            0x08
#define USBD_AUDIO_RESTART_REQUIRED             0x40


#define DEBUG_AUDIO_SPEAKER   /* Define when debug is required */
#define DEBUG_AUDIO_USB       /* Define when debug USB input for playback */

#ifdef DEBUG_AUDIO_SPEAKER
#define SPEAKER_DEBUG_BUFFER_SIZE               1000
#endif /* DEBUG_AUDIO_SPEAKER */

#ifdef DEBUG_AUDIO_USB
#define USB_INPUT_NODE_DEBUG_BUFFER_SIZE        1000
#endif /* DEBUG_AUDIO_USB */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
VOID USBD_AUDIO_PlaybackStreamChange(UX_DEVICE_CLASS_AUDIO_STREAM *audio_play_stream,
                                     ULONG alternate_setting);
VOID USBD_AUDIO_PlaybackStreamFrameDone(UX_DEVICE_CLASS_AUDIO_STREAM *audio_play_stream,
                                        ULONG length);
ULONG USBD_AUDIO_PlaybackStreamGetMaxFrameBufferNumber(VOID);
ULONG USBD_AUDIO_PlaybackStreamGetMaxFrameBufferSize(VOID);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* USER CODE BEGIN 2 */

typedef struct AUDIO_SPEAKER_PARMETER_STRUCT
{
  __IO uint8_t cmd;
  uint16_t      packet_length;
  uint16_t      injection_size;
  uint8_t       *data;
  uint16_t      data_size;
  uint8_t*      alt_buffer;
  uint16_t      alt_buf_half_size; /* half size of the alternative buffer */
  uint8_t       double_buff;        /* when the padding is needed the double buffering are required. It means that the alt_buff will contain two packet */
  uint8_t       offset ;            /* binary flag. used to indicate if next packet is in the first half of alternate buffer or in the second half */
  uint16_t      dma_remaining;     /* used for synchronization, it helps to provide the counter of played samples */
  uint16_t      input_threshold;
  uint8_t       flags;
}AUDIO_SPEAKER_PARMETER;

#ifdef DEBUG_AUDIO_SPEAKER
typedef struct
{
  uint32_t      time;
  uint16_t      injection_size;
  uint16_t      read;
  uint16_t      dump;
  uint8_t*      data;
} AUDIO_SPEAKER_BUFFER_STATS;
#endif /* DEBUG_AUDIO_SPEAKER */

#ifdef DEBUG_AUDIO_USB
typedef struct
{
  uint32_t      time;
  uint16_t      write;
  uint16_t      read;
  uint16_t      error;
} AUDIO_USB_INPUT_BUFFER_DEBUG_STATS;
#endif /* DEBUG_AUDIO_USB */

typedef enum
{
  AUDIO_PLAYBACK_OFF,
  AUDIO_PLAYBACK_INITIALIZED,
  AUDIO_PLAYBACK_STARTED,
  AUDIO_PLAYBACK_STOPPED,
  AUDIO_PLAYBACK_ERROR
}AUDIO_PLAYBACK_STATE;

typedef enum
{
  AUDIO_SPEAKER_OFF,
  AUDIO_SPEAKER_INITIALIZED,
  AUDIO_SPEAKER_STARTED,
  AUDIO_SPEAKER_STOPPED,
  AUDIO_SPEAKER_ERROR
}AUDIO_SPEAKER_STATE;

VOID USBD_AUDIO_PlaybackStreamChange(UX_DEVICE_CLASS_AUDIO_STREAM *audio_play_stream,
                                     ULONG alternate_setting);
VOID USBD_AUDIO_PlaybackStreamFrameDone(UX_DEVICE_CLASS_AUDIO_STREAM *audio_play_stream,
                                        ULONG length);
ULONG USBD_AUDIO_PlaybackStreamGetMaxFrameBufferNumber(VOID);
ULONG USBD_AUDIO_PlaybackStreamGetMaxFrameBufferSize(VOID);
/* USER CODE BEGIN EFP */

UINT USBD_AUDIO_PlaybackInit(AUDIO_DESCRIPTION *audio_speaker_desc, AUDIO_CIRCULAR_BUFFER *audio_speaker_buffer);
UINT USBD_AUDIO_PlaybackInitializeDataBuffer(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer);


UINT AUDIO_SpeakerInit(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer, AUDIO_SPEAKER_PARMETER *speaker_specific);
UINT AUDIO_SpeakerDeInit(VOID);
UINT AUDIO_SpeakerStart(AUDIO_DESCRIPTION audio_desc);
UINT AUDIO_SpeakerStop(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer, AUDIO_SPEAKER_PARMETER *speaker_specific);
UINT AUDIO_SpeakerChangeFrequency(VOID);
UINT AUDIO_SpeakerMute(AUDIO_DESCRIPTION audio_desc);
UINT AUDIO_SpeakerSetVolume(AUDIO_DESCRIPTION audio_desc);
/* USER CODE END 2 */

#ifdef __cplusplus
}
#endif
#endif  /* __UX_DEVICE_AUDIO_PLAY_H__ */
