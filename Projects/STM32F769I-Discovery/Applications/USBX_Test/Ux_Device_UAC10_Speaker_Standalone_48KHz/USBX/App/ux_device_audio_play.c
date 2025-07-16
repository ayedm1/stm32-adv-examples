/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio_play.c
  * @author  MCD Application Team
  * @brief   USBX Device Video applicative source file
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

/* Includes ------------------------------------------------------------------*/
#include "ux_device_audio_play.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ux_device_audio.h"
#include "ux_device_descriptors.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define USBD_AUDIO_PLAYBACK_BUFFER_SIZE (10*1024)
#define SPEAKER_ALT_BUFFER_SIZE         ((USBD_AUDIO_PLAY_DEFAULT_FREQ+999)/1000)*2*3*2
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
AUDIO_CircularBufferTypeDef speaker_buffer;
AUDIO_PlaybackStateTypeDef  playback_state;
AUDIO_SpeakerStateTypeDef   speaker_state;
AUDIO_SpeakerSpecificParmsTypedef speaker_specific;

AUDIO_DescriptionTypeDef PlaybackAudioDescription;

extern UX_DEVICE_CLASS_AUDIO_STREAM *playback_stream;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  USBD_AUDIO_PlaybackStreamChange
  *         This function is invoked to inform application that the
  *         alternate setting are changed.
  * @param  audio_play_stream: Pointer to audio playback class stream instance.
  * @param  alternate_setting: interface alternate setting.
  * @retval none
  */
VOID USBD_AUDIO_PlaybackStreamChange(UX_DEVICE_CLASS_AUDIO_STREAM *audio_play_stream,
                                     ULONG alternate_setting)
{
  /* USER CODE BEGIN USBD_AUDIO_PlaybackStreamChange */

  /* Do nothing if alternate setting is 0 (stream closed) */
  if (alternate_setting == 0)
  {
    return;
  }

  AUDIO_SpeakerStart();

  /* Start reception (stream opened) */
  ux_device_class_audio_reception_start(audio_play_stream);

  /* USER CODE END USBD_AUDIO_PlaybackStreamChange */

  return;
}

/**
  * @brief  USBD_AUDIO_PlaybackStreamFrameDone
  *         This function is invoked whenever a USB packet (audio frame) is received
  *         from the host.
  * @param  audio_play_stream: Pointer to audio playback class stream instance.
  * @param  length: transfer length.
  * @retval none
  */
VOID USBD_AUDIO_PlaybackStreamFrameDone(UX_DEVICE_CLASS_AUDIO_STREAM *audio_play_stream,
                                        ULONG length)
{
  /* USER CODE BEGIN USBD_AUDIO_PlaybackStreamFrameDone */
  UX_PARAMETER_NOT_USED(length);
  UCHAR *frame_buffer;
  ULONG frame_length;

  /* Get access to first audio input frame */
  if (ux_device_class_audio_read_frame_get(audio_play_stream, &frame_buffer, &frame_length) == UX_SUCCESS)
  {

    /* Check if write pointer equal to buffer size */
    if (speaker_buffer.wr_ptr >= speaker_buffer.size)
    {
      /* Reset speaker buffer write pointer */
      speaker_buffer.wr_ptr = 0;
    }

    /* Copy audio frame in speaker buffer */
    ux_utility_memory_copy(speaker_buffer.data + speaker_buffer.wr_ptr,
                           frame_buffer,
                           frame_length);

    /* Increment speaker buffer write pointer */
    speaker_buffer.wr_ptr += frame_length;
  }

  /* Re-free the first audio input frame for transfer.  */
  ux_device_class_audio_read_frame_free(audio_play_stream);

  /* USER CODE END USBD_AUDIO_PlaybackStreamFrameDone */

  return;
}

/**
  * @brief  USBD_AUDIO_PlaybackStreamGetMaxFrameBufferNumber
  *         This function is invoked to Set audio playback stream max Frame buffer number.
  * @param  none
  * @retval max frame buffer number
  */
ULONG USBD_AUDIO_PlaybackStreamGetMaxFrameBufferNumber(VOID)
{
  ULONG max_frame_buffer_number = 0U;

  /* USER CODE BEGIN USBD_AUDIO_PlaybackStreamGetMaxFrameBufferNumber */
  max_frame_buffer_number = 3U;
  /* USER CODE END USBD_AUDIO_PlaybackStreamGetMaxFrameBufferNumber */

  return max_frame_buffer_number;
}

/**
  * @brief  USBD_AUDIO_PlaybackStreamGetMaxFrameBufferSize
  *         This function is invoked to Set audio playback stream max Frame buffer size.
  * @param  none
  * @retval max frame buffer size
  */
ULONG USBD_AUDIO_PlaybackStreamGetMaxFrameBufferSize(VOID)
{
  ULONG max_frame_buffer_size = 0U;

  /* USER CODE BEGIN USBD_AUDIO_PlaybackStreamGetMaxFrameBufferSize */
  max_frame_buffer_size = USBD_AUDIO_PLAY_EPOUT_FS_MPS;
  /* USER CODE END USBD_AUDIO_PlaybackStreamGetMaxFrameBufferSize */

  return max_frame_buffer_size;
}

