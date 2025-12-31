/*
 * File:        dds_wave_tables.h
 * Author:      Abolfazl Safagholi
 * Brief:       External declarations for DDS waveform lookup tables.
 *
 * Created:     2026-01-01
 * Version:     1.0
 *
 * Email:       ab.safagholi@gmail.com
 * GitHub:      https://github.com/AbolfazlSafa
 *
 * License:     MIT License
 *
 */

#ifndef DDS_WAVE_TABLES_
#define DDS_WAVE_TABLES_

#include <stdint.h>

// Waveforms Look-up Tables
extern const uint8_t wave_sin[256];
extern const uint8_t wave_sqr[256];
extern const uint8_t wave_tri[256];
extern const uint8_t wave_saw[256];

#endif