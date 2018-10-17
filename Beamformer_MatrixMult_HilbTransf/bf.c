/*
 * bf.c
 */

#include <bf.h>

complex_float blank = {0,0};

complex_float * aplicar_beamformer_vetor(
        complex_float vetor_pesos[NUMERO_MICS],
        complex_float matriz_sinal[NUMERO_MICS][NUMERO_AMOSTRAS],
        complex_float return_vetor_aplicado[NUMERO_AMOSTRAS]){

    //Indices usados
    uint32_t i=0;
    uint32_t j=0;


     for (i = 0 ; i < NUMERO_AMOSTRAS ; i++){
         for (j = 0 ; j < NUMERO_MICS ; j++){
             __asm(" NOP");
             return_vetor_aplicado[i] = mpy_SP_CSxCS(vetor_pesos[j], matriz_sinal[j][i]);
             __asm(" NOP");
     }
 }

     return return_vetor_aplicado;
}





