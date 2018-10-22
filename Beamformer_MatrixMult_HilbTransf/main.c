

/**
 * main.c
 */

#include <F28x_Project.h>
#include <fpu_filter.h>
#include "setup.h"
#include "hilbert_transformer.h"
#include "bf.h"

#define TEST_SIGNAL_SIZE 160
/*
 * Declara uma seção na memoria com um nome. O linker deve ser alterado e a memoria alinhada, de acordo com as instrucoes da biblioteca
 */
// Secoes para dados de teste
#pragma DATA_SECTION(test_mic_in_chnl0, "sigIn0");
#pragma DATA_SECTION(test_out_signal_chnl0, "sigOut0");
const float test_mic_in_chnl0[TEST_SIGNAL_SIZE] = {
#include <test_input_signal_chnl0.h>
};
const float test_out_signal_chnl0[TEST_SIGNAL_SIZE] = {
#include <test_output_signal_chnl0.h>
};

#pragma DATA_SECTION(test_mic_in_chnl1, "sigIn1");
#pragma DATA_SECTION(test_out_signal_chnl1, "sigOut1");
const float test_mic_in_chnl1[TEST_SIGNAL_SIZE] = {
#include <test_input_signal_chnl1.h>
};
const float test_out_signal_chnl1[TEST_SIGNAL_SIZE] = {
#include <test_output_signal_chnl1.h>
};

#pragma DATA_SECTION(test_mic_in_chnl2, "sigIn2");
#pragma DATA_SECTION(test_out_signal_chnl2, "sigOut2");
const float test_mic_in_chnl2[TEST_SIGNAL_SIZE] = {
#include <test_input_signal_chnl2.h>
};
const float test_out_signal_chnl2[TEST_SIGNAL_SIZE] = {
#include <test_output_signal_chnl2.h>
};

#pragma DATA_SECTION(test_mic_in_chnl3, "sigIn3");
#pragma DATA_SECTION(test_out_signal_chnl3, "sigOut3");
const float test_mic_in_chnl3[TEST_SIGNAL_SIZE] = {
#include <test_input_signal_chnl3.h>
};
const float test_out_signal_chnl3[TEST_SIGNAL_SIZE] = {
#include <test_output_signal_chnl3.h>
};
//---------------------------------------------------------------------

#pragma DATA_SECTION(bf_out_vector, "bf_test_output");
complex_float bf_out_vector[TEST_SIGNAL_SIZE-HILBERT_DELAY];
#pragma DATA_SECTION(bf_in_vector, "bf_test_input");
complex_float bf_in_vector[TEST_SIGNAL_SIZE-HILBERT_DELAY][NUMERO_MICS];
complex_float bf_in[NUMERO_MICS];
complex_float bf_out;

