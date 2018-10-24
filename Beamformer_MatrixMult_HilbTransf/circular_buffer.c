/*
 * circular_buffer.c
 *
 *  Created on: 24 de out de 2018
 *      Author: lucas
 */
#include "circular_buffer.h"

void cb_inicializar(circ_buf_handler cb, uint32_t tamanho, float *buffer){
    cb->buffer = buffer;
    cb->head = 0;
    cb->tail = 0;
    cb->size = tamanho;
    cb->livre = tamanho;
};

void cb_push(circ_buf_handler cb, float input){
    uint32_t prox;
    prox = cb->head + 1;
    if (prox >= cb->size){
        prox = 0;
    }
    cb->buffer[cb->head] = input;
    cb->head = prox;
    if (cb->livre > 0){
        cb->livre--;
    }
}

float cb_pop(circ_buf_handler cb){
    uint32_t prox;
    if (cb->livre == cb->size) return 0;  //Caso nao haja valor, retornar 0
    prox = cb->tail +1;
    if (prox >= cb->size){
        prox = 0;
    }
    float data;
    data = cb->buffer[cb->tail];
    cb->tail = prox;
    return data;
}
