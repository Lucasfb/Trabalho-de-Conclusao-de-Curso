/*
 * circular_buffer.h
 *
 *  Created on: 24 de out de 2018
 *      Author: lucas
 *  Baseado em:
 *       https://embedjournal.com/implementing-circular-buffer-embedded-c/
 *       https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc#c-implementation
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdint.h>

typedef struct {
    float *buffer;
    uint32_t tail;
    uint32_t head;
    uint32_t size;
    uint32_t livre;
} circ_buf_t;

typedef circ_buf_t  *circ_buf_handler;

void cb_inicializar(circ_buf_handler circ_buf, uint32_t tamanho, float *buffer);
void cb_push(circ_buf_handler circ_buf, float input);
float cb_pop(circ_buf_handler cb);

#endif /* CIRCULAR_BUFFER_H_ */
