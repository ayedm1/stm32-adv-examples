/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio.c
  * @author  MCD Application Team
  * @brief   USBX Device Audio applicative source file
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
#include "ux_device_audio.h"

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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UX_DEVICE_CLASS_AUDIO *audio;
UX_DEVICE_CLASS_AUDIO_STREAM *playback_stream;
UX_DEVICE_CLASS_AUDIO10_CONTROL audio_control[USBD_AUDIO_STREAM_NMNBER];

extern AUDIO_DESCRIPTION audio_speaker_description;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  USBD_AUDIO_Activate
  *         This function is called when insertion of a Audio device.
  * @param  audio_instance: Pointer to the audio class instance.
  * @retval none
  */
VOID USBD_AUDIO_Activate(VOID *audio_instance)
{
  /* USER CODE BEGIN USBD_AUDIO_Activate */

  /* Save the audio instance */
  audio = (UX_DEVICE_CLASS_AUDIO *)audio_instance;

  /* Get the audio stream instance */
  ux_device_class_audio_stream_get(audio, 0, &playback_stream);

  /* USER CODE END USBD_AUDIO_Activate */

  return;
}

/**
  * @brief  USBD_AUDIO_Deactivate
  *         This function is called when extraction of a Audio device.
  * @param  audio_instance: Pointer to the audio class instance.
  * @retval none
  */
VOID USBD_AUDIO_Deactivate(VOID *audio_instance)
{
  /* USER CODE BEGIN USBD_AUDIO_Deactivate */

  /* Reset the Audio instance */
  audio = UX_NULL;

  /* Reset the Audio stream */
  playback_stream = UX_NULL;

  /* USER CODE END USBD_AUDIO_Deactivate */

  return;
}

/**
  * @brief  USBD_AUDIO_ControlProcess
  *         This function is invoked to manage the UAC class requests.
  * @param  audio_instance: Pointer to Audio class stream instance.
  * @param  transfer: Pointer to the transfer request.
  * @retval status
  */
UINT USBD_AUDIO_ControlProcess(UX_DEVICE_CLASS_AUDIO *audio_instance,
                               UX_SLAVE_TRANSFER *transfer)
{
  UINT status  = UX_SUCCESS;

  /* USER CODE BEGIN USBD_AUDIO_ControlProcess */
  UX_DEVICE_CLASS_AUDIO10_CONTROL_GROUP group;

  group.ux_device_class_audio10_control_group_controls_nb = 1;
  group.ux_device_class_audio10_control_group_controls = audio_control;

  status = ux_device_class_audio10_control_process(audio, transfer, &group);

  if (status == UX_SUCCESS)
  {
    switch(audio_control[0].ux_device_class_audio10_control_changed)
    {
      case UX_DEVICE_CLASS_AUDIO10_CONTROL_MUTE_CHANGED:

        /* Update mute state */
        audio_speaker_description.audio_mute = audio_control[0].ux_device_class_audio10_control_mute[0];

        /* Mute/Unmute audio codec volume */
        AUDIO_SpeakerMute(audio_speaker_description);

        break;

      case UX_DEVICE_CLASS_AUDIO10_CONTROL_VOLUME_CHANGED:

        /* Update current volume */
        audio_speaker_description.audio_volume_db_256 = (audio_control[0].ux_device_class_audio10_control_volume[0]);

        /* Set audio volume */
        AUDIO_SpeakerSetVolume(audio_speaker_description);

        break;

      case UX_DEVICE_CLASS_AUDIO10_CONTROL_FREQUENCY_CHANGED:
        break;

      default:
        break;
    }
  }
  /* USER CODE END USBD_AUDIO_ControlProcess */

  return status;
}

/**
  * @brief  USBD_AUDIO_SetControlValues
  *         This function is invoked to Set audio control values.
  * @param  none
  * @retval none
  */
VOID USBD_AUDIO_SetControlValues(VOID)
{
  /* USER CODE BEGIN USBD_AUDIO_SetControlValues */

  /* Fill audio control structure */
  audio_control[0].ux_device_class_audio10_control_sam_freq = USBD_AUDIO_PLAY_DEFAULT_FREQ;
  audio_control[0].ux_device_class_audio10_control_fu_id = USBD_AUDIO_PLAY_FEATURE_UNIT_ID;
  audio_control[0].ux_device_class_audio10_control_mute[0] = USBD_AUDIO_SPEAKER_MUTED;
  audio_control[0].ux_device_class_audio10_control_volume_min[0] = USBD_AUDIO_VOLUME_SPEAKER_MIN_DB_256;
  audio_control[0].ux_device_class_audio10_control_volume_max[0] = USBD_AUDIO_VOLUME_SPEAKER_MAX_DB_256;
  audio_control[0].ux_device_class_audio10_control_volume_res[0] = USBD_AUDIO_VOLUME_SPEAKER_RES_DB_256;
  audio_control[0].ux_device_class_audio10_control_volume[0] = USBD_AUDIO_VOLUME_SPEAKER_DEFAULT_DB_256;

  /* USER CODE END USBD_AUDIO_SetControlValues */

  return;
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
