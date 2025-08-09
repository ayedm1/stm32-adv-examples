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
#define AUDIO_IO_BEGIN_OF_STREAM                0x01 /* Begin of stream sent to session when first packet is received */
#define AUDIO_IO_RESTART_REQUIRED               0x40 /* Restart of USB node is required , after frequency changes for examples */
#define AUDIO_IO_THRESHOLD_REACHED              0x08 /* this flag is set when  main circular audio  buffer fill threshold is reached.Then consumer node starts  reading from the buffer, this is to avoid overrun and underrun in the begin of streaming*/

#define USB_AUDIO_CONFIG_RECORD_BUFFER_SIZE     (USBD_AUDIO_RECORD_CHANNEL_COUNT*1024)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

AUDIO_DESCRIPTION               audio_microphone_description;
AUDIO_CIRCULAR_BUFFER           audio_microphone_buffer;
AUDIO_MICROPHPNE_PARMETER       audio_microphone_specific;
AUDIO_RECORDING_STATE           audio_recording_state;
AUDIO_MICROPHONE_STATE          audio_microphone_state;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static VOID AUDIO_FillDataToBuffer(uint32_t pcm_offset);
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

  /* Do nothing if alternate setting is 0 (stream closed) */
  if (alternate_setting == 0)
  {
    return;
  }

  AUDIO_MicrophoneStart();

  ux_device_class_audio_frame_write(audio_record_stream, audio_microphone_specific.alt_buff, audio_microphone_specific.usb_packet_length);

  /* Start sending valid frames in the audio class */
  ux_device_class_audio_transmission_start(audio_record_stream);

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
  UX_PARAMETER_NOT_USED(length);
  static uint8_t flags;

  if ((flags & AUDIO_IO_BEGIN_OF_STREAM) != 0U)
  {
    /* First threshold is a half of buffer */
    if (audio_microphone_buffer.wr_ptr < (audio_microphone_buffer.size/2))
    {
      /* Buffer is not ready  */
      ux_device_class_audio_frame_write(audio_record_stream, audio_microphone_specific.alt_buff, audio_microphone_specific.usb_packet_length);

      return;
    }

    flags |= AUDIO_IO_BEGIN_OF_STREAM;
  }


  /* Increment read pointer */
  audio_microphone_buffer.rd_ptr += audio_microphone_specific.usb_packet_length;

  /* Manage the packet not aligned */
  if (audio_microphone_buffer.rd_ptr > audio_microphone_buffer.size)
  {
    audio_microphone_buffer.rd_ptr -= audio_microphone_buffer.size;

    ux_utility_memory_copy(audio_microphone_buffer.data + audio_microphone_buffer.size, audio_microphone_buffer.data, audio_microphone_buffer.rd_ptr);

    if (audio_microphone_buffer.rd_ptr + audio_microphone_buffer.size >= USB_AUDIO_CONFIG_RECORD_BUFFER_SIZE)
    {
      return;
    }
  }

  if(audio_microphone_buffer.rd_ptr == audio_microphone_buffer.size)
  {
    audio_microphone_buffer.rd_ptr = 0;
  }

  ux_device_class_audio_frame_write(audio_record_stream, audio_microphone_buffer.data + audio_microphone_buffer.rd_ptr, audio_microphone_specific.usb_packet_length);

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
  max_frame_buffer_number = 3U;
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
  max_frame_buffer_size = USBD_AUDIO_RECORD_EPIN_FS_MPS;
  /* USER CODE END USBD_AUDIO_RecordingStreamGetMaxFrameBufferSize */

  return max_frame_buffer_size;
}

/* USER CODE BEGIN 2 */

/**
  * @brief  USBD_AUDIO_RecordingInit
  *         Initializes the record.
  * @param  none
  * @retval status
  */
UINT USBD_AUDIO_RecordingInit(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer)
{

  /* Set audio used option */
  audio_desc -> audio_frequency = USBD_AUDIO_RECORD_DEFAULT_FREQ;
  audio_desc -> audio_channels_count = USBD_AUDIO_RECORD_CHANNEL_COUNT;
  audio_desc -> audio_resolution = USBD_AUDIO_RECORD_RES_BYTE;

  /* Initializes recording circular buffer */
  USBD_AUDIO_RecordingInitializeDataBuffer(audio_desc, buffer);

  /* Initializes the audio microphone */
  AUDIO_MicrophoneInit(audio_desc, buffer, &audio_microphone_specific);

  /* Update recording state */
  audio_recording_state = AUDIO_RECORDING_INITIALIZED;

  return UX_SUCCESS;
}

UINT USBD_AUDIO_RecordingInitializeDataBuffer(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer)
{
  uint16_t packet_length;
  uint16_t max_packet_length;
  uint16_t buffer_margin = 0;
  uint16_t packet_size;

  /* Initialize microphone buffer and allocate Playback data buffer */
  buffer -> size = USB_AUDIO_CONFIG_RECORD_BUFFER_SIZE;
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
  * @brief  AUDIO_MicrophoneInit
  *         Start the audio Microphone.
  * @param  none
  * @retval status
  */
UINT AUDIO_MicrophoneInit(AUDIO_DESCRIPTION *audio_desc, AUDIO_CIRCULAR_BUFFER *buffer, AUDIO_MICROPHPNE_PARMETER *audio_microphone_specific)
{
  audio_microphone_specific -> usb_packet_length = AUDIO_USB_PACKET_SIZE_FROM_AUD_DESC(audio_desc);

  audio_microphone_specific -> max_packet_length = AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc);

  /* Allocate and initialize the alternative buffer. It is filled with zero and it is sent to USB host when no enough data are ready */
  audio_microphone_specific -> alt_buff = ux_utility_memory_allocate(UX_SAFE_ALIGN, UX_CACHE_SAFE_MEMORY, audio_microphone_specific -> max_packet_length);

  if (audio_microphone_specific -> alt_buff  == UX_NULL)
  {
    return UX_ERROR;
  }

  ux_utility_memory_set(audio_microphone_specific -> alt_buff, 0, audio_microphone_specific -> max_packet_length);

  BSP_AUDIO_IN_Init(audio_desc -> audio_frequency, audio_desc -> audio_resolution, audio_desc -> audio_channels_count, INPUT_DEVICE_DIGITAL_MIC);

  BSP_AUDIO_IN_AllocScratch(audio_microphone_specific -> scratch, AUDIO_MIC_SCRATCH_SIZE);

  audio_microphone_specific -> packet_sample_count = AUDIO_PACKET_SAMPLES_COUNT(audio_desc ->audio_frequency);
  audio_microphone_specific -> packet_sample_size = AUDIO_SAMPLE_LENGTH(audio_desc);

  audio_microphone_specific -> mic_packet_length = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(audio_desc);

  BSP_AUDIO_IN_Record(0,0);

  audio_microphone_state = AUDIO_MICROPHONE_INITIALIZED;

  return UX_SUCCESS;
}

/**
  * @brief  AUDIO_MicrophoneStart
  *         Start the audio Microphone.
  * @param  none
  * @retval status
  */
UINT AUDIO_MicrophoneStart(VOID)
{

  audio_microphone_state = AUDIO_MICROPHONE_STARTED;

  return UX_SUCCESS;
}

/**
  * @brief  BSP_AUDIO_IN_HalfTransfer_CallBack
            Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  if (audio_microphone_state == AUDIO_MICROPHONE_STARTED)
  {
    AUDIO_FillDataToBuffer(0);
  }
}

/**
  * @brief  BSP_AUDIO_IN_TransferComplete_CallBack
  *         Manages the DMA Transfer complete interrupt
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  if (audio_microphone_state == AUDIO_MICROPHONE_STARTED)
  {
    AUDIO_FillDataToBuffer(audio_microphone_specific.packet_sample_count);
  }
}

/**
  * @brief  AUDIO_FillDataToBuffer
   *        Get received data from dfsdm
  * @param  pcm_offset
  * @retval none
  */
static VOID AUDIO_FillDataToBuffer(uint32_t pcm_offset)
{

  if (audio_microphone_state == AUDIO_MICROPHONE_STARTED)
  {

    /* to change to support other frequencies */
    BSP_AUDIO_IN_Get_PcmBuffer((audio_microphone_buffer.data + audio_microphone_buffer.wr_ptr), audio_microphone_specific.packet_sample_count,
                               pcm_offset, audio_microphone_description.audio_resolution);

    /* check for overflow */
    audio_microphone_buffer.wr_ptr += audio_microphone_specific.mic_packet_length;

    if(audio_microphone_buffer.wr_ptr == audio_microphone_buffer.size)
    {
      audio_microphone_buffer.wr_ptr = 0;
    }
  }
}

/* USER CODE END 2 */
