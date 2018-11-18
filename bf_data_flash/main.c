

/**
 * main.c
 */

#include <F28x_Project.h>
#include <fpu_filter.h>
#include "setup.h"
#include "hilbert_transformer.h"
#include "bf.h"
#include  "circular_buffer.h"
// #define INPUT_SIGNAL_LENGTH 41600
#define INPUT_SIGNAL_LENGTH 32768
#define TEST_SIGNAL_SIZE 1000
/*
 * Declara uma seção na memoria com um nome. O linker deve ser alterado e a memoria alinhada, de acordo com as instrucoes da biblioteca
 */
// Secoes para dados de teste

#pragma DATA_SECTION(mic1_in,"mic1_in");
const int16_t mic1_in[INPUT_SIGNAL_LENGTH] = {
#include <mic1_pcm.h>
};
#pragma DATA_SECTION(mic2_in,"mic2_in");
const int16_t mic2_in[INPUT_SIGNAL_LENGTH] = {
#include <mic2_pcm.h>
};
#pragma DATA_SECTION(mic3_in,"mic3_in");
const int16_t mic3_in[INPUT_SIGNAL_LENGTH] = {
#include <mic3_pcm.h>
};
#pragma DATA_SECTION(mic4_in,"mic4_in");
const int16_t mic4_in[INPUT_SIGNAL_LENGTH] = {
#include <mic4_pcm.h>
};
//---------------------------------------------------------------------

#pragma DATA_SECTION(bf_out_vector, "bf_test_output");
complex_float bf_out_vector[TEST_SIGNAL_SIZE];
//#pragma DATA_SECTION(bf_in_vector, "bf_test_input");
//complex_float bf_in_vector[TEST_SIGNAL_SIZE-HILBERT_DELAY][NUMERO_MICS];
complex_float bf_in[NUMERO_MICS];
complex_float bf_out;

//----------------------------------------------------------------
// Coeficientes da transformada de Hilbert. Os mesmos para todos os casos
#pragma DATA_SECTION(coeffs_hilb, "coeffhilb");
float coeffs_hilb[HILBERT_TRANSFORMER_ORDER+1] = {
#include "coeffs_hilbert.h"
};
//----------------------------------------------------------------
// Secao para declaracao dos filtros. Sao criados 4, cada um com buffer proprio, para cada canal de microfone
#pragma DATA_SECTION(hilb_chnl0, "hilbfilt0")
FIR_FP  hilb_chnl0 = FIR_FP_DEFAULTS;
FIR_FP_Handle hnd_hilb_chnl0 = &hilb_chnl0;
#pragma DATA_SECTION(hilb_buffer_chnl0, "hilbbuffer0");
float hilb_buffer_chnl0[HILBERT_TRANSFORMER_ORDER+1];

#pragma DATA_SECTION(hilb_chnl1, "hilbfilt1")
FIR_FP  hilb_chnl1 = FIR_FP_DEFAULTS;
FIR_FP_Handle hnd_hilb_chnl1 = &hilb_chnl1;
#pragma DATA_SECTION(hilb_buffer_chnl1, "hilbbuffer1");
float hilb_buffer_chnl1[HILBERT_TRANSFORMER_ORDER+1];

#pragma DATA_SECTION(hilb_chnl2, "hilbfilt2")
FIR_FP  hilb_chnl2 = FIR_FP_DEFAULTS;
FIR_FP_Handle hnd_hilb_chnl2 = &hilb_chnl2;
#pragma DATA_SECTION(hilb_buffer_chnl2, "hilbbuffer2");
float hilb_buffer_chnl2[HILBERT_TRANSFORMER_ORDER+1];

#pragma DATA_SECTION(hilb_chnl3, "hilbfilt3")
FIR_FP  hilb_chnl3 = FIR_FP_DEFAULTS;
FIR_FP_Handle hnd_hilb_chnl3 = &hilb_chnl3;
#pragma DATA_SECTION(hilb_buffer_chnl3, "hilbbuffer3");
float hilb_buffer_chnl3[HILBERT_TRANSFORMER_ORDER+1];
//------------------------------------------------------------------------------

#pragma DATA_SECTION(hilbert_out_chnl0_vector, "hilb_out_vector");
float hilbert_out_chnl0_vector[TEST_SIGNAL_SIZE];



