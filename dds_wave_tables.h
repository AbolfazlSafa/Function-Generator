/*
 * File:        dds_wave_tables.h
 * Author:      Abolfazl Safagholi
 * Brief:       External declarations for DDS waveform lookup tables.
 *
 * Created:     2026-01-01
 * Version:     2.0
 *
 * Email:       ab.safagholi@gmail.com
 * GitHub:      https://github.com/AbolfazlSafa
 *
 */

#ifndef DDS_WAVE_TABLES_
#define DDS_WAVE_TABLES_

#include <stdint.h>

// Waveforms Look-up Tables

// 8-bit, 256 Samples
extern const uint8_t wave_sin_u8_256[256];
extern const uint8_t wave_sqr_u8_256[256];
extern const uint8_t wave_tri_u8_256[256];
extern const uint8_t wave_saw_u8_256[256];

// 8-bit, 1024 Samples
extern const uint8_t wave_sin_u8_1024[1024];
extern const uint8_t wave_sqr_u8_1024[1024];
extern const uint8_t wave_tri_u8_1024[1024];
extern const uint8_t wave_saw_u8_1024[1024];

// 16-bit, 1024 Samples
extern const uint16_t wave_sin_u16_1024[1024];
extern const uint16_t wave_sqr_u16_1024[1024];
extern const uint16_t wave_tri_u16_1024[1024];
extern const uint16_t wave_saw_u16_1024[1024];

#endif
