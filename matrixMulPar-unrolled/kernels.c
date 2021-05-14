#include "pmsis.h"


#if NUM_CORES == 1
// Sequential version
void matmul_int32_unroll2x4(int * in1, int * in2, int* out3,int MN, int NN, int KN){
    // task to profile
    int acc0, acc1, acc2, acc3,
        acc4, acc5, acc6, acc7;

    for (int i = 0; i < MN; i=i+2) {
      for (int j = 0; j < NN; j=j+4) {
        acc0 = acc1 = acc2 = acc3 = 0;
        acc4 = acc5 = acc6 = acc7 = 0;

        for (int k = 0; k < KN; k++) {
           int shared_op = in1[i*KN+k]; 
           char a = in2[k*NN+j];
           char b = in2[k*NN+j+1];
           char c = in2[k*NN+j+2];
           char d = in2[k*NN+j+3];

           acc0   += shared_op * a;
           acc1   += shared_op * b;
           acc2   += shared_op * c;
           acc3   += shared_op * d;

           shared_op = in1[(i+1)*KN+k];
           acc4   += shared_op * a;
           acc5   += shared_op * b;
           acc6   += shared_op * c;
           acc7   += shared_op * d;
        } //k
        out3[i*NN+j]   = acc0;
        out3[i*NN+j+1] = acc1;
        out3[i*NN+j+2] = acc2;
        out3[i*NN+j+3] = acc3;
        out3[(i+1)*NN+j]   = acc4;
        out3[(i+1)*NN+j+1] = acc5;
        out3[(i+1)*NN+j+2] = acc6;
        out3[(i+1)*NN+j+3] = acc7;

      }//j
    }//i
}
#else
// Parallel version
void matmul_int32_unroll2x4(int * in1, int * in2, int* out3,int MN, int NN, int KN){
    // task to profile
    int acc0, acc1, acc2, acc3,
        acc4, acc5, acc6, acc7;

    int core_id = pi_core_id();

    for (int i = 2*core_id; i < MN; i+=NUM_CORES*2) {
      for (int j = 0; j < NN; j=j+4) {
        acc0 = acc1 = acc2 = acc3 = 0;
        acc4 = acc5 = acc6 = acc7 = 0;

        for (int k = 0; k < KN; k++) {
           int shared_op = in1[i*KN+k]; 
           char a = in2[k*NN+j];
           char b = in2[k*NN+j+1];
           char c = in2[k*NN+j+2];
           char d = in2[k*NN+j+3];

           acc0   += shared_op * a;
           acc1   += shared_op * b;
           acc2   += shared_op * c;
           acc3   += shared_op * d;

           shared_op = in1[(i+1)*KN+k];
           acc4   += shared_op * a;
           acc5   += shared_op * b;
           acc6   += shared_op * c;
           acc7   += shared_op * d;
        } //k
        out3[i*NN+j]   = acc0;
        out3[i*NN+j+1] = acc1;
        out3[i*NN+j+2] = acc2;
        out3[i*NN+j+3] = acc3;
        out3[(i+1)*NN+j]   = acc4;
        out3[(i+1)*NN+j+1] = acc5;
        out3[(i+1)*NN+j+2] = acc6;
        out3[(i+1)*NN+j+3] = acc7;

      }//j
    }//i

    pi_cl_team_barrier();
}
#endif
