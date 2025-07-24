/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio_play.c
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
#include "ux_device_audio_play.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
#define USBD_AUDIO_PLAYBACK_BUFFER_SIZE         (10*1024)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

AUDIO_DESCRIPTION       audio_speaker_description;
AUDIO_CIRCULAR_BUFFER   audio_speaker_buffer;
AUDIO_SPEAKER_PARMETER  audio_speaker_specific;
AUDIO_PLAYBACK_STATE    audio_playback_state;
AUDIO_SPEAKER_STATE     audio_speaker_state;
#ifdef DEBUG_AUDIO_SPEAKER
AUDIO_SPEAKER_BUFFER_STATS      audio_speaker_debug_stats_buffer[SPEAKER_DEBUG_BUFFER_SIZE];
INT                             audio_speaker_debug_stats_count = 0U;
extern __IO uint32_t            uwTick;
#endif /* DEBUG_AUDIO_SPEAKER */

#ifdef DEBUG_AUDIO_USB
AUDIO_USB_INPUT_BUFFER_DEBUG_STATS      audio_usb_debug_stats_buffer[USB_INPUT_NODE_DEBUG_BUFFER_SIZE];
INT                                     audio_usb_debug_stats_count = 0U;
extern __IO uint32_t                    uwTick;
#endif /* DEBUG_AUDIO_USB */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void USBD_AUDIO_DoPadding_24_32(AUDIO_CIRCULAR_BUFFER *buffer, uint8_t *data_dest, int size);
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
    if (AUDIO_PLAYBACK_ERROR == AUDIO_PLAYBACK_STARTED)
    {
      AUDIO_SpeakerStop(&audio_speaker_description, &audio_speaker_buffer, &audio_speaker_specific);

      audio_speaker_buffer.rd_ptr = 0U;
      audio_speaker_buffer.wr_ptr = 0U;
      audio_speaker_specific.flags = 0U;

      audio_playback_state = AUDIO_PLAYBACK_STOPPED;
    }

    return;
  }

  /* Start reception (stream opened) */
  ux_device_class_audio_reception_start(audio_play_stream);

  audio_playback_state = AUDIO_PLAYBACK_STARTED;

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
  uint16_t buffer_data_count;
  uint16_t buffer_free_size;

  /* Get access to first audio input frame */
  if (ux_device_class_audio_read_frame_get(audio_play_stream, &frame_buffer, &frame_length) == UX_SUCCESS)
  {

    if(audio_speaker_specific.flags &USBD_AUDIO_RESTART_REQUIRED)
    {
      /* When restart is required ignore the packet and reset buffer */
      audio_speaker_buffer.rd_ptr = 0U;
      audio_speaker_buffer.wr_ptr = 0U;
      audio_speaker_specific.flags = 0U;

      return;
    }

    /* Copy audio frame in speaker buffer */
    ux_utility_memory_copy(audio_speaker_buffer.data + audio_speaker_buffer.wr_ptr, frame_buffer, frame_length);

    /* Increment speaker buffer write pointer */
    audio_speaker_buffer.wr_ptr += frame_length;

    /* Check If some sample are in the margin area */
    if(audio_speaker_buffer.wr_ptr > audio_speaker_buffer.size)
    {
      audio_speaker_buffer.wr_ptr -= audio_speaker_buffer.size;

      /* Copy samples from margin area to regular area */
      ux_utility_memory_copy(audio_speaker_buffer.data, audio_speaker_buffer.data + audio_speaker_buffer.size, audio_speaker_buffer.wr_ptr);
    }

    /* Count pending audio samples in the buffer */
    buffer_data_count = USBD_AUDIO_BUFFER_FILLED_SIZE(audio_speaker_buffer);

    /* Check if write pointer equal to buffer size */
    if (audio_speaker_buffer.wr_ptr == audio_speaker_buffer.size)
    {
      /* Reset speaker buffer write pointer */
      audio_speaker_buffer.wr_ptr = 0;
    }

    /* Check if  circular audio buffer fill threshold is reached, To avoid overrun and underrun in the begin of streaming */
    if (((audio_speaker_specific.flags & USBD_AUDIO_THRESHOLD_REACHED) == 0) && (buffer_data_count >= audio_speaker_specific.input_threshold))
    {
      /* Start reading from the buffer */
      AUDIO_SpeakerStart(audio_speaker_description);

      audio_speaker_specific.flags |= USBD_AUDIO_THRESHOLD_REACHED;
    }

#ifdef DEBUG_AUDIO_USB
  audio_usb_debug_stats_buffer[audio_usb_debug_stats_count].read = audio_speaker_buffer.rd_ptr;
  audio_usb_debug_stats_buffer[audio_usb_debug_stats_count].write = audio_speaker_buffer.wr_ptr;
  audio_usb_debug_stats_buffer[audio_usb_debug_stats_count].time = uwTick;

  audio_usb_debug_stats_count++;

  if(audio_usb_debug_stats_count == USB_INPUT_NODE_DEBUG_BUFFER_SIZE)
  {
    audio_usb_debug_stats_count=0;
  }

#endif /* DEBUG_AUDIO_USB */

    /* control of possible overflow */
    buffer_free_size  = USBD_AUDIO_BUFFER_FREE_SIZE(audio_speaker_buffer);

    /* Check if overrun is happened */
    if(buffer_free_size < audio_speaker_specific.packet_length)
    {
      AUDIO_SpeakerStop(&audio_speaker_description, &audio_speaker_buffer, &audio_speaker_specific);

      audio_speaker_specific.flags = USBD_AUDIO_RESTART_REQUIRED;
    }

    if(audio_speaker_specific.flags &USBD_AUDIO_RESTART_REQUIRED)
    {
      /* When restart is required ignore the packet and reset buffer */
      audio_speaker_buffer.rd_ptr = 0U;
      audio_speaker_buffer.wr_ptr = 0U;
      audio_speaker_specific.flags = 0U;
    }

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

/* USER CODE BEGIN 2 */

/**
  * @brief  USBD_AUDIO_PlaybackInit
  *         Initializes the playback.
  * @param  none
  * @retval status
  */
UINT USBD_AUDIO_PlaybackInit(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer)
{

  /* Set audio used option */
  audio_desc -> audio_frequency = USBD_AUDIO_PLAY_DEFAULT_FREQ;
  audio_desc -> audio_channels_count = USBD_AUDIO_PLAY_CHANNEL_COUNT;
  audio_desc -> audio_resolution = USBD_AUDIO_PLAY_RES_BYTE;
  audio_desc -> audio_mute = USBD_AUDIO_SPEAKER_MUTED;
  audio_desc -> audio_volume_db_256 = USBD_AUDIO_VOLUME_SPEAKER_DEFAULT_DB_256;

  /* Initializes playback circular buffer */
  USBD_AUDIO_PlaybackInitializeDataBuffer(audio_desc, buffer);

  /* Initializes the audio speaker */
  AUDIO_SpeakerInit(audio_desc, buffer, &audio_speaker_specific);

  /* Update playback state */
  audio_playback_state = AUDIO_PLAYBACK_INITIALIZED;

  return UX_SUCCESS;
}

/**
  * @brief  USBD_AUDIO_PlaybackInitializeDataBuffer
  *         The circular buffer has the total size of buffer_size. this size is divided to two : the regular size and the margin.
  *         Margin is located at the tail of the circular buffer. Margin is used as some packet have regular size+/-1 sample.
  * @param  buffer: main circular buffer.
  * @retval none
  */
UINT USBD_AUDIO_PlaybackInitializeDataBuffer(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer)
{
  uint16_t packet_length;
  uint16_t max_packet_length;
  uint16_t buffer_margin = 0;
  uint16_t packet_size;

  /* Initialize speaker buffer and allocate Playback data buffer */
  buffer -> size = USBD_AUDIO_PLAYBACK_BUFFER_SIZE;
  buffer -> data = ux_utility_memory_allocate(UX_SAFE_ALIGN, UX_CACHE_SAFE_MEMORY, buffer -> size);
  buffer -> rd_ptr = 0U;
  buffer -> wr_ptr = 0U;

  /* Check Buffer allocated */
  if (buffer -> data == UX_NULL)
  {
    return UX_ERROR;
  }

  ux_utility_memory_set(buffer -> data, 0, buffer -> size);

  /* Packet_length is the packet normal length */
  packet_length = AUDIO_USB_PACKET_SIZE_FROM_AUD_DESC(audio_desc);

  /* Max_packet_length is the packet to read each time from buffer */
  max_packet_length = AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc);

  /* Calculate buffer_margin a protection area size in circular buffer */
  buffer_margin = (max_packet_length > packet_length) ? max_packet_length : 0;

  /* Packet_size is the USB Audio packet size */
  packet_size = AUDIO_MS_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc);

  buffer -> size = ((USHORT)((buffer -> size - buffer_margin) / packet_size)) * packet_size;

  return UX_SUCCESS;
}


