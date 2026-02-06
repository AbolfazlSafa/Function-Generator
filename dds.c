/*
 * File:        dds.c
 * Author:      Abolfazl Safagholi
 * Brief:       Direct Digital Synthesis (DDS) engine for waveform generation.
 *
 * Created:     2026-01-01
 * Version:     2.0
 *
 * Email:       ab.safagholi@gmail.com
 * GitHub:      https://github.com/AbolfazlSafa
 *
 */

#include "dds.h"
#include "dds_wave_tables.h"

void dds_init(dds_t *dds)
{
    if (!dds)
        return;

    dds->phase_acc = 0;
    dds->phase_inc = 0;
    dds->user_func = 0;
    dds->amplitude = 255;
    dds->wave      = DDS_SINE;
    dds->brsq      = DDS_8_BIT_256_SAMPLE;
    dds->intp      = DDS_INTERPOLATION_OFF;
}

void dds_set_amp_8bit(dds_t *dds, uint8_t amp)
{
    if (!dds)
        return;

    dds->amplitude = amp;
}

void dds_set_amp_percent(dds_t *dds, float amp)
{
    if (!dds)
        return;

    if (amp < 0)
        amp = 0;
    if (amp > 100)
        amp = 100;

    dds->amplitude = (uint8_t)(amp*2.55f);
}

void dds_set_wave(dds_t* dds, dds_wave_t waveform)
{
    if (!dds)
        return;

    if (waveform > DDS_USER_DEFINED)
        return;
    dds->wave = waveform;
}

void dds_set_bitR_sampleQ(dds_t* dds, dds_brsq_t bs)
{
    if (!dds)
        return;

    if (bs > DDS_16_BIT_1024_SAMPLE)
        return;

    dds->brsq = bs;
    dds->phase_acc = 0;
}

void dds_set_interpolation(dds_t* dds, dds_intp_t ip)
{
    if (!dds)
        return;

    if (ip > DDS_INTERPOLATION_QUADRATIC)
        return;

    dds->intp = ip;
}

void dds_set_freq(dds_t *dds, float freq_Hz, uint32_t sample_rate)
{
    if (!dds)
        return;

    dds->phase_inc = (uint32_t)((freq_Hz * 4294967296.0f) / sample_rate);
}

void dds_set_note(dds_t *dds, dds_note_t note, uint32_t sample_rate)
{
    if (!dds)
        return;

    float n = note / 100.0f;
    dds_set_freq(dds, n, sample_rate);
}

void dds_clear_user_wave(dds_t* dds)
{
    if (!dds)
        return;

    dds->user_func = 0;
    dds->wave = DDS_SINE;
}

void dds_set_user_wave(dds_t* dds, dds_user_func_t func)
{
    if (!dds)
        return;

    dds->user_func = func;
    dds->wave = DDS_USER_DEFINED;
}

void dds_set_user_phase_inc(dds_t* dds, uint32_t inc)
{
    if (!dds)
        return;

    dds->phase_inc = inc;
}

void dds_out_zero(dds_t* dds)
{
    if (!dds)
        return;

    dds->phase_inc = 0;
    dds->phase_acc = 0;
}

