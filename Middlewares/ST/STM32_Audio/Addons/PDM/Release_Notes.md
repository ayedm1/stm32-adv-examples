---
pagetitle: Release Notes for PDM2PCM Library
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="../_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>\ PDM2PCM Library\ </mark>
Copyright &copy; 2023 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>
# Purpose

The PDM2PCM library has the function to decimate and filter out a Pulse Density Modulated (PDM) stream from a digital microphone, 
in order to convert it to a Pulse Code Modulated (PCM) signal output stream.

The PCM output stream is implemented at a sampling rate of 16kHz with a 16 bits resolution. Various decimation factors can be configured, in order to adapt to various PDM clocks.
Digital microphone gain is configurable at any time and allows 1dB step in the range of -12dB to +51dB.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section_3.7.0" checked aria-hidden="true">
<label for="collapse-section_3.7.0" aria-hidden="true">V3.7.0 / 27-November-2024</label>
<div>

## Main Changes

   - Add PDM Filter libraries for Cortex-M55 (CM55).
      - libPDMFilter_CM55_GCC_wc16.a
      - libPDMFilter_CM55_GCC_wc16_soft.a
      - libPDMFilter_CM55_GCC_wc16_softfp.a
      - libPDMFilter_CM55_GCC_wc32.a
      - libPDMFilter_CM55_GCC_wc32_soft.a
      - libPDMFilter_CM55_GCC_wc32_softfp.a
      - libPDMFilter_CM55_IAR_wc32.a
      - libPDMFilter_CM55_Keil_wc16.lib

## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_3.6.0" aria-hidden="true">
<label for="collapse-section_3.6.0" aria-hidden="true">V3.6.0 / 07-March-2024</label>
<div>

## Main Changes

  - Bug fix: Kept legacy API (PDM_Filter_Init, PDM_Filter_setConfig, PDM_Filter_getConfig, PDM_Filter in pdm2pcm_glo.h) in which bit_order config must be set in the opposite state of PDM HW config (same as before) to avoid any issue with old develpments. Added a new API (PDM2PCM_init, PDM2PCM_setConfig, PDM2PCM_getConfig, PDM2PCM_process in pdm2pcm.h) in which bit_order config must be aligned with PDM HW config.
  - Bug fix: In case of several instances of PDM2PCM with different bit_order config, the last bit_order config was overwriting the previous ones.
  - List of updated files:
    - libPDMFilter_CM0_GCC_wc16.a
    - libPDMFilter_CM0_GCC_wc32.a
    - libPDMFilter_CM0_IAR_wc16.a
    - libPDMFilter_CM0_IAR_wc32.a
    - libPDMFilter_CM0_Keil_wc16.lib
    - libPDMFilter_CM33_GCC_wc16.a
    - libPDMFilter_CM33_GCC_wc16_softfp.a
    - libPDMFilter_CM33_GCC_wc32.a
    - libPDMFilter_CM33_GCC_wc32_softfp.a
    - libPDMFilter_CM33_IAR_wc16.a
    - libPDMFilter_CM33_IAR_wc32.a
    - libPDMFilter_CM33_Keil_wc16.lib
    - libPDMFilter_CM3_GCC_wc16.a
    - libPDMFilter_CM3_GCC_wc32.a
    - libPDMFilter_CM3_IAR_wc16.a
    - libPDMFilter_CM3_IAR_wc32.a
    - libPDMFilter_CM3_Keil_wc16.lib
    - libPDMFilter_CM4_GCC_wc16.a
    - libPDMFilter_CM4_GCC_wc16_soft.a
    - libPDMFilter_CM4_GCC_wc16_softfp.a
    - libPDMFilter_CM4_GCC_wc32.a
    - libPDMFilter_CM4_GCC_wc32_soft.a
    - libPDMFilter_CM4_GCC_wc32_softfp.a
    - libPDMFilter_CM4_IAR_wc16.a
    - libPDMFilter_CM4_IAR_wc32.a
    - libPDMFilter_CM4_Keil_wc16.lib
    - libPDMFilter_CM7_GCC_wc16.a
    - libPDMFilter_CM7_GCC_wc16_softfp.a
    - libPDMFilter_CM7_GCC_wc32.a
    - libPDMFilter_CM7_GCC_wc32_softfp.a
    - libPDMFilter_CM7_IAR_wc16.a
    - libPDMFilter_CM7_IAR_wc32.a
    - libPDMFilter_CM7_Keil_wc16.lib

## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_3.5.1" aria-hidden="true">
<label for="collapse-section_3.5.1" aria-hidden="true">V3.5.1 / 01-September-2023</label>
<div>

## Main Changes

  - All source files: update disclaimer to add reference to the new license agreement.


## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_3.5.0" aria-hidden="true">
<label for="collapse-section_3.5.0" aria-hidden="true">V3.5.0 / 13-March-2023</label>
<div>

## Main Changes

  - Update PDM Filter libraries for Cortex-M0 (CM0).
    - List of updated files:
      - libPDMFilter_CM0_GCC_wc16.a
      - libPDMFilter_CM0_GCC_wc32.a
      - libPDMFilter_CM0_IAR_wc32.a
      - libPDMFilter_CM0_Keil_wc16.lib


## Backward Compatibility

None
</div>

:::

::: {.collapse}
<input type="checkbox" id="collapse-section_3.4.0" aria-hidden="true">
<label for="collapse-section_3.4.0" aria-hidden="true">V3.4.0 / 02-February-2021</label>
<div>

## Main Changes

  - Update PDM Filter libraries for Cortex-M4 (CM4). Add compiler barriers to fix issue with O3 optimization in GCC.
    - List of updated files:
       - libPDMFilter_CM4_GCC_wc16.a
       - libPDMFilter_CM4_GCC_wc16_softfp.a
       - libPDMFilter_CM4_GCC_wc32.a
       - libPDMFilter_CM4_GCC_wc32_softfp.a
       - libPDMFilter_CM4_IAR_wc32.a
       - libPDMFilter_CM4_Keil_wc16.lib
       - List of added files:
       - libPDMFilter_CM4_GCC_wc16_soft.a
       - libPDMFilter_CM4_GCC_wc32_soft.a
  - Update PDM Filter libraries for Cortex-M7 (CM7). Add compiler barriers to fix issue with O3 optimization in GCC.
    - List of updated files:
       - libPDMFilter_CM7_GCC_wc16.a
       - libPDMFilter_CM7_GCC_wc16_softfp.a
       - libPDMFilter_CM7_GCC_wc32.a
       - libPDMFilter_CM7_GCC_wc32_softfp.a
       - libPDMFilter_CM7_IAR_wc32.a
       - libPDMFilter_CM7_Keil_wc16.lib
  - Add PDM Filter libraries for Cortex-M33 (CM33).
    - List of added files:
       - libPDMFilter_CM33_GCC_wc16.a
       - libPDMFilter_CM33_GCC_wc16_softfp.a
       - libPDMFilter_CM33_GCC_wc32.a
       - libPDMFilter_CM33_GCC_wc32_softfp.a
       - libPDMFilter_CM33_IAR_wc32.a
       - libPDMFilter_CM33_Keil_wc16.lib
  - Add PDM Filter libraries for Cortex-M3 (CM3). Add compiler barriers to fix issue with O3 optimization in GCC.
    - List of updated files:
       - libPDMFilter_CM3_GCC_wc16.a
       - libPDMFilter_CM3_GCC_wc32.a
       - libPDMFilter_CM3_IAR_wc32.a
       - libPDMFilter_CM3_Keil_wc16.lib
  - Add PDM Filter libraries for Cortex-M0 (CM0).
    - List of added files:
       - libPDMFilter_CM0_GCC_wc16.a
       - libPDMFilter_CM0_GCC_wc32.a
       - libPDMFilter_CM0_IAR_wc32.a
       - libPDMFilter_CM0_Keil_wc16.lib

 
 

## Backward Compatibility

None
</div>

:::


::: {.collapse}
<input type="checkbox" id="collapse-section_3.3.0" aria-hidden="true">
<label for="collapse-section_3.3.0" aria-hidden="true">V3.3.0 / 20-April-2020</label>
<div>

## Main Changes

  - Update PDM Filter libraries for Cortex-M7 (CM7) to support STM32H72x/3x devices
      - List of updated files:
      - libPDMFilter_CM7_GCC_wc16.a
      - libPDMFilter_CM7_GCC_wc16_softfp.a
      - libPDMFilter_CM7_GCC_wc32.a
      - libPDMFilter_CM7_GCC_wc32_softfp.a
      - libPDMFilter_CM7_IAR_wc16.a
      - libPDMFilter_CM7_IAR_wc32.a
      - libPDMFilter_CM7_Keil_wc16.lib
  - Note: New CM7 libs support both CortexM7 core versions for STM32F7 and STM32H7 (STM32H74x/5x, STM32H7Ax/Bx, STM32H72x/3x) series



