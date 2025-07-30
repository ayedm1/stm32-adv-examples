#ifndef __AUDIO_USER_H__
#define __AUDIO_USER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ux_api.h"

/* Properties of audio stream */
typedef struct AUDIO_DESCRIPTION_STRUCT
{
  ULONG         audio_frequency;       /* Audio frequency */
  UCHAR         audio_channels_count;  /* Audio channels count */
  SHORT         audio_volume_db_256;   /* Audio volume */
  UCHAR         audio_mute;            /* Audio mute state */
  UCHAR         audio_resolution;      /* Audio resolution */
}AUDIO_DESCRIPTION;

/* Main audio buffer structure */
typedef struct AUDIO_CIRCULAR_BUFFER_STRUCT
{
  UCHAR*        data;     /* Pointer to circular buffer data */
  USHORT        rd_ptr;   /* Circular buffer reading offset */
  USHORT        wr_ptr;   /* Circular buffer writing offset */
  USHORT        size;     /* Size of buffer segment where samples may be read or written */
}AUDIO_CIRCULAR_BUFFER;


/* USBD_AUDIO_BUFFER_FREE_SIZE computes the free size in the circular buffer */
#define USBD_AUDIO_BUFFER_FREE_SIZE(buff)  ((buff.wr_ptr >= buff.rd_ptr)?(buff.rd_ptr + buff.size - buff.wr_ptr):(buff.rd_ptr - buff.wr_ptr))

/* USBD_AUDIO_BUFFER_FILLED_SIZE computes the filled size in the circular buffer */
#define USBD_AUDIO_BUFFER_FILLED_SIZE(buff) ((buff.wr_ptr >= buff.rd_ptr)?(buff.wr_ptr - buff.rd_ptr):(buff.wr_ptr + buff.size - buff.rd_ptr))

/* AUDIO_MS_PACKET_SIZE compute the nominal size(number of bytes) of an audio packet requierd for one millisecond
   for example for audio:
   ** 48KHZ/24 bit/sterio required size is 48*3*2 = 288
   ** 44.1KHZ/16bits/sterio required size is 44*2*2 = 176
   */
#define AUDIO_MS_PACKET_SIZE(freq, channel_count, res_byte) (((uint32_t)((freq) /1000)) * (channel_count) * (res_byte))

/* USBD_AUDIO_USB_PACKET_SIZE_HS computes the nominal size of audio packet in HS mode(it uses the floor of frequency fractional part) */
#define AUDIO_USB_PACKET_SIZE_HS(audio_desc) (((uint32_t)(((audio_desc)->audio_frequency) /8000))* ((audio_desc)->audio_channels_count) * ((audio_desc)->audio_resolution))

/* USBD_AUDIO_USB_MAX_PACKET_SIZE_HS computes the nominal size of audio packet in HS mode(it uses the ceil of frequecy fractional part) */
#define USBD_AUDIO_USB_MAX_PACKET_SIZE_HS(audio_desc) USBD_AUDIO_USB_PACKET_SIZE_HS(((audio_desc)->audio_frequency)+7999, ((audio_desc)->audio_channels_count), ((audio_desc)->audio_resolution))

/* USBD_AUDIO_USB_PACKET_SIZE_FS computes the nominal size of audio packet in HS mode(it uses the floor of frequency fractional part) */
#define USBD_AUDIO_PACKET_SIZE_FS(freq, channel_count, res_byte) (((uint32_t)((freq) /1000))* (channel_count) * (res_byte))

/* USBD_AUDIO_USB_MAX_PACKET_SIZE_FS computes the nominal size of audio packet in HS mode(it uses the ceil of frequecy fractional part) */
#define USBD_AUDIO_MAX_PACKET_SIZE_FS(freq, channel_count, res_byte) USBD_AUDIO_PACKET_SIZE_FS(freq+999, channel_count, res_byte)

#define VOLUME_DB_256_TO_PERCENT(volume_db_256) ((uint8_t)((((INT)(volume_db_256) - USBD_AUDIO_VOLUME_SPEAKER_MIN_DB_256)*100)/ \
                                                          (USBD_AUDIO_VOLUME_SPEAKER_MAX_DB_256 - USBD_AUDIO_VOLUME_SPEAKER_MIN_DB_256)))

#define AUDIO_SAMPLE_LENGTH(audio_desc)  (USBD_AUDIO_RESOLUTION_BIT(audio_desc -> audio_resolution) == USBD_PLAY_RES_BIT_16B) ?          \
                                                             ((audio_desc)->audio_channels_count * (audio_desc)->audio_resolution) :     \
                                                             ((audio_desc)->audio_channels_count * ((audio_desc)->audio_resolution + 1))


#define AUDIO_USB_PACKET_SIZE_FROM_AUD_DESC(audio_desc)         USBD_AUDIO_PACKET_SIZE_FS((audio_desc)->audio_frequency,      \
                                                                                          (audio_desc)->audio_channels_count, \
                                                                                          (audio_desc)->audio_resolution)


#define AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc)     USBD_AUDIO_MAX_PACKET_SIZE_FS((audio_desc)->audio_frequency,      \
                                                                                              (audio_desc)->audio_channels_count, \
                                                                                              (audio_desc)->audio_resolution)

#define AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(audio_desc)         AUDIO_MS_PACKET_SIZE((audio_desc)->audio_frequency,       \
                                                                                    (audio_desc)->audio_channels_count,  \
                                                                                    (audio_desc)->audio_resolution)

#define AUDIO_MS_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc)     AUDIO_MS_PACKET_SIZE((audio_desc)->audio_frequency + 999, \
                                                                                    (audio_desc)->audio_channels_count,  \
                                                                                    (audio_desc)->audio_resolution)

#define USBD_AUDIO_RESOLUTION_BIT(audio_res)                    (audio_res * 8)



#define AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc)     (USBD_AUDIO_RESOLUTION_BIT(audio_desc -> audio_resolution) == USBD_PLAY_RES_BIT_16B) ?   \
                                                                            AUDIO_MS_PACKET_SIZE((audio_desc)->audio_frequency,                 \
                                                                                                 (audio_desc)->audio_channels_count,            \
                                                                                                 (audio_desc)->audio_resolution) :              \
                                                                            AUDIO_MS_PACKET_SIZE((audio_desc)->audio_frequency,                 \
                                                                                                 (audio_desc)->audio_channels_count,            \
                                                                                                 (audio_desc)->audio_resolution+1)

#define AUDIO_SPEAKER_MAX_INJECTION_LENGTH(audio_desc) (((audio_desc)->audio_frequency == USBD_AUDIO_FREQ_44_1_K) ?                                        \
                                                                            AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc) + AUDIO_SAMPLE_LENGTH(audio_desc) : \
                                                                            AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc))

#ifdef __cplusplus
}
#endif
#endif /* __AUDIO_USER_H__ */
