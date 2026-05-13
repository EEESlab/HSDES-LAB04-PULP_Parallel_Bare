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
void vectAddPar(int * pSrcA, int  * pSrcB, int * pDstC, int n)
{
    int i;
    // GET CORE ID / COMPUTE BOUNDS

    // FIX THE ITERATION SPACE
    for (i = 0 ; i < n; i++) {
        pDstC[i] = pSrcA[i] + pSrcB[i];
        
    }

    // ADD A BARRIER
}
