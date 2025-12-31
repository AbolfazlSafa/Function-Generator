/*
 * File:        dds.c
 * Author:      Abolfazl Safagholi
 * Brief:       Direct Digital Synthesis (DDS) engine for waveform generation.
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

#include "dds.h"
#include "dds_wave_tables.h"

void dds_init(dds_t *dds)
{
    dds->phase_acc = 0;
    dds->phase_inc = 0;
    dds->wave  = SINE;
}

void dds_set_freq(dds_t *dds, float freq_Hz, uint32_t sample_rate)
{
    dds->phase_inc = (uint32_t)((freq_Hz * 4294967296.0f) / sample_rate);
}

void dds_set_note(dds_t *dds, dds_note_t note, uint32_t sample_rate)
{
    float n = note / 100.0f;
    dds_set_freq(dds, n, sample_rate);
}

void dds_set_wave(dds_t *dds, dds_wave_t waveform)
{
    dds->wave = waveform;
}

uint8_t dds_next_sample(dds_t *dds)
{
    dds->phase_acc += dds->phase_inc;
    uint8_t index = dds->phase_acc >> 24;

    switch (dds->wave) {
    case SINE:
        return wave_sin[index];
    case SQUARE:
        return wave_sqr[index];
    case TRIANGLE:
        return wave_tri[index];
    case SAWTOOTH:
        return wave_saw[index];
    default:
        return 0;
    }
    return 0;
}