/**
  * @brief  AUDIO_SpeakerInit
  *         Initializes the audio speaker node, set callbacks and start the codec. As no data are ready. The
  *         SAI is feeded from the alternate buffer (filled by zeros)
  * @param  none
  * @retval status
  */
UINT AUDIO_SpeakerInit(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer, AUDIO_SPEAKER_PARMETER *speaker_specific)
{

  /* Injection size is the nominal size of the unit packet sent using DMA to SAI */
  speaker_specific -> packet_length = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(audio_desc);

  if (USBD_AUDIO_RESOLUTION_BIT(audio_speaker_description.audio_resolution) == USBD_PLAY_RES_BIT_24B)
  {
    /* Set nominal size of the unit packet sent using DMA to SAI */
    speaker_specific -> injection_size = AUDIO_SPEAKER_INJECTION_LENGTH_24B(audio_desc);


    /* Set half size of the alternative buffer */
    speaker_specific -> alt_buf_half_size = speaker_specific -> injection_size;

    /* When the padding is needed the double buffering are required. It means that the alt_buff will contain two packet */
    speaker_specific -> double_buff = 1U;

  }
  else
  {
    speaker_specific -> double_buff = 0U;

    speaker_specific -> injection_size = speaker_specific -> packet_length;
  }

  /* Allocate memory for playback alternate buffer
   *   speaker_specific -> alt_buffer: alternative buffer used  when underrun is produced(no enough data to inject) or when padding should be added.
   */
  speaker_specific -> alt_buffer = ux_utility_memory_allocate(UX_SAFE_ALIGN, UX_CACHE_SAFE_MEMORY, speaker_specific -> injection_size);

  /* Check if memory allocation succefuly */
  if (speaker_specific ->alt_buffer == UX_NULL)
  {
    return UX_ERROR;
  }

  ux_utility_memory_set(speaker_specific -> alt_buffer, 0, speaker_specific -> injection_size);

  /* Set offset binary flag. used to indicate if next packet is in the first half of alternate buffer or in the second half */
  speaker_specific -> offset = 0U;


  /* Start injection of dumped data
   *  speaker_specific -> data: pointer to data, which is going to transmit using DMA to SAI
   *  speaker_specific -> data_size: size of data, which is going to transmit using DMA to SAI
   */
  speaker_specific -> data = speaker_specific -> alt_buffer;
  speaker_specific -> data_size = speaker_specific -> injection_size;

  /* Configures the audio peripherals */
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, (uint32_t) audio_desc -> audio_frequency, USBD_AUDIO_RESOLUTION_BIT(audio_desc -> audio_resolution));

  /* Start DMA stream from output double buffer to codec in Circular mode launch */
  BSP_AUDIO_OUT_Play((uint16_t*) speaker_specific -> data, speaker_specific -> data_size);

  /* Threshold for Circular buffer.
   * - After streaming starts, buffer write is allowed and buffer read is blocked.
   * - Then when threshold is reached an event is raised to the session and buffer read is unlocked.
   */
  speaker_specific -> input_threshold = buffer->size/2;

  /* Update speaker state */
  audio_speaker_state = AUDIO_SPEAKER_INITIALIZED;

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
  if (audio_speaker_state != AUDIO_SPEAKER_OFF)
  {
   if (audio_speaker_state != AUDIO_SPEAKER_ERROR)
    {
      /* Stop the DMA injection */
      audio_speaker_specific.cmd ^= SPEAKER_CMD_EXIT;
    }

    /* Stops audio playing and Power down the Audio Codec */
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);

    /* De-initializes the audio out peripheral */
    BSP_AUDIO_OUT_DeInit();

    /* Update speaker state */
    audio_speaker_state = AUDIO_SPEAKER_OFF;
  }

  return UX_SUCCESS;
}

