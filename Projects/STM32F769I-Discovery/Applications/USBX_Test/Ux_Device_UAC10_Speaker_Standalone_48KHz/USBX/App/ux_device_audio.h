/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio.h
  * @author  MCD Application Team
  * @brief   USBX Device Audio applicative header file
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
#ifndef __UX_DEVICE_AUDIO_H__
#define __UX_DEVICE_AUDIO_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_audio.h"
#include "ux_device_class_audio20.h"
#include "ux_device_audio_play.h"
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
#define USBD_AUDIO_SPEAKER_MUTED                  0
#define USBD_AUDIO_VOLUME_SPEAKER_MIN_DB_256      -6400 /* -25db == -25*256 = -6400 db */
#define USBD_AUDIO_VOLUME_SPEAKER_MAX_DB_256      1536  /* 6db == 6*256 = 1536 db */
#define USBD_AUDIO_VOLUME_SPEAKER_RES_DB_256      1     /* 1db == 1*256 = 256 db */
#define USBD_AUDIO_VOLUME_SPEAKER_DEFAULT_DB_256  256   /* 1db */

/*  USBD_AUDIO_BUFFER_FREE_SIZE computes the free size in the circular buffer */
#define USBD_AUDIO_BUFFER_FREE_SIZE(buff)  ((buff.wr_ptr >= buff.rd_ptr)?(buff.rd_ptr + buff.size - buff.wr_ptr) : \
                                                                         (buff.rd_ptr - buff.wr_ptr))

/* USBD_AUDIO_BUFFER_FILLED_SIZE computes the filled size in the circular buffer */
#define USBD_AUDIO_BUFFER_FILLED_SIZE(buff) ((buff.wr_ptr >= buff.rd_ptr)?(buff.wr_ptr - buff.rd_ptr) : \
                                                                          (buff.wr_ptr + buff.size - buff.rd_ptr))


/* AUDIO_USB_PACKET_SIZE computes the nominal size of audio packet in HS mode(it uses the floor of frequency fractional part) */
#define USBD_AUDIO_USB_PACKET_SIZE(freq,channel_count,res_byte) (((uint32_t)((freq) /8000))* (channel_count) * (res_byte))

/* AUDIO_USB_MAX_PACKET_SIZE computes the nominal size of audio packet in HS mode(it uses the ceil of frequecy fractional part) */
#define USBD_AUDIO_USB_MAX_PACKET_SIZE(freq,channel_count,res_byte) USBD_AUDIO_USB_PACKET_SIZE(freq+7999,channel_count,res_byte)

#define USBD_AUDIO_MAX_PACKET_WITH_FEEDBACK_LENGTH(audio_desc) USBD_AUDIO_USB_MAX_PACKET_SIZE(audio_desc.audio_frequency + 1, \
                                                                                              audio_desc.audio_channels_count,\
                                                                                              audio_desc.audio_resolution)

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

/* Properties of audio stream */
typedef struct
{
  ULONG audio_frequency;      /* Audio frequency */
  CHAR  audio_channels_count; /* Audio channels count */
  SHORT audio_volume_db_256;  /* Audio volume */
  CHAR  audio_mute;           /* Audio mute state */
  CHAR  audio_resolution;     /* Audio resolution */
}AUDIO_DescriptionTypeDef;


/* Main audio buffer structure */
typedef struct
{
  UCHAR* data;   /* Pointer to circular buffer data */
  USHORT rd_ptr; /* Circular buffer reading offset */
  USHORT wr_ptr; /* Circular buffer writing offset */
  USHORT size;   /* Size of buffer segment where samples may be read or written */
}AUDIO_CircularBufferTypeDef;


/* USER CODE END PD */

#ifndef USBD_AUDIO_STREAM_NMNBER
#define USBD_AUDIO_STREAM_NMNBER  1
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
#ifdef __cplusplus
}
#endif
#endif  /* __UX_DEVICE_AUDIO_H__ */