/* USER CODE BEGIN 1 */

/**
  * @brief  USBD_AUDIO_PlaybackInit
  *         Initializes the playback.
  * @param  none
  * @retval status
  */
UINT USBD_AUDIO_PlaybackInit(VOID)
{
  uint16_t buffer_margin;
  uint16_t max_packet_length;
  uint16_t packet_length;
  uint16_t packet_size;

  PlaybackAudioDescription.audio_frequency = USBD_AUDIO_PLAY_DEFAULT_FREQ;
  PlaybackAudioDescription.audio_channels_count = USBD_AUDIO_PLAY_CHANNEL_COUNT;
  PlaybackAudioDescription.audio_resolution = USBD_AUDIO_PLAY_RES_BYTE;

  /* Init speaker buffer */
  speaker_buffer.size = USBD_AUDIO_PLAYBACK_BUFFER_SIZE;

  /* Allocate Playback data buffer */
  speaker_buffer.data = ux_utility_memory_allocate(UX_SAFE_ALIGN, UX_CACHE_SAFE_MEMORY,
                                                   USBD_AUDIO_PLAYBACK_BUFFER_SIZE);

  /* Check Buffer allocated */
  if (speaker_buffer.data == UX_NULL)
  {
    while(1){}
  }

  ux_utility_memory_set(speaker_buffer.data, 0, USBD_AUDIO_PLAYBACK_BUFFER_SIZE);

  /* Initializes the audio speaker */
  AUDIO_SpeakerInit();


  packet_length = ((uint32_t)((PlaybackAudioDescription.audio_frequency)/1000)) * \
                              (PlaybackAudioDescription.audio_channels_count)   * \
                              (PlaybackAudioDescription.audio_resolution);

#ifdef UX_DEVICE_CLASS_AUDIO_FEEDBACK_SUPPORT
  max_packet_length =((uint32_t)((PlaybackAudioDescription.audio_frequency+999)/1000)) * \
                                 (PlaybackAudioDescription.audio_channels_count)     * \
                                 (PlaybackAudioDescription.audio_resolution);
#else /* UX_DEVICE_CLASS_AUDIO_FEEDBACK_SUPPORT */
  max_packet_length =((uint32_t)((PlaybackAudioDescription.audio_frequency+1000)/1000)) * \
                                 (PlaybackAudioDescription.audio_channels_count)     * \
                                 (PlaybackAudioDescription.audio_resolution);
#endif /* UX_DEVICE_CLASS_AUDIO_FEEDBACK_SUPPORT */



  buffer_margin = (max_packet_length > packet_length) ? max_packet_length : 0;

  packet_size = ((uint32_t)((PlaybackAudioDescription.audio_frequency)/1000)) * \
                            (PlaybackAudioDescription.audio_channels_count) * \
                            (PlaybackAudioDescription.audio_resolution);

  USBD_AUDIO_PlaybackInitializeDataBuffer(&speaker_buffer,
                                          USBD_AUDIO_PLAYBACK_BUFFER_SIZE,
                                          packet_size,
                                          buffer_margin);

  playback_state = AUDIO_PLAYBACK_INITIALIZED;

  return UX_SUCCESS;
}

/**
  * @brief  USBD_AUDIO_PlaybackInitializeDataBuffer
  *         The circular buffer has the total size of buffer_size. this size is divided to two : the regular size and the margin.
  *         Margin is located at the tail of the circular buffer. Margin is used as some packet have regular size+/-1 sample.
  * @param  buf: main circular buffer.
  * @param  buffer_size: whole buffer size when allocated.
  * @param  packet_size:USB Audio packet size.
  * @param  margin: protection area size.
  * @retval none
  */
VOID USBD_AUDIO_PlaybackInitializeDataBuffer(VOID *buffer, ULONG buffer_size,
                                             USHORT packet_size, USHORT margin)
{
  AUDIO_CircularBufferTypeDef *speaker_buffer;

  speaker_buffer = (AUDIO_CircularBufferTypeDef *) buffer;

  speaker_buffer->size = ((int)((buffer_size - margin) / packet_size)) * packet_size;

  speaker_buffer->rd_ptr = 0;
  speaker_buffer->wr_ptr = 0;
}

/**
  * @brief  AUDIO_SpeakerInit
  *         Initializes the audio speaker node, set callbacks and start the codec. As no data are ready. The
  *         SAI is feeded from the alternate buffer (filled by zeros)
  * @param  none
  * @retval status
  */
UINT AUDIO_SpeakerInit(VOID)
{

  /* Allocate memory for playback alternate buffer */
  speaker_specific.alt_buffer = ux_utility_memory_allocate(UX_SAFE_ALIGN,
                                                           UX_CACHE_SAFE_MEMORY,
                                                           SPEAKER_ALT_BUFFER_SIZE);

  /* Check if memory allocation succefuly */
  if (speaker_specific.alt_buffer == UX_NULL)
  {
    Error_Handler();
  }

  AUDIO_SpeakerInitInjectionsParams(&speaker_specific);

  /* Configures the audio peripherals */
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70,
                     PlaybackAudioDescription.audio_frequency,
                     (PlaybackAudioDescription.audio_resolution*8));

  /* DMA stream from output double buffer to codec in Circular mode launch */
  BSP_AUDIO_OUT_Play((uint16_t*) speaker_specific.data, speaker_specific.data_size);

  speaker_state = AUDIO_SPEAKER_INITIALIZED;

  return UX_SUCCESS;
}

