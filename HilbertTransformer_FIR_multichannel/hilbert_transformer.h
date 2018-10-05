/*
 * hilbert_transformer.h
 *
 *  Created on: 28 de set de 2018
 *      Author: lucas
 */

#ifndef HILBERT_TRANSFORMER_H_
#define HILBERT_TRANSFORMER_H_

#include "fpu_filter.h"

#define HILBERT_TRANSFORMER_ORDER 32

// Funcoes
uint32_t hilbert_buffer_size();
FIR_FP_Handle hilbert_transformer_create(FIR_FP hilb, float buffer[HILBERT_TRANSFORMER_ORDER+1],float coeffs[HILBERT_TRANSFORMER_ORDER+1]);
float hilbert_transformer(float input_data, FIR_FP_Handle hilb_handle);


#endif /* HILBERT_TRANSFORMER_H_ */
