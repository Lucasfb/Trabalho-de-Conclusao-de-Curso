

/**
 * main.c
 */

#include "F28x_Project.h"
#include "fpu_filter.h"
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


#pragma DATA_SECTION(bf_out_vector, "bf_test_output");
complex_float bf_out_vector[TEST_SIGNAL_SIZE];
#pragma DATA_SECTION(bf_in_vector, "bf_test_input");
complex_float bf_in_vector[TEST_SIGNAL_SIZE][NUMERO_MICS];

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

    // Variavel para teste de BF
    static complex_float bf_pesos[NUMERO_MICS] = {
        #include <pesos_teste.h>
    };
    complex_float bf_output;

    complex_float bf_input[NUMERO_MICS];

    memset_fast(bf_input, 0, NUMERO_MICS*sizeof(complex_float));

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
        // USo da Transformada de Hilbert para gerar numeros complexos
        bf_input[0].dat[0] = test_mic_in_chnl0[idx_hilb-HILBERT_DELAY];
        bf_input[0].dat[1] = hilbert_out_signal_chnl0[idx_hilb];
        bf_input[1].dat[0] = test_mic_in_chnl1[idx_hilb-HILBERT_DELAY];
        bf_input[1].dat[1] = hilbert_out_signal_chnl1[idx_hilb];
        bf_input[2].dat[0] = test_mic_in_chnl2[idx_hilb-HILBERT_DELAY];
        bf_input[2].dat[1] = hilbert_out_signal_chnl2[idx_hilb];
        bf_input[3].dat[0] = test_mic_in_chnl3[idx_hilb-HILBERT_DELAY];
        bf_input[3].dat[1] = hilbert_out_signal_chnl3[idx_hilb];
        uint16_t idx_mic = 0;
        for (idx_mic = 0 ; idx_mic < 4;idx_mic++ ){
            bf_in_vector[idx_hilb-HILBERT_DELAY][idx_mic] = bf_input[idx_mic];
        }


        // Aplicacao do Beamformer
        uint16_t j = 0;
        complex_float bf_sum = {0,0};
        for (j = 0 ; j < NUMERO_MICS ; j++){
                        __asm(" NOP");
                        bf_sum = mpy_SP_CSxCS(bf_pesos[j],bf_input[j]);
                        bf_output.dat[0] = bf_output.dat[0] + bf_sum.dat[0];
                        bf_output.dat[1] = bf_output.dat[1] + bf_sum.dat[1];
                        __asm(" NOP");
                        }
        bf_out_vector[idx_hilb]=bf_output;
    }

    for(;;);
    done();
	return 0;
}