//----------------------------------------------------------------
// Coeficientes da transformada de Hilbert. Os mesmos para todos os casos
#pragma DATA_SECTION(coeffs_hilb, "coeffhilb");
float coeffs_hilb[HILBERT_TRANSFORMER_ORDER+1] = {
#include "coeffs_hilbert_negativo.h"
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



float hilbert_out_signal_chnl0[TEST_SIGNAL_SIZE];
float hilbert_out_signal_chnl1[TEST_SIGNAL_SIZE];
float hilbert_out_signal_chnl2[TEST_SIGNAL_SIZE];
float hilbert_out_signal_chnl3[TEST_SIGNAL_SIZE];

int main(void)
{
    FPU_initSystemClocks();
    FPU_initEpie();

    // Inicializacao da transformada de Hilbert
    hilbert_transformer_create(hnd_hilb_chnl0, hilb_buffer_chnl0,coeffs_hilb);
    hilbert_transformer_create(hnd_hilb_chnl1, hilb_buffer_chnl1,coeffs_hilb);
    hilbert_transformer_create(hnd_hilb_chnl2, hilb_buffer_chnl2,coeffs_hilb);
    hilbert_transformer_create(hnd_hilb_chnl3, hilb_buffer_chnl3,coeffs_hilb);

    // Aplicando o filtro FIR para a Transformada de Hilbert
    uint32_t idx_hilb = 0;

    for (idx_hilb = 0; idx_hilb < HILBERT_DELAY;idx_hilb++){
        // Aplicacao da Transformada de Hilbert, para os
        hilbert_out_signal_chnl0[idx_hilb]= hilbert_transformer(test_mic_in_chnl0[idx_hilb], hnd_hilb_chnl0);
        hilbert_out_signal_chnl1[idx_hilb]= hilbert_transformer(test_mic_in_chnl1[idx_hilb], hnd_hilb_chnl1);
        hilbert_out_signal_chnl2[idx_hilb]= hilbert_transformer(test_mic_in_chnl2[idx_hilb], hnd_hilb_chnl2);
        hilbert_out_signal_chnl3[idx_hilb]= hilbert_transformer(test_mic_in_chnl3[idx_hilb], hnd_hilb_chnl3);
    }
    for (idx_hilb = HILBERT_DELAY; idx_hilb < TEST_SIGNAL_SIZE;idx_hilb++){
        // Aplicacao da Transformada de Hilbert
        hilbert_out_signal_chnl0[idx_hilb]= hilbert_transformer(test_mic_in_chnl0[idx_hilb], hnd_hilb_chnl0);
        hilbert_out_signal_chnl1[idx_hilb]= hilbert_transformer(test_mic_in_chnl1[idx_hilb], hnd_hilb_chnl1);
        hilbert_out_signal_chnl2[idx_hilb]= hilbert_transformer(test_mic_in_chnl2[idx_hilb], hnd_hilb_chnl2);
        hilbert_out_signal_chnl3[idx_hilb]= hilbert_transformer(test_mic_in_chnl3[idx_hilb], hnd_hilb_chnl3);
        }


        // USo da Transformada de Hilbert para gerar numeros complexos para o beamformer
    uint16_t idx_bf;
    uint16_t idx_mic;
    for (idx_bf = 0; idx_bf <TEST_SIGNAL_SIZE - HILBERT_DELAY; idx_bf++ ){
            for (idx_mic = 0; idx_mic < NUMERO_MICS; idx_mic++){
            bf_in[0].dat[0] = test_mic_in_chnl0[idx_bf];
            bf_in[0].dat[1] = hilbert_out_signal_chnl0[idx_bf+HILBERT_DELAY];
            bf_in[1].dat[0] = test_mic_in_chnl1[idx_bf];
            bf_in[1].dat[1] = hilbert_out_signal_chnl1[idx_bf+HILBERT_DELAY];
            bf_in[2].dat[0] = test_mic_in_chnl2[idx_bf];
            bf_in[2].dat[1] = hilbert_out_signal_chnl2[idx_bf+HILBERT_DELAY];
            bf_in[3].dat[0] = test_mic_in_chnl3[idx_bf];
            bf_in[3].dat[1] = hilbert_out_signal_chnl3[idx_bf+HILBERT_DELAY];
            for (idx_mic = 0; idx_mic < NUMERO_MICS; idx_mic++){
                bf_in_vector[idx_bf][idx_mic] = bf_in[idx_mic];
                }
            }
    };

        // Aplicacao do Beamformer
        for (idx_bf = 0 ; idx_bf < TEST_SIGNAL_SIZE-HILBERT_DELAY ; idx_bf++){
            for (idx_mic = 0 ; idx_mic < NUMERO_MICS ; idx_mic++){
            bf_in[idx_mic].dat[0] = bf_in_vector[idx_bf][idx_mic].dat[0];
            bf_in[idx_mic].dat[1] = bf_in_vector[idx_bf][idx_mic].dat[1];
            }
            bf_out = bf_aplicar(bf_in);
            bf_out_vector[idx_bf] = bf_out;

         }


    for(;;);
    done();
	return 0;
}