## Backward Compatibility

None
</div>

:::


::: {.collapse}
<input type="checkbox" id="collapse-section_3.2.0" aria-hidden="true">
<label for="collapse-section_3.2.0" aria-hidden="true">V3.2.0 / 06-November-2019</label>
<div>

## Main Changes

  - Add new PDM Filter libraries compliant with GCC version with FPU = softfp
    - libPDMFilter_CM4_GCC_wc32_softfp.a for GCC with wchar = 4
    - libPDMFilter_CM7_GCC_wc32_softfp.a for GCC with wchar = 4
    - libPDMFilter_CM4_GCC_wc16_softfp.a for GCC with wchar = 2 
    - libPDMFilter_CM7_GCC_wc16_softfp.a for GCC with wchar = 2
  - Add new PDM Filter libraries compliant with GCC version with wchar = 2
    - libPDMFilter_CM3_GCC_wc16.a
    - libPDMFilter_CM4_GCC_wc16.a
    - libPDMFilter_CM7_GCC_wc16.a
  - This PDM Filter libraries cortexM3 version is not backward compatible with previous V3.1.0 and earlier version, below the list of complete changes requiring updates on User applications:
    - For cortexM3 (CM3):
      - libPDMFilter_CM3_IAR.a updated and renamed to:
        - libPDMFilter_CM3_IAR_wc32.a for EWARM V8.x and later versions
        - libPDMFilter_CM3_IAR_wc16.a for EWARM V7.80.x and earlier versions
      - libPDMFilter_CM3_Keil.lib updated and renamed to libPDMFilter_CM3_Keil_wc16.lib
      - libPDMFilter_CM3_GCC.a updated and renamed to libPDMFilter_CM3_GCC_wc32.a

  
## Backward Compatibility

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_3.1.0" aria-hidden="true">
<label for="collapse-section_3.1.0" aria-hidden="true">V3.1.0 / 29-September-2018</label>
<div>

## Main Changes

  - Update PDM Filter libraries for CortexM7 (CM7) and cortexM4 (CM4) core versions.
  - This version is not backward compatible with previous V3.0.1 and earlier version, below the list of complete changes requiring updates on User applications:
    - For cortexM4 (CM4):
      - libPDMFilter_CM4_IAR.a updated and renamed to:
        - libPDMFilter_CM4_IAR_wc32.a for EWARM V8.x and later versions
        - libPDMFilter_CM4_IAR_wc16.a for EWARM V7.80.x and earlier versions
      - libPDMFilter_CM4_Keil.lib updated and renamed to libPDMFilter_CM4_Keil_wc16.lib
      - libPDMFilter_CM4_GCC.a updated and renamed to libPDMFilter_CM4_GCC_wc32.a
    - For cortexM7 (CM7):
      - libPDMFilter_CM7_IAR.a updated and renamed to:
        - libPDMFilter_CM7_IAR_wc32.a for EWARM V8.x and later versions
        - libPDMFilter_CM7_IAR_wc16.a for EWARM V7.80.x and earlier versions
      - libPDMFilter_CM7_Keil.lib updated and renamed to libPDMFilter_CM7_Keil_wc16.lib
      - libPDMFilter_CM7_GCC.a updated and renamed to libPDMFilter_CM7_GCC_wc32.
     
  - Note: New CM7 libs support both CortexM7 core versions for STM32F7 and STM32H7 series


## Backward Compatibility

None
</div>

:::


::: {.collapse}
<input type="checkbox" id="collapse-section_3.0.1" aria-hidden="true">
<label for="collapse-section_3.0.1" aria-hidden="true">V3.0.1 / 29-December-2017</label>
<div>

## Main Changes

- Update libPDMFilter_CM7_Keil.lib to support  CortexM7 (CM7) core version available on STM32F7 family.

  
## Known Limitations

  - The new libPDMFilter_CM7_Keil.lib supports only CortexM7 core version available on STM32F7 family. New version for libPDMFilter_CM7_Keil.lib supporting both CortexM7 core versions (STM32F7 and STM32H7) will be available for next release.
    - The libPDMFilter_CM7_Keil.lib compatible with CortexM7 core version available on STM32H7 family is available on PDM Lib version V3.0.0
  

  
## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_3.0.0" aria-hidden="true">
<label for="collapse-section_3.0.0" aria-hidden="true">V3.0.0 / 28-February-2017</label>
<div>

## Main Changes

  - Major APIs change
  - Only one filtering function is provided PDM_Filter()
  - The decimation factor, endianness, and bit order are now configured once at the PDM_Filter_Init() function call
  - Decimation factor, demodulation loop size and digital gain can be changed on-the-fly using PDM_Filter_setConfig() function
  - This version is not backward compatible with previous V2.1.0 and earlier version, below guidelines on how to use the new APIs Library initialization
  Once the memory is allocated, some routines must be called to initialize the PDM2PCM library static memory:

    - PDM_Filter_Init() routine should be called each time the processing in the audio is stopped and started.
    - PDM_Filter_setConfig() routine should be called at least once before processing start in order to set configurable parameter
  - Furthermore, has the PDM2PCM library runs on STM32 devices, CRC HW block must be enabled and reset.

  - Module execution
    - The run time process can start, when the hardware is configured and the PDM2PCM library is initialized and configured. At each new interrupt, when enough bits have been buffered, the PDM2PCM filter routine PDM_Filter() can be called. Between two consecutive calls to this filter routine, the dynamic parameters can be changed. 

  
## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_2.1.0" aria-hidden="true">
<label for="collapse-section_2.1.0" aria-hidden="true">V2.1.0 / 31-March-2015</label>
<div>

## Main Changes

  - Add Binaries for CortexM7 (CM7) core
  - PDM_Filter_128_MSB() and PDM_Filter_128_LSB() functions: enhance volume setting, it was lower than expected
  - PDM_Filter_64_MSB() function: fix issue when using more than 2 microphones

  
## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_2.0.1" aria-hidden="true">
<label for="collapse-section_2.0.1" aria-hidden="true">V2.0.1 / 17-December-2013</label>
<div>

## Main Changes

  - In HTONS() macro, change u16 type by uint16_t
 
## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_2.0.0" aria-hidden="true">
<label for="collapse-section_2.0.0" aria-hidden="true">V2.0.0 / 07-August-2013</label>
<div>

## Main Changes

  - Update Library binary name format
  - Add Library version optimized for CM0, CM3 and CM4F cores
  - Add functions for decimation factor 128

## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_1.1.2" aria-hidden="true">
<label for="collapse-section_1.1.2" aria-hidden="true">V1.1.2 / 17-April-2012</label>
<div>

## Main Changes

- All source files: license disclaimer text update and add link to the License file on ST Internet.

## Backward Compatibility

None
</div>

:::

::: {.collapse}
<input type="checkbox" id="collapse-section_1.1.1" aria-hidden="true">
<label for="collapse-section_1.1.1" aria-hidden="true">V1.1.1 / 30-December-2011</label>
<div>

## Main Changes

  - PDM Library moved from Utilities\STM32F4-Discovery to Utilities\STM32_Audio\ Addons\ PDM folder
  - Add new license agreement "MCD-ST Image SW License Agreement V2.pdf"

## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_1.1.0" aria-hidden="true">
<label for="collapse-section_1.1.0" aria-hidden="true">V1.1.0 / 28-October-2011</label>
<div>

## Main Changes

  - Add ***PDM audio software decoding Library*** drivers, used to decode and reconstruct the audio signal produced by MP45DT02 MEMS microphone from STMicroelectronics. For more details about this Library, please refer to document ***"PDM audio software decoding on STM32 microcontrollers (AN3998)"***.
  - stm32f4_discovery_audio_codec.c/.h: update to use DMA instead of Interrupt in I2S communication with the external codec.

## Backward Compatibility

None
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section_1.0.0" aria-hidden="true">
<label for="collapse-section_1.0.0" aria-hidden="true">V1.0.0 / 19-September-2011</label>
<div>

## Main Changes

- First official version of the ***STM32F4-Discovery Board Drivers***

## Backward Compatibility

None
</div>
:::

:::
:::
<footer class="sticky">
::: {.columns}
::: {.column width="95%"}
:::
:::
</footer>

