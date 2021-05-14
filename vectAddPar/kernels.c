#include "pmsis.h"

// SEQUENTIAL VERSION
void vectAdd(int * pSrcA, int  * pSrcB, int * pDstC, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        pDstC[i] = pSrcA[i] + pSrcB[i];
        
    }
}

// PARALLEL VERSION

// v1
/*
void vectAddPar(int * pSrcA, int  * pSrcB, int * pDstC, int n)
{
    int i;
    int core_id = pi_core_id();
    
    for (i = core_id ; i < n; i+=NUM_CORES) {
        pDstC[i] = pSrcA[i] + pSrcB[i];
    }

    pi_cl_team_barrier();
}
*/

// v2
void vectAddPar(int * pSrcA, int  * pSrcB, int * pDstC, int n)
{
    int i, core_id, i_start, i_end, i_block, i_chunk;
    core_id = pi_core_id();    
    i_chunk = n / NUM_CORES;
    i_start = core_id * i_chunk;
    i_end   = i_start + i_chunk;
    
    for (i = i_start; i < i_end; i++) {
        pDstC[i] =  pSrcA[i] + pSrcB[i];
    }
    pi_cl_team_barrier();

}