/**
  * @brief  AUDIO_SpeakerStart
  *         Start the audio speaker.
  * @param  none
  * @retval status
  */
UINT AUDIO_SpeakerStart(AUDIO_DESCRIPTION audio_desc)
{

  AUDIO_SpeakerMute(audio_desc);
  AUDIO_SpeakerSetVolume(audio_desc);

  /* Update speaker state */
  audio_speaker_state = AUDIO_SPEAKER_STARTED;

  return UX_SUCCESS;
}

/**
  * @brief  AUDIO_SpeakerStop
  *         Stop the audio speaker.
  * @param  none
  * @retval status
  */
UINT AUDIO_SpeakerStop(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer, AUDIO_SPEAKER_PARMETER *speaker_specific)
{
  UINT status = 0U;

  speaker_specific -> cmd = SPEAKER_CMD_STOP;

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
UINT AUDIO_SpeakerMute(AUDIO_DESCRIPTION audio_desc)
{
  UINT status = UX_SUCCESS;

  /* Enables the mute mode */
  status = BSP_AUDIO_OUT_SetMute(audio_desc.audio_mute);

  return status;
}

/**
  * @brief  AUDIO_SpeakerSetVolume
  *         set Volume value to speaker.
  * @param  channel_number: channel number
  * @param  volume_db_256: volume value in DB
  * @retval status
  */
UINT AUDIO_SpeakerSetVolume(AUDIO_DESCRIPTION audio_desc)
{
  UINT status = UX_SUCCESS;

  /* Controls the current audio volume level */
  status = BSP_AUDIO_OUT_SetVolume(VOLUME_DB_256_TO_PERCENT(audio_desc.audio_volume_db_256));

  return status;
}

/**
  * @brief  Manages the DMA full Transfer complete event
  * @param  Instance : AUDIO OUT Instance It can only be 0 (SAI)
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  uint16_t read_length;
  uint16_t wr_distance;

  if (audio_speaker_state != AUDIO_SPEAKER_OFF)
  {

    if ((audio_speaker_specific.cmd & SPEAKER_CMD_EXIT) != 0U)
    {
      /* Reset Speaker command value */
      audio_speaker_specific.cmd ^= SPEAKER_CMD_EXIT;

      /* Return  */
      return ;
    }

    /* Check if speaker should be stopped */
    if ((audio_speaker_specific.cmd & SPEAKER_CMD_STOP) != 0U)
    {
      audio_speaker_specific.data = audio_speaker_specific.alt_buffer;
      audio_speaker_specific.data_size = audio_speaker_specific.injection_size;
      audio_speaker_specific.offset = 0;
      ux_utility_memory_set(audio_speaker_specific.data, 0, audio_speaker_specific.data_size);
      audio_speaker_state = AUDIO_SPEAKER_STOPPED;
      audio_speaker_specific.cmd ^= SPEAKER_CMD_STOP;
    }

    /* Inject current data */
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)audio_speaker_specific.data, (uint16_t)audio_speaker_specific.data_size);


    /* Check if speaker was started */
    if (audio_speaker_state == AUDIO_SPEAKER_STARTED)
    {
#ifdef DEBUG_AUDIO_SPEAKER
      audio_speaker_debug_stats_buffer[audio_speaker_debug_stats_count].time = uwTick;
#endif /* DEBUG_AUDIO_SPEAKER */

      if (USBD_AUDIO_RESOLUTION_BIT(audio_speaker_description.audio_resolution) == USBD_PLAY_RES_BIT_24B)
      {
        if (audio_speaker_specific.offset == 0U)
        {
          audio_speaker_specific.data = audio_speaker_specific.alt_buffer;
        }
        else
        {
          audio_speaker_specific.data = audio_speaker_specific.alt_buffer + audio_speaker_specific.data_size;
        }

        audio_speaker_specific.offset ^= 1;
      }

      /* Prepare next size to inject */
      audio_speaker_specific.data_size = audio_speaker_specific.injection_size;

      read_length = audio_speaker_specific.packet_length;
      wr_distance = USBD_AUDIO_BUFFER_FILLED_SIZE(audio_speaker_buffer);

      /* Check if underrun is happened */
      if(wr_distance < audio_speaker_specific.injection_size)
      {
        AUDIO_SpeakerStop(&audio_speaker_description, &audio_speaker_buffer, &audio_speaker_specific);

        audio_speaker_specific.flags = USBD_AUDIO_RESTART_REQUIRED;
      }
      else
      {

        if (USBD_AUDIO_RESOLUTION_BIT(audio_speaker_description.audio_resolution) == USBD_PLAY_RES_BIT_24B)
        {

          /* Buffer already prepared in half transfer */
          USBD_AUDIO_DoPadding_24_32(&audio_speaker_buffer, audio_speaker_specific.data, read_length);
        }
        else
        {

          audio_speaker_specific.data = audio_speaker_buffer.data + audio_speaker_buffer.rd_ptr;

#ifdef DEBUG_AUDIO_SPEAKER
          audio_speaker_debug_stats_buffer[audio_speaker_debug_stats_count].data = audio_speaker_specific.data;
          audio_speaker_debug_stats_buffer[audio_speaker_debug_stats_count].injection_size = audio_speaker_specific.data_size;
#endif /* DEBUG_AUDIO_SPEAKER */
        }

        /* Increment read pointer */
        audio_speaker_buffer.rd_ptr += read_length;

        if (audio_speaker_buffer.rd_ptr >= audio_speaker_buffer.size)
        {
          audio_speaker_buffer.rd_ptr = audio_speaker_buffer.rd_ptr - audio_speaker_buffer.size;
        }
#ifdef DEBUG_AUDIO_SPEAKER
        audio_speaker_debug_stats_buffer[audio_speaker_debug_stats_count].read = audio_speaker_buffer.rd_ptr;
#endif /* DEBUG_AUDIO_SPEAKER */

      }

#ifdef DEBUG_AUDIO_SPEAKER
      if(++audio_speaker_debug_stats_count == SPEAKER_DEBUG_BUFFER_SIZE)
      {
        audio_speaker_debug_stats_count = 0;
      }
#endif /* DEBUG_AUDIO_SPEAKER */
    }
  }
}

/**
  * @brief  AUDIO_DoPadding_24_32
  *         padding 24bit  sample to 32 sample by adding zeros .
  * @param  buffer:
  * @param  data_dest:
  * @param  size:
  * @retval none
  */
static void USBD_AUDIO_DoPadding_24_32(AUDIO_CIRCULAR_BUFFER *buffer, uint8_t *data_dest, int size)
{
  int index1 = 0U;
  int index2 = 0U;
  int index3 = 0U;
  int read_pointer;

  read_pointer = buffer -> rd_ptr;

  for(index1 = 0; index1 < size; index1+=3)
  {
    data_dest[index2++] = 0;

    for(index3 = 0; index3<3; index3++)
    {
      if (read_pointer == buffer->size)
      {
        read_pointer = 0;
      }

      data_dest[index2++] = buffer->data[read_pointer++];
    }
  }
}
/* USER CODE END 2 */
