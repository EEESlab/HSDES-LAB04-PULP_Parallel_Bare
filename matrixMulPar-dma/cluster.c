#include "pmsis.h"
#include "perf.h"

#define M 32
#define N 32
#define O 32


// APPLICATION DATA
PI_L2 int matA[M*N];
PI_L2 int matB[N*O];
PI_L2 int matC[M*O];

PI_L1 int matA_cl[M*N];
PI_L1 int matB_cl[N*O];
PI_L1 int matC_cl[M*O];

void matMul(int * pSrcA, int  * pSrcB, int * pDstC, int m, int n, int o);


// Helper functions

void matrix_init(int * A, int * B, int * C) {
  if(pi_core_id() == 0)
  {

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      A[i*N+j] = (int)(j+1);
    }
  }

  for (int i = 0; i <N; i++) {
    for (int j = 0; j < O; j++) {
      B[i*O+j] = i+1;
    }
  }

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < O; j++) {
      C[i*O+j] = 0;
    }
  }

  }

  pi_cl_team_barrier();
}

unsigned int matrix_check(int * C) {
  unsigned int errors = 0;
  // check
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < O; j++) {
      int ref_val = ((N)*(N+1)*(2*(N)+1))/6;
      if (C[i*O+j] != ref_val) {
        printf("Error at index (%d,%d): %d instead of %d\n", i, j, C[i*N+j], ref_val);
        errors++;
      }
    }
  }
  return errors;
}


void cluster_fn() {
  volatile int m = M;
  volatile int n = N;
  volatile int o = O;

  pi_cl_dma_cmd_t command1, command2;

  // init performance counters
  INIT_STATS();

  // executing the code multiple times to perform average statistics
  ENTER_STATS_LOOP();
  
  // set initial values (not considered by performance counters)
  matrix_init(matA, matB, matC);

  // start measuring
  START_STATS();

  if(pi_core_id() == 0) {
    pi_cl_dma_cmd((uint32_t) matA, (uint32_t) matA_cl, M*N*sizeof(matA[0]), PI_CL_DMA_DIR_EXT2LOC, &command1);
    pi_cl_dma_cmd((uint32_t) matB, (uint32_t) matB_cl, N*O*sizeof(matB[0]), PI_CL_DMA_DIR_EXT2LOC, &command2);
    pi_cl_dma_cmd_wait(&command1);
    pi_cl_dma_cmd_wait(&command2);
  }
  pi_cl_team_barrier();

  // workload
  matMul(matA_cl, matB_cl, matC_cl, m, n, o);

  if(pi_core_id() == 0) {
    pi_cl_dma_cmd((uint32_t) matC, (uint32_t) matC_cl, M*N*sizeof(matC[0]), PI_CL_DMA_DIR_LOC2EXT, &command1);
    pi_cl_dma_cmd_wait(&command1);
  }
  pi_cl_team_barrier();

  // stop measuring
  STOP_STATS();

  // end of the performance statistics loop
  EXIT_STATS_LOOP();

#ifdef DEBUG  
  // check the result (optional)
  if(pi_core_id() == 0)
    matrix_check(matC);
#endif  
}
