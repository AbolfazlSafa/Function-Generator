/*
 * File:        dds.h
 * Author:      Abolfazl Safagholi
 * Brief:       Direct Digital Synthesis (DDS) engine for waveform generation.
 *
 * Created:     2026-01-01
 * Version:     1.1
 *
 * Email:       ab.safagholi@gmail.com
 * GitHub:      https://github.com/AbolfazlSafa
 *
 */

#ifndef DDS_H_
#define DDS_H_

#include <stdint.h>

// Waveform Types
typedef enum {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH,
} dds_wave_t;

// Musical Notes Frequencies (x100)
typedef enum {
    // Octave 0
    C0  = 1635,  Cs0 = 1732,  D0  = 1835,  Ds0 = 1945,
    E0  = 2060,  F0  = 2183,  Fs0 = 2312,  G0  = 2450,
    Gs0 = 2596,  A0  = 2750,  As0 = 2914,  B0  = 3087,

    // Octave 1
    C1  = 3270,  Cs1 = 3465,  D1  = 3671,  Ds1 = 3889,
    E1  = 4120,  F1  = 4365,  Fs1 = 4625,  G1  = 4900,
    Gs1 = 5191,  A1  = 5500,  As1 = 5827,  B1  = 6174,

    // Octave 2
    C2  = 6541,  Cs2 = 6930,  D2  = 7342,  Ds2 = 7778,
    E2  = 8241,  F2  = 8731,  Fs2 = 9250,  G2  = 9800,
    Gs2 = 10383, A2  = 11000, As2 = 11654, B2  = 12347,

    // Octave 3
    C3  = 13081, Cs3 = 13859, D3  = 14683, Ds3 = 15556,
    E3  = 16481, F3  = 17461, Fs3 = 18500, G3  = 19600,
    Gs3 = 20765, A3  = 22000, As3 = 23308, B3  = 24694,

    // Octave 4
    C4  = 26163, Cs4 = 27718, D4  = 29366, Ds4 = 31113,
    E4  = 32963, F4  = 34923, Fs4 = 36999, G4  = 39200,
    Gs4 = 41530, A4  = 44000, As4 = 46616, B4  = 49388,

    // Octave 5
    C5  = 52325, Cs5 = 55437, D5  = 58733, Ds5 = 62225,
    E5  = 65925, F5  = 69846, Fs5 = 73999, G5  = 78399,
    Gs5 = 83061, A5  = 88000, As5 = 93233, B5  = 98777,

    // Octave 6
    C6  = 104650, Cs6 = 110873, D6  = 117466, Ds6 = 124451,
    E6  = 131851, F6  = 139691, Fs6 = 147998, G6  = 156798,
    Gs6 = 166122, A6  = 176000, As6 = 186466, B6  = 197553,

    // Octave 7
    C7  = 209300, Cs7 = 221746, D7  = 234932, Ds7 = 248900,
    E7  = 263700, F7  = 279383, Fs7 = 295996, G7  = 313596,
    Gs7 = 332240, A7  = 352000, As7 = 372931, B7  = 395107,

    // Octave 8
    C8  = 418601, Cs8 = 443492, D8  = 469863, Ds8 = 497800,
    E8  = 527400, F8  = 558765, Fs8 = 591991, G8  = 627193,
    Gs8 = 664488, A8  = 704000, As8 = 745862, B8  = 790213
} dds_note_t;

// Sounds Structure
typedef struct {
    dds_wave_t wave;
    uint32_t phase_acc;
    uint32_t phase_inc;
    uint8_t  amplitude;
} dds_t;

// Functions
void dds_init(dds_t *dds);
void dds_set_amp_8bit(dds_t *dds, uint8_t amp);
void dds_set_amp_percent(dds_t *dds, float amp);
void dds_set_wave(dds_t *dds, dds_wave_t waveform);
void dds_set_freq(dds_t *dds, float freq_Hz, uint32_t sample_rate);
void dds_set_note(dds_t *dds, dds_note_t note, uint32_t sample_rate);
void dds_out_zero(dds_t *dds);

uint8_t dds_next_sample(dds_t *dds);

#endif
