#include "pmsis.h"

// Sequential version
void vectAdd(int * pSrcA, int  * pSrcB, int * pDstC, int n) {
    int i;
    for (i = 0; i < n; i++) {
        pDstC[i] = pSrcA[i] + pSrcB[i];
        
    }
}

// Parallel version
void vectAddPar(int * pSrcA, int  * pSrcB, int * pDstC, int n) {
    int i;
    // TODO: GET CORE ID / COMPUTE BOUNDS

    // TODO: FIX THE ITERATION SPACE
    for (i = 0; i < n; i++) {
        pDstC[i] = pSrcA[i] + pSrcB[i];
    }

    // TODO: ADD A BARRIER
}