int main(void)
{
    FPU_initSystemClocks();
    InitSysPll(XTAL_OSC,IMULT_20,FMULT_0,PLLCLK_BY_1);
    FPU_initEpie();

    uint32_t idx_out = 0;

    circ_buf_t cb_chnl0;
    circ_buf_handler cb_hnd_chnl0 = &cb_chnl0;
    circ_buf_t cb_chnl1;
    circ_buf_handler cb_hnd_chnl1 = &cb_chnl1;
    circ_buf_t cb_chnl2;
    circ_buf_handler cb_hnd_chnl2 = &cb_chnl2;
    circ_buf_t cb_chnl3;
    circ_buf_handler cb_hnd_chnl3 = &cb_chnl3;

    float cb_mic_in_chnl0[HILBERT_DELAY+1];
    float cb_mic_in_chnl1[HILBERT_DELAY+1];
    float cb_mic_in_chnl2[HILBERT_DELAY+1];
    float cb_mic_in_chnl3[HILBERT_DELAY+1];

    float mic_in_chnl0;
    float mic_in_chnl1;
    float mic_in_chnl2;
    float mic_in_chnl3;

    float hilbert_out_chnl0;
    float hilbert_out_chnl1;
    float hilbert_out_chnl2;
    float hilbert_out_chnl3;

    //Inicializacao dos buffers circulares
    cb_inicializar(cb_hnd_chnl0, HILBERT_DELAY+1, cb_mic_in_chnl0);
    cb_inicializar(cb_hnd_chnl1, HILBERT_DELAY+1, cb_mic_in_chnl1);
    cb_inicializar(cb_hnd_chnl2, HILBERT_DELAY+1, cb_mic_in_chnl2);
    cb_inicializar(cb_hnd_chnl3, HILBERT_DELAY+1, cb_mic_in_chnl3);

    // Inicializacao da transformada de Hilbert
    hilbert_transformer_create(hnd_hilb_chnl0, hilb_buffer_chnl0,coeffs_hilb);
    hilbert_transformer_create(hnd_hilb_chnl1, hilb_buffer_chnl1,coeffs_hilb);
    hilbert_transformer_create(hnd_hilb_chnl2, hilb_buffer_chnl2,coeffs_hilb);
    hilbert_transformer_create(hnd_hilb_chnl3, hilb_buffer_chnl3,coeffs_hilb);

    // Variaveis auxiliares para loops
    uint16_t idx_bf;

    uint32_t idx_hilb = 0;

    // Aplicando o filtro FIR para a Transformada de Hilbert
    for (idx_hilb = 0; idx_hilb < HILBERT_DELAY;idx_hilb++){
        // Aplicacao da Transformada de Hilbert, para os
        mic_in_chnl0 = (float) mic1_in[idx_hilb]/32768;
        cb_push(cb_hnd_chnl0, mic_in_chnl0);
        hilbert_transformer(mic_in_chnl0, hnd_hilb_chnl0);

        mic_in_chnl1 = (float) mic2_in[idx_hilb]/32768;
        cb_push(cb_hnd_chnl1, mic_in_chnl1);
        hilbert_transformer(mic_in_chnl1, hnd_hilb_chnl1);

        mic_in_chnl2 = (float) mic3_in[idx_hilb]/32768;
        cb_push(cb_hnd_chnl2, mic_in_chnl2);
        hilbert_transformer(mic_in_chnl2, hnd_hilb_chnl2);

        mic_in_chnl3 = (float) mic4_in[idx_hilb]/32768;
        cb_push(cb_hnd_chnl3, mic_in_chnl3);
        hilbert_transformer(mic_in_chnl3, hnd_hilb_chnl3);
    }

    for (idx_bf = 0; idx_bf < INPUT_SIGNAL_LENGTH-2*HILBERT_DELAY;idx_bf++){
        // Aplicacao da Transformada de Hilbert
        mic_in_chnl0 = (float) mic1_in[idx_bf+HILBERT_DELAY]/32768;
        cb_push(cb_hnd_chnl0, mic_in_chnl0);
        hilbert_out_chnl0 = hilbert_transformer(mic_in_chnl0, hnd_hilb_chnl0);
        if (idx_out < TEST_SIGNAL_SIZE){
             hilbert_out_chnl0_vector[idx_out] = hilbert_out_chnl0;
         }

        mic_in_chnl1 = (float) mic2_in[idx_bf+HILBERT_DELAY]/32768;
        cb_push(cb_hnd_chnl1, mic_in_chnl1);
        hilbert_out_chnl1 = hilbert_transformer(mic_in_chnl1, hnd_hilb_chnl1);

        mic_in_chnl2 = (float) mic3_in[idx_bf+HILBERT_DELAY]/32768;
        cb_push(cb_hnd_chnl2, mic_in_chnl2);
        hilbert_out_chnl2 = hilbert_transformer(mic_in_chnl2, hnd_hilb_chnl2);

        mic_in_chnl3 = (float) mic4_in[idx_bf+HILBERT_DELAY]/32768;
        cb_push(cb_hnd_chnl3, mic_in_chnl3);
        hilbert_out_chnl3 = hilbert_transformer(mic_in_chnl3, hnd_hilb_chnl3);


        // USo da Transformada de Hilbert para gerar numeros complexos para o beamformer
            bf_in[0].dat[0] = cb_pop(cb_hnd_chnl0);
            bf_in[0].dat[1] = hilbert_out_chnl0;
            bf_in[1].dat[0] = cb_pop(cb_hnd_chnl1);
            bf_in[1].dat[1] = hilbert_out_chnl1;
            bf_in[2].dat[0] = cb_pop(cb_hnd_chnl2);
            bf_in[2].dat[1] = hilbert_out_chnl2;
            bf_in[3].dat[0] = cb_pop(cb_hnd_chnl3);
            bf_in[3].dat[1] = hilbert_out_chnl3;


           // uint16_t idx_mic;
           // if (idx_out < TEST_SIGNAL_SIZE){
           //     for (idx_mic = 0; idx_mic < NUMERO_MICS; idx_mic++){
           //         bf_in_vector[idx_bf][idx_mic] = bf_in[idx_mic];
           //      }
           // }

        // Aplicacao dos pesos do Beamformer
            bf_out = bf_aplicar(bf_in);
            if (idx_out < TEST_SIGNAL_SIZE){
                bf_out_vector[idx_out] = bf_out;
                idx_out++;
            }

    }
    for(;;);
    done();
	return 0;
}
