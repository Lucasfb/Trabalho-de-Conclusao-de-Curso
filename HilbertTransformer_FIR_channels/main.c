

/**
 * main.c
 */

#include "F28x_Project.h"
#include "fpu_filter.h"
#include "setup.h"
#include  "hilbert_transformer.h"

#define TEST_SIGNAL_SIZE 160
/*
 * Declara uma seção na memoria com um nome. O linker deve ser alterado e a memoria alinhada, de acordo com as instrucoes da biblioteca
 */
// Secoes para dados de teste
#pragma DATA_SECTION(test_input_signal, "sigIn");
#pragma DATA_SECTION(test_output_signal, "sigOut");
const float test_input_signal[TEST_SIGNAL_SIZE] = {
#include "test_input_signal.h"
};
const float test_output_signal[TEST_SIGNAL_SIZE] = {
#include "test_output_signal.h"
};
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



float output_signal[TEST_SIGNAL_SIZE];

int main(void)
{

    FPU_initSystemClocks();
    FPU_initEpie();


    // Inicializacao da transformada de Hilbert
    hilbert_transformer_create(hnd_hilb_chnl0, hilb_buffer_chnl0,coeffs_hilb);

    // Aplicando o filtro FIR para a Transformada de Hilbert
    uint32_t idx_hilb = 0;
    float xn = 0; // O uso de xn e yn em vez dos arrays diretamente é para se aproveitar das ferramentas de
    float yn = 0; // visualizacao da IDE CCS
    for (idx_hilb = 0; idx_hilb < TEST_SIGNAL_SIZE;idx_hilb++){
        xn = test_input_signal[idx_hilb];
        yn = hilbert_transformer(xn, hnd_hilb_chnl0);
        output_signal[idx_hilb] = yn;
    }

    for(;;);
    done();
	return 0;
}
