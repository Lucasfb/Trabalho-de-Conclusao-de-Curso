/*
 * bf.h
 */

#ifndef BF_H_
#define BF_H_

#include <fpu_vector.h>

#define NUMERO_MICS 4U
#define NUMERO_AMOSTRAS 100U

extern complex_float bf_sum;


complex_float * aplicar_beamformer_vetor(
        complex_float vetor_pesos[NUMERO_MICS],
        complex_float matriz_sinal[NUMERO_MICS][NUMERO_AMOSTRAS],
        complex_float return_vetor_aplicado[NUMERO_AMOSTRAS]
        );


#endif /* BF_H_ */
