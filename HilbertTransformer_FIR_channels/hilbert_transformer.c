/*
 * hilbert_transformer.c
 *
 *  Created on: 7 de out de 2018
 *      Author: lucas
 */

#include "hilbert_transformer.h"



void hilbert_transformer_create(FIR_FP_Handle hilb_handle, float buffer[HILBERT_TRANSFORMER_ORDER+1], float coeffs[HILBERT_TRANSFORMER_ORDER+1]){
    // É necessario o programa principal alocar a memoria para o estrutura do filtro e do buffer utilizado
    // Essa funcao apenas configura a estrutura do filtro e retorna uma handle usada para manipular e identificar o transformador
    //quando for utilizado em outras funcoes
    //FIR_FP_Handle hnd_firFP = &hilb;
    // Inicializacao da transformada de Hilbert
    hilb_handle->order = HILBERT_TRANSFORMER_ORDER;
    hilb_handle->dbuffer_ptr = buffer;
    hilb_handle->coeff_ptr   = (float *)coeffs;
    hilb_handle->init(hilb_handle);
    return;
}

float hilbert_transformer(float input_data, FIR_FP_Handle hilb_handle){
    hilb_handle->input = input_data;
    hilb_handle->calc(hilb_handle);
    return hilb_handle->output;
}
