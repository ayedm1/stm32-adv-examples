/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_device.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "app_usbx_device.h"

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

/* USER CODE BEGIN UX_Device_Memory_Buffer */
/* Set hpcd_USB_OTG_HS at the end of non-cacheable
   Memory address 0x20028000. */
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma location = 0x20028000
#elif defined ( __CC_ARM ) /* MDK ARM Compiler */
__attribute__((section(".UsbHpcdSection")))
#elif defined ( __GNUC__ ) /* GNU Compiler */
__attribute__((section(".UsbHpcdSection")))
#endif
/* USER CODE END UX_Device_Memory_Buffer */
#if defined ( __ICCARM__ )
#pragma data_alignment=4
#endif
__ALIGN_BEGIN static UCHAR ux_device_memory_buffer[USBX_DEVICE_APP_MEMORY_BUFFER_SIZE] __ALIGN_END;

static ULONG audio_interface_number;
static ULONG audio_configuration_number;
static UX_DEVICE_CLASS_AUDIO_PARAMETER audio_parameter;
static UX_DEVICE_CLASS_AUDIO_STREAM_PARAMETER audio_stream_parameter[USBD_AUDIO_STREAM_NMNBER];
static uint8_t audio_stream_index = 0U;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application USBX Device Initialization.
  * @param  none
  * @retval status
  */
UINT MX_USBX_Device_Init(VOID)
{
  UINT ret = UX_SUCCESS;
  UCHAR *device_framework_full_speed;
  ULONG device_framework_fs_length;
  ULONG string_framework_length;
  ULONG language_id_framework_length;
  UCHAR *string_framework;
  UCHAR *language_id_framework;
  UCHAR *pointer;

  /* USER CODE BEGIN MX_USBX_Device_Init0 */

  /* USER CODE END MX_USBX_Device_Init0 */

  pointer = ux_device_memory_buffer;

  /* Initialize USBX Memory */
  if (ux_system_initialize(pointer, USBX_DEVICE_MEMORY_STACK_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_SYSTEM_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_SYSTEM_INITIALIZE_ERROR */
  }

  /* Get Device Framework Full Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                                                &device_framework_fs_length);

  /* Get String Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get Language Id Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&language_id_framework_length);

  /* Install the device portion of USBX */
  if (ux_device_stack_initialize(UX_NULL,
                                 0,
                                 device_framework_full_speed,
                                 device_framework_fs_length,
                                 string_framework,
                                 string_framework_length,
                                 language_id_framework,
                                 language_id_framework_length,
                                 UX_NULL) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_INITIALIZE_ERROR */
  }

  /* Initialize audio playback control values */
  USBD_AUDIO_SetControlValues();

  /* Initialize the audio class parameters for audio playback */
  audio_stream_parameter[audio_stream_index].ux_device_class_audio_stream_parameter_callbacks.ux_device_class_audio_stream_change
    = USBD_AUDIO_PlaybackStreamChange;

  audio_stream_parameter[audio_stream_index].ux_device_class_audio_stream_parameter_callbacks.ux_device_class_audio_stream_frame_done
    = USBD_AUDIO_PlaybackStreamFrameDone;

  audio_stream_parameter[audio_stream_index].ux_device_class_audio_stream_parameter_max_frame_buffer_nb
    = USBD_AUDIO_PlaybackStreamGetMaxFrameBufferNumber();

  audio_stream_parameter[audio_stream_index].ux_device_class_audio_stream_parameter_max_frame_buffer_size
    = USBD_AUDIO_PlaybackStreamGetMaxFrameBufferSize();

  audio_stream_parameter[audio_stream_index].ux_device_class_audio_stream_parameter_task_function
    = ux_device_class_audio_read_task_function;

  /* Set the parameters for audio device */
  audio_parameter.ux_device_class_audio_parameter_streams_nb  = USBD_AUDIO_STREAM_NMNBER;
  audio_parameter.ux_device_class_audio_parameter_streams     = audio_stream_parameter;

  audio_parameter.ux_device_class_audio_parameter_callbacks.ux_slave_class_audio_instance_activate
    = USBD_AUDIO_Activate;

  audio_parameter.ux_device_class_audio_parameter_callbacks.ux_slave_class_audio_instance_deactivate
    = USBD_AUDIO_Deactivate;

 audio_parameter.ux_device_class_audio_parameter_callbacks.ux_device_class_audio_control_process
    = USBD_AUDIO_ControlProcess;

  /* Get audio configuration number */
  audio_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_AUDIO_10, 0);

  /* Find audio interface number */
  audio_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_AUDIO_10, 0);
  /* Initialize the device audio class */
  if (ux_device_stack_class_register(_ux_system_slave_class_audio_name,
                                     ux_device_class_audio_entry,
                                     audio_configuration_number,
                                     audio_interface_number,
                                     &audio_parameter) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_AUDIO_REGISTER_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_AUDIO_REGISTER_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Device_Init1 */

  /* Initialization of USB device */
  MX_USB_Device_Init();

  /* USER CODE END MX_USBX_Device_Init1 */

  return ret;
}

/**
  * @brief  _ux_utility_interrupt_disable
  *         USB utility interrupt disable.
  * @param  none
  * @retval none
  */
ALIGN_TYPE _ux_utility_interrupt_disable(VOID)
{
  UINT interrupt_save;

  /* USER CODE BEGIN _ux_utility_interrupt_disable */
  interrupt_save = __get_PRIMASK();
  __disable_irq();
  /* USER CODE END _ux_utility_interrupt_disable */

  return interrupt_save;
}

/**
  * @brief  _ux_utility_interrupt_restore
  *         USB utility interrupt restore.
  * @param  flags
  * @retval none
  */
VOID _ux_utility_interrupt_restore(ALIGN_TYPE flags)
{
  /* USER CODE BEGIN _ux_utility_interrupt_restore */
  __set_PRIMASK(flags);
  /* USER CODE END _ux_utility_interrupt_restore */
}

/**
  * @brief  _ux_utility_time_get
  *         Get Time Tick for host timing.
  * @param  none
  * @retval time tick
  */
ULONG _ux_utility_time_get(VOID)
{
  ULONG time_tick = 0U;

  /* USER CODE BEGIN _ux_utility_time_get */
  time_tick = HAL_GetTick();
  /* USER CODE END _ux_utility_time_get */

  return time_tick;
}

/* USER CODE BEGIN 2 */

/**
  * @brief  MX_USBX_Device_Process
  *         Run USBX state machine.
  * @param  arg: not used
  * @retval none
  */
VOID USBX_Device_Process(VOID *arg)
{
  ux_device_stack_tasks_run();
}

/**
  * @brief  MX_USB_Device_Init
  *         Initialization of USB device.
  * @param  none
  * @retval none
  */
VOID MX_USB_Device_Init(VOID)
{
  /* USER CODE BEGIN USB_Device_Init_PreTreatment_0 */

  /* USER CODE END USB_Device_Init_PreTreatment_0 */

  MX_USB_OTG_HS_PCD_Init();

  /* USER CODE BEGIN USB_Device_Init_PreTreatment_1 */

  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_HS, 0x200);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 0, 0x40);

  /* USER CODE END USB_Device_Init_PreTreatment_1 */

  /* Initialize the device controller driver */
  ux_dcd_stm32_initialize((ULONG)USB_OTG_HS, (ULONG)&hpcd_USB_OTG_HS);

  HAL_PCD_Start(&hpcd_USB_OTG_HS);

  /* USER CODE BEGIN USB_Device_Init_PostTreatment */

  /* USER CODE END USB_Device_Init_PostTreatment */
}

/* USER CODE END 2 */
