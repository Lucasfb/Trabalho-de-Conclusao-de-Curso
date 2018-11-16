/*
 * bf.h
 */

#ifndef BF_H_
#define BF_H_

#include <fpu_vector.h>

#define NUMERO_MICS 4U


extern complex_float bf_in[NUMERO_MICS];
extern complex_float bf_out;
extern complex_float bf_sum;
extern complex_float _bf_pesos[NUMERO_MICS];


complex_float bf_aplicar(complex_float input[NUMERO_MICS]);

#endif /* BF_H_ */
