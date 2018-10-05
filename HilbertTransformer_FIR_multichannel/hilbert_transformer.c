/*
 * hilbert_transformer.c
 *
 *  Created on: 28 de set de 2018
 *      Author: lucas
 */

#include "hilbert_transformer.h"



uint32_t hilbert_buffer_size(){
    return (HILBERT_TRANSFORMER_ORDER+1);
}


FIR_FP_Handle hilbert_transformer_create(FIR_FP hilb, float buffer[HILBERT_TRANSFORMER_ORDER+1], float coeffs[HILBERT_TRANSFORMER_ORDER+1]){
    // É necessario o programa principal alocar a memoria para o estrutura do filtro e do buffer utilizado
    // Essa funcao apenas configura a estrutura do filtro e retorna uma handle usada para manipular e identificar o transformador
    //quando for utilizado em outras funcoes
    //hilb = FIR_FP_DEFAULTS;  //Por algum motivo, nao é possivel atribuir o padrao dentro dessa funcao. Deve ser atribuido antes dessa funcao ser utilizada
    FIR_FP_Handle hnd_firFP = &hilb;
    // Inicializacao da transformada de Hilbert
    hnd_firFP->order = HILBERT_TRANSFORMER_ORDER;
    hnd_firFP->dbuffer_ptr = buffer;
    hnd_firFP->coeff_ptr   = (float *)coeffs;
    hnd_firFP->init(hnd_firFP);
    return hnd_firFP;
}


float hilbert_transformer(float input_data, FIR_FP_Handle hilb_handle){
    // Aplicando o filtro FIR para a Transformada de Hilbert
    float output_data = 0;
    hilb_handle->input = input_data;
    hilb_handle->calc(hilb_handle);
    output_data = hilb_handle->output;
    return output_data;
}
