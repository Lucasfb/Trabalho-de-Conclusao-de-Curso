/*
 * hilbert_transformer.h
 *
 *  Created on: 7 de out de 2018
 *      Author: lucas
 */

#ifndef HILBERT_TRANSFORMER_H_
#define HILBERT_TRANSFORMER_H_

#include "fpu_filter.h"

#define HILBERT_TRANSFORMER_ORDER 32

void hilbert_transformer_create(FIR_FP_Handle hilb_handle, float buffer[HILBERT_TRANSFORMER_ORDER+1],float coeffs[HILBERT_TRANSFORMER_ORDER+1]);
float hilbert_transformer(float input_data, FIR_FP_Handle hilb_handle);



#endif /* HILBERT_TRANSFORMER_H_ */
