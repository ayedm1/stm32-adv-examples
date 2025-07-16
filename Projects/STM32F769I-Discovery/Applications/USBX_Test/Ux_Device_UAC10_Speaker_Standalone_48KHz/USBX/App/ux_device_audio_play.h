/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio_play.h
  * @author  MCD Application Team
  * @brief   USBX Device audio plyaback applicative header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_audio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "stm32f769i_discovery_audio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define AUDIO_IO_BEGIN_OF_STREAM          0x01 /* Begin of stream sent to session when first packet is received */
#define AUDIO_IO_RESTART_REQUIRED         0x40 /* Restart of USB node is required , after frequency changes for examples */
#define AUDIO_IO_THRESHOLD_REACHED        0x08 /* This flag is set when  main circular audio  buffer fill threshold is reached.
                                                  Then consumer node starts  reading from the buffer, this is to avoid overrun and underrun in the begin of streaming*/

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define VOLUME_DB_256_TO_PERCENT(volume_db_256) ((uint8_t)((((INT)(volume_db_256) - USBD_AUDIO_VOLUME_SPEAKER_MIN_DB_256)*100)/ \
                                                                  (USBD_AUDIO_VOLUME_SPEAKER_MAX_DB_256 - USBD_AUDIO_VOLUME_SPEAKER_MIN_DB_256)))
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
VOID USBD_AUDIO_PlaybackStreamChange(UX_DEVICE_CLASS_AUDIO_STREAM *audio_play_stream,
                                     ULONG alternate_setting);
VOID USBD_AUDIO_PlaybackStreamFrameDone(UX_DEVICE_CLASS_AUDIO_STREAM *audio_play_stream,
                                        ULONG length);
ULONG USBD_AUDIO_PlaybackStreamGetMaxFrameBufferNumber(VOID);
ULONG USBD_AUDIO_PlaybackStreamGetMaxFrameBufferSize(VOID);
/* USER CODE BEGIN EFP */
UINT USBD_AUDIO_PlaybackInit(VOID);
VOID USBD_AUDIO_PlaybackInitializeDataBuffer(VOID *buffer, ULONG buffer_size,
                                             USHORT packet_size, USHORT margin);

UINT AUDIO_SpeakerInit(VOID);
UINT AUDIO_SpeakerDeInit(VOID);
UINT AUDIO_SpeakerStart(VOID);
UINT AUDIO_SpeakerStop(VOID);
UINT AUDIO_SpeakerChangeFrequency(VOID);
UINT AUDIO_SpeakerMute(UCHAR mute_state);
UINT AUDIO_SpeakerSetVolume(INT volume_db_256);
VOID AUDIO_SpeakerInitInjectionsParams(VOID *specific_params);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* List of session states */
typedef enum
{
  AUDIO_PLAYBACK_OFF,
  AUDIO_PLAYBACK_INITIALIZED,
  AUDIO_PLAYBACK_STARTED,
  AUDIO_PLAYBACK_STOPPED,
  AUDIO_PLAYBACK_ERROR
}AUDIO_PlaybackStateTypeDef;

/* Audio states list */
typedef enum
{
  AUDIO_SPEAKER_OFF,         /* node not initialized */
  AUDIO_SPEAKER_INITIALIZED,
  AUDIO_SPEAKER_STARTED,     /* node is running */
  AUDIO_SPEAKER_STOPPED,
  AUDIO_SPEAKER_ERROR
}AUDIO_SpeakerStateTypeDef;

typedef struct
{
  uint16_t packet_length;     /* packet maximal length */
  uint16_t injection_size;    /* nominal size of the unit packet sent using DMA to SAI */
  uint8_t *data;              /* pointer to data, which is going to transmit using DMA to SAI */
  uint16_t data_size;         /* size of data, which is going to transmit using DMA to SAI */
  uint8_t* alt_buffer;        /* alternative buffer used  when underrun is produced(no enough data to inject) or when padding should be added */
  uint16_t alt_buf_half_size; /* half size of the alternative buffer */
  uint8_t double_buff;        /* when the padding is needed the double buffering are required. It means that the alt_buff will contain two packet */
  uint8_t offset ;            /* binary flag. used to indicate if next packet is in the first half of alternate buffer or in the second half */
  uint16_t dma_remaining;     /* used for synchronization, it helps to provide the counter of played samples */
  uint16_t input_threshold;   /* After starting playback , usb input node starts receiving packet and writing them in the audio circular buffer.
                                 when written data size reaches this threshold it raises an event to playback session*/
}AUDIO_SpeakerSpecificParmsTypedef;

/* USER CODE END PD */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif  /* __UX_DEVICE_AUDIO_PLAY_H__ */
