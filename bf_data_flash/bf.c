/*
 * bf.c
 */

#include "bf.h"

complex_float _bf_pesos[NUMERO_MICS] = {
        #include <pesos_teste.h>
};


complex_float bf_aplicar(complex_float input[NUMERO_MICS]){
    uint16_t idx_mic;
    complex_float output = {0,0};
    complex_float bf_sum = {0,0};
    for (idx_mic = 0 ; idx_mic < NUMERO_MICS ; idx_mic++){
        __asm(" NOP");
        bf_sum = mpy_SP_CSxCS(_bf_pesos[idx_mic],input[idx_mic]);
        output.dat[0] = output.dat[0] + bf_sum.dat[0];
        output.dat[1] = output.dat[1] + bf_sum.dat[1];
        __asm(" NOP");
 }
    return output;
}
