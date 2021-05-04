#include "pmsis.h"

void vectAdd(int * pSrcA, int  * pSrcB, int * pDstC, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        pDstC[i] = pSrcA[i] + pSrcB[i];
        
    }
}

