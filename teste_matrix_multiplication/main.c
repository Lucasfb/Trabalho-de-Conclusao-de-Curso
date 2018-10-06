

#include <bf.h>
//#include "signal_gen.h"


int main(void)
{
    static complex_float pesos_teste[NUMERO_MICS] = {
        #include <pesos_teste.h>
    };

    //Static é necessário, senao a variável nao cabe no stack
    static complex_float input_teste[NUMERO_MICS][NUMERO_AMOSTRAS] = {
       {
        #include <input_teste_mic1.h>
       },
       {
        #include <input_teste_mic2.h>
       },
       {
        #include <input_teste_mic3.h>
       },
       {
        #include <input_teste_mic4.h>
       },
    };

    //variavel gerada e zerada
    complex_float output_teste[NUMERO_AMOSTRAS];
    memset_fast(output_teste, 0, NUMERO_AMOSTRAS*sizeof(complex_float));


    for(;;){
        uint32_t i=0;
        uint32_t j=0;
        complex_float temp = {0,0};

        //Reseta o output entre cada execucao
        memset_fast(output_teste, 0, NUMERO_AMOSTRAS*sizeof(complex_float));

        for (i = 0 ; i < NUMERO_AMOSTRAS ; i++){
             for (j = 0 ; j < NUMERO_MICS ; j++){
                 __asm(" NOP");
                 temp = mpy_SP_CSxCS(pesos_teste[j],input_teste[j][i]);
                 output_teste[i].dat[0] = output_teste[i].dat[0] + temp.dat[0];
                 output_teste[i].dat[1] = output_teste[i].dat[1] + temp.dat[1];
                 __asm(" NOP");
             }
         }
    };

//	return 0;
}
