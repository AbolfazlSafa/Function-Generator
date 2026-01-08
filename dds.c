/*
 * File:        dds.c
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

#include "dds.h"
#include "dds_wave_tables.h"

void dds_init(dds_t *dds)
{
    dds->phase_acc = 0;
    dds->phase_inc = 0;
    dds->amplitude = 255;
    dds->wave  = SINE;
}

void dds_set_amp_8bit(dds_t *dds, uint8_t amp)
{
    dds->amplitude = amp;
}

void dds_set_amp_percent(dds_t *dds, float amp)
{
    if (amp < 0)
        amp = 0;
    if (amp > 100)
        amp = 100;
    
    dds->amplitude = (uint8_t)(amp*2.55f);
}

void dds_set_wave(dds_t *dds, dds_wave_t waveform)
{
    dds->wave = waveform;
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

void dds_out_zero(dds_t *dds)
{
	dds->phase_inc = 0;
    dds->phase_acc = 0;
}

uint8_t dds_next_sample(dds_t *dds)
{
    dds->phase_acc += dds->phase_inc;
    uint8_t index = dds->phase_acc >> 24;

    switch (dds->wave) {
    case SINE:
        return (wave_sin[index] * dds->amplitude) >> 8;
    case SQUARE:
        return (wave_sqr[index] * dds->amplitude) >> 8;
    case TRIANGLE:
        return (wave_tri[index] * dds->amplitude) >> 8;
    case SAWTOOTH:
        return (wave_saw[index] * dds->amplitude) >> 8;
    default:
        return 0;
    }
    return 0;
}