uint16_t dds_next_sample(dds_t* dds)
{
    if (!dds)
        return 0;

    uint16_t index, frac;

    dds->phase_acc += dds->phase_inc;

    if (dds->wave == DDS_USER_DEFINED && dds->user_func) {
        uint16_t s = dds->user_func(dds->phase_acc);
        return (s * dds->amplitude) >> 8;
    }

    if (dds->brsq == DDS_8_BIT_256_SAMPLE) {
        index = dds->phase_acc >> 24;
        switch (dds->wave) {
        case DDS_SINE:
            return (wave_sin_u8_256[index] * dds->amplitude) >> 8;
        case DDS_SQUARE:
            return (wave_sqr_u8_256[index] * dds->amplitude) >> 8;
        case DDS_TRIANGLE:
            return (wave_tri_u8_256[index] * dds->amplitude) >> 8;
        case DDS_SAWTOOTH:
            return (wave_saw_u8_256[index] * dds->amplitude) >> 8;
        default:
            return 0;
        }
    }

    index = dds->phase_acc >> 22;
    frac = (dds->phase_acc >> 12) & 0x3FF;

    const uint8_t* table_8bit = 0;
    const uint16_t* table_16bit = 0;

    switch (dds->wave) {
    case DDS_SINE:
        table_8bit = wave_sin_u8_1024;
        table_16bit = wave_sin_u16_1024;
        break;
    case DDS_SQUARE:
        table_8bit = wave_sqr_u8_1024;
        table_16bit = wave_sqr_u16_1024;
        break;
    case DDS_TRIANGLE:
        table_8bit = wave_tri_u8_1024;
        table_16bit = wave_tri_u16_1024;
        break;
    case DDS_SAWTOOTH:
        table_8bit = wave_saw_u8_1024;
        table_16bit = wave_saw_u16_1024;
        break;
    default:
        return 0;
    }

    if (dds->brsq == DDS_8_BIT_1024_SAMPLE) {
        switch (dds->intp) {
        case DDS_INTERPOLATION_OFF:
            return (table_8bit[index] * dds->amplitude) >> 8;
        case DDS_INTERPOLATION_LINEAR: {
            uint8_t a = table_8bit[index];
            uint8_t b = table_8bit[(index + 1) & 1023];

            int16_t diff = (int16_t)b - (int16_t)a;
            uint8_t interp = (uint8_t)(a + ((diff * frac) >> 10));

            return (interp * dds->amplitude) >> 8;
        }
        case DDS_INTERPOLATION_QUADRATIC: {
            uint8_t p0 = table_8bit[index];
            uint8_t p1 = table_8bit[(index + 1) & 1023];
            uint8_t p2 = table_8bit[(index + 2) & 1023];

            int32_t t = frac;
            int32_t t2 = (t * t) >> 10;

            int32_t d1 = (int32_t)p2 - (int32_t)p0;
            int32_t d2 = (int32_t)p0 - 2 * (int32_t)p1 + (int32_t)p2;

            int32_t y = (int32_t)p1
                        + ((d1 * t) >> 11)
                        + ((d2 * t2) >> 11);

            if (y < 0)   y = 0;
            if (y > 255) y = 255;

            return (y * dds->amplitude) >> 8;

        }
        default:
            return (table_8bit[index] * dds->amplitude) >> 8;
        }
    } else if (dds->brsq == DDS_16_BIT_1024_SAMPLE) {
        switch (dds->intp) {
        case DDS_INTERPOLATION_OFF:
            return (table_16bit[index] * dds->amplitude) >> 8;
        case DDS_INTERPOLATION_LINEAR: {
            uint16_t a = table_16bit[index];
            uint16_t b = table_16bit[(index + 1) & 1023];

            int32_t diff = (int32_t)b - (int32_t)a;
            uint16_t interp = (uint16_t)(a + ((diff * frac) >> 10));

            return (interp * dds->amplitude) >> 8;
        }
        case DDS_INTERPOLATION_QUADRATIC: {
            uint16_t p0 = table_16bit[index];
            uint16_t p1 = table_16bit[(index + 1) & 1023];
            uint16_t p2 = table_16bit[(index + 2) & 1023];

            int32_t t = frac;
            int32_t t2 = (t * t) >> 10;

            int32_t d1 = (int32_t)p2 - (int32_t)p0;
            int32_t d2 = (int32_t)p0 - 2 * (int32_t)p1 + (int32_t)p2;

            int32_t y = (int32_t)p1
                        + ((d1 * t) >> 11)
                        + ((d2 * t2) >> 11);

            if (y < 0)       y = 0;
            if (y > 65535)   y = 65535;

            return (y * dds->amplitude) >> 8;

        }
        default:
            return (table_16bit[index] * dds->amplitude) >> 8;
        }
    }

    return 0;
}
