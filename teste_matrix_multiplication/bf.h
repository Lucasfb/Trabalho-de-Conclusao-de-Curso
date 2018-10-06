/*
 * bf.h
 */

#ifndef BF_H_
#define BF_H_

#include <fpu_vector.h>

#define NUMERO_MICS 4U
#define NUMERO_AMOSTRAS 100U

// Nao usado
//extern complex_float pesos[NUMERO_MICS];
//extern complex_float sinal_mics[NUMERO_MICS][NUMERO_AMOSTRAS];
//extern complex_float sinal_processado[NUMERO_AMOSTRAS];

complex_float * aplicacao_pesos(
        complex_float vetor_pesos[NUMERO_MICS],
        complex_float matriz_sinal[NUMERO_MICS][NUMERO_AMOSTRAS],
        complex_float return_vetor_aplicado[NUMERO_AMOSTRAS] );

#endif /* BF_H_ */
