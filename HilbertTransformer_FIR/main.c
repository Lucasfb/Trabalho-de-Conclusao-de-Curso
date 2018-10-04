

/**
 * main.c
 */

#include "F28x_Project.h"
#include "fpu_filter.h"
#include "setup.h"


#define HILBERT_TRANSFORMER_ORDER 32
#define TEST_SIGNAL_SIZE 160
/*
 * Declara uma seção na memoria com um nome. O linker deve ser alterado e a memoria alinhada, de acordo com as instrucoes da biblioteca
 */
#pragma DATA_SECTION(hilb_buffer, "firldb")
#pragma DATA_SECTION(test_input_signal, "sigIn");
#pragma DATA_SECTION(test_output_signal, "sigOut");
#pragma DATA_SECTION(coeffs_hilb, "coeffhilb");
#pragma DATA_SECTION(firFP, "firfilt")

FIR_FP  firFP = FIR_FP_DEFAULTS;
FIR_FP_Handle hnd_firFP = &firFP;

float const coeffs_hilb[HILBERT_TRANSFORMER_ORDER+1] = {
#include "coeffs_hilbert_negativo.h"
};

const float test_input_signal[TEST_SIGNAL_SIZE] = {
#include "test_input_signal.h"
};

const float test_output_signal[TEST_SIGNAL_SIZE] = {
#include "test_output_signal.h"
};

float hilb_buffer[HILBERT_TRANSFORMER_ORDER+1];
float output_signal[TEST_SIGNAL_SIZE];

// Variaveis usadas apenas no linker
extern uint16_t CoeffFiltRunStart, CoeffFiltLoadStart, CoeffFiltLoadSize;


int main(void)
{

#ifdef FLASH
    EALLOW;
    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0;
    memcpy((uint32_t *)&RamfuncsRunStart, (uint32_t *)&RamfuncsLoadStart,
            (uint32_t)&RamfuncsLoadSize );
    memcpy((uint32_t *)&CoeffFiltRunStart, (uint32_t *)&CoeffFiltLoadStart,
            (uint32_t)&CoeffFiltLoadSize );
    FPU_initFlash();
#endif //FLASH

    FPU_initSystemClocks();
    FPU_initEpie();

    // Inicializacao da transformada de Hilbert
    hnd_firFP->order = HILBERT_TRANSFORMER_ORDER;
    hnd_firFP->dbuffer_ptr = hilb_buffer;
    hnd_firFP->coeff_ptr   = (float *)coeffs_hilb;
    hnd_firFP->init(hnd_firFP);


    // Aplicando o filtro FIR para a Transformada de HIblert
    uint32_t idx_hilb = 0;
    float xn = 0; // O uso de xn e yn em vez dos arrays diretamente é para se aproveitar das ferramentas de
    float yn = 0; // visualizacao da IDE CCS
    for (idx_hilb = 0; idx_hilb < TEST_SIGNAL_SIZE;idx_hilb++){
        xn = test_input_signal[idx_hilb];
        hnd_firFP->input = xn;
        hnd_firFP->calc(&firFP);
        yn = hnd_firFP->output;
        output_signal[idx_hilb] = yn;
    }
    done();
	return 0;
}