/**
  * @brief  AUDIO_SpeakerDeInit
  *         De-Initializes the audio speaker node
  * @param  none
  * @retval status
  */
UINT AUDIO_SpeakerDeInit(VOID)
{
  if (speaker_state != AUDIO_SPEAKER_OFF)
  {
    /* Stops audio playing and Power down the Audio Codec */
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);

    /* De-initializes the audio out peripheral */
    BSP_AUDIO_OUT_DeInit();

    /* update speaker state */
    speaker_state = AUDIO_SPEAKER_OFF;
  }

  return UX_SUCCESS;
}

/**
  * @brief  AUDIO_SpeakerInitInjectionsParams
  *         audio speaker injection parameter Initialize.
  * @param  none
  * @retval none
  */
VOID AUDIO_SpeakerInitInjectionsParams(VOID *specific_params)
{

  AUDIO_SpeakerSpecificParmsTypedef *specific = (AUDIO_SpeakerSpecificParmsTypedef *)specific_params;

  specific->packet_length = ((PlaybackAudioDescription.audio_frequency)/1000) * \
                             (PlaybackAudioDescription.audio_channels_count) *  \
                             (PlaybackAudioDescription.audio_resolution);

  specific->injection_size = ((PlaybackAudioDescription.audio_frequency)/1000) * \
                              (PlaybackAudioDescription.audio_channels_count) *  \
                              (PlaybackAudioDescription.audio_resolution);

  specific->double_buff = 0U;
  specific->offset = 0U;

  ux_utility_memory_set(specific->alt_buffer, 0, specific->injection_size);

  /* Start injection of dumped data */
  specific->data = specific->alt_buffer;
  specific->data_size = specific->injection_size;
}

/**
  * @brief  AUDIO_SpeakerStart
  *         Start the audio speaker.
  * @param  none
  * @retval status
  */
UINT AUDIO_SpeakerStart(VOID)
{

  AUDIO_SpeakerMute(PlaybackAudioDescription.audio_mute);
  AUDIO_SpeakerSetVolume(PlaybackAudioDescription.audio_volume_db_256);

  /* update speaker state */
  speaker_state = AUDIO_SPEAKER_STARTED;

  return UX_SUCCESS;
}

/**
  * @brief  AUDIO_SpeakerStop
  *         Stop the audio speaker.
  * @param  none
  * @retval status
  */
UINT AUDIO_SpeakerStop(VOID)
{
  UINT status = 0U;

  /* Stops audio playing and Power down the Audio Codec */
  status = BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);

  return status;
}

/**
  * @brief  AUDIO_SpeakerChangeFrequency
  *         change frequency then stop speaker.
  * @param  none
  * @retval status
  */
UINT AUDIO_SpeakerChangeFrequency(VOID)
{
  return UX_SUCCESS;
}

/**
  * @brief  AUDIO_SpeakerMute
  *         set Mute value to speaker.
  * @param  mute_state: mute value (0: mute, 1: unmute)
  * @retval status
  */
UINT AUDIO_SpeakerMute(UCHAR mute_state)
{
  UINT status = UX_SUCCESS;

  if (mute_state == 0)
  {
    /* Enables the mute mode */
    status = BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_OFF);
  }
  else
  {
    /* Disable the mute mode */
    status = BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
  }

  return status;
}

/**
  * @brief  AUDIO_SpeakerSetVolume
  *         set Volume value to speaker.
  * @param  channel_number: channel number
  * @param  volume_db_256: volume value in DB
  * @retval status
  */
UINT AUDIO_SpeakerSetVolume(INT volume_db_256)
{
  UINT status = UX_SUCCESS;

  /* Controls the current audio volume level */
  status = BSP_AUDIO_OUT_SetVolume(VOLUME_DB_256_TO_PERCENT(volume_db_256));

  return status;
}

/**
  * @brief  Manages the DMA full Transfer complete event
  * @param  Instance : AUDIO OUT Instance It can only be 0 (SAI)
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{

  /* Inject current data */
  BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)speaker_specific.data, (uint16_t)speaker_specific.injection_size);

  speaker_specific.data = speaker_buffer.data + speaker_buffer.rd_ptr;

  /* Increment read pointer */
  speaker_buffer.rd_ptr += speaker_specific.packet_length;

  if(speaker_buffer.rd_ptr >= speaker_buffer.size)
  {
    speaker_buffer.rd_ptr = speaker_buffer.rd_ptr - speaker_buffer.size;
  }

}

/**
  * @brief  Manages the DMA Half Transfer complete event
  * @param  Instance : AUDIO OUT Instance It can only be 0 (SAI)
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{

}
/* USER CODE END 1 */
