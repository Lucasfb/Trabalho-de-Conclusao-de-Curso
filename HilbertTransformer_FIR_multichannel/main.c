

/**
 * main.c
 */

#include "F28x_Project.h"
#include "setup.h"
#include "hilbert_transformer.h"

#pragma DATA_SECTION(test_input_signal, "sigIn");
#pragma DATA_SECTION(test_output_signal, "sigOut");


// A declaracao dos coeficientes foi problematica dentro do header para a transformada de HIlbert, devido ao data_section
// Utilizar no arquivo main() e passar os coeficientes como parametro foi a solucao encontrada
// Declara uma seção na memoria com um nome.
#pragma DATA_SECTION(coeffs_hilb,"coeffhilb");
//#pragma DATA_ALIGN(coeffs_hilb,64); //Alinhado
//#pragma DATA_SECTION(firFP, "firfilt") // Deve ser declarado em main.c

// Coeficientes constantes, gerados via MATLAB
float coeffs_hilb[HILBERT_TRANSFORMER_ORDER+1] = {
#include "coeffs_hilbert_negativo.h"
};


#define TEST_SIGNAL_SIZE 160

const float test_input_signal[TEST_SIGNAL_SIZE] = {
#include "test_input_signal.h"
};

const float test_output_signal[TEST_SIGNAL_SIZE] = {
#include "test_output_signal.h"
};

float output_signal[TEST_SIGNAL_SIZE];

// Variaveis usadas apenas no linker
//extern uint16_t CoeffFiltRunStart, CoeffFiltLoadStart, CoeffFiltLoadSize;

float hilb_buffer_chnl0[HILBERT_TRANSFORMER_ORDER+1];
//float hilb_buffer_chnl1[HILBERT_TRANSFORMER_ORDER+1];
//float hilb_buffer_chnl2[HILBERT_TRANSFORMER_ORDER+1];
//float hilb_buffer_chnl3[HILBERT_TRANSFORMER_ORDER+1];

FIR_FP hilb_channel0 = FIR_FP_DEFAULTS;
//FIR_FP hilb_channel1 = FIR_FP_DEFAULTS;;
//FIR_FP hilb_channel2 = FIR_FP_DEFAULTS;;
//FIR_FP hilb_channel3 = FIR_FP_DEFAULTS;;

FIR_FP_Handle hnd_hilb_channel0;

int main(void)
{
/*
#ifdef FLASH
    EALLOW;
    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0;
    memcpy((uint32_t *)&RamfuncsRunStart, (uint32_t *)&RamfuncsLoadStart,
            (uint32_t)&RamfuncsLoadSize );
    memcpy((uint32_t *)&CoeffFiltRunStart, (uint32_t *)&CoeffFiltLoadStart,
            (uint32_t)&CoeffFiltLoadSize );
    FPU_initFlash();
#endif //FLASH
*/
    FPU_initSystemClocks();
    FPU_initEpie();

    hnd_hilb_channel0 = hilbert_transformer_create(hilb_channel0, hilb_buffer_chnl0,coeffs_hilb);

    uint32_t idx_hilb = 0;
    float xn = 0; // O uso de xn e yn em vez dos arrays diretamente é para se aproveitar das ferramentas de
    float yn = 0; // visualizacao da IDE CCS
    for (idx_hilb = 0; idx_hilb < TEST_SIGNAL_SIZE;idx_hilb++){
        xn = test_input_signal[idx_hilb];
        yn = hilbert_transformer(xn, hnd_hilb_channel0);
        output_signal[idx_hilb] = yn;
     }


    done();
	return 0;
}
