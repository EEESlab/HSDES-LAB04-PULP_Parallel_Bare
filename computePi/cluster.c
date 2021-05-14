#include "pmsis.h"
#include "perf.h"


#if NUM_CORES == 1

// Sequential code
float iterative_pi(int n) {
        float x, area = 0;
        int i;
        for(i=0; i<n; i++) {
                x = (i + 0.5f)/n;
                float val = 4.0f/(1.0f + x*x);
                area += val;
        }
        return area/n;
}

#else


// Parallel version (poor performance)

/*
PI_L1 float area;

float iterative_pi(int n) {
        float x;
	area = 0;
        int i;
        for(i=pi_core_id(); i<n; i+=NUM_CORES) {
                x = (i + 0.5f)/n;
                float val = 4.0f/(1.0f + x*x);
		pi_cl_team_critical_enter();
                area += val;
		pi_cl_team_critical_exit();
        }
	pi_cl_team_barrier();
        return area/n;
}
*/

// Parallel version (optimized moving using local variables and moving the critical region after the loop)

PI_L1 float area_sh;

float iterative_pi(int n) {
        float x, area = 0;
        int i;

        if(pi_core_id() == 0) area_sh = 0.0f;
        pi_cl_team_barrier();

        for(i=pi_core_id(); i<n; i+=NUM_CORES) {
                x = (i + 0.5f)/n;
                float val = 4.0f/(1.0f + x*x);
                area += val;
        }
        pi_cl_team_barrier();

        pi_cl_team_critical_enter();
        area_sh += area;
        pi_cl_team_critical_exit();

        pi_cl_team_barrier();
        return area_sh/n;
}

#endif

void cluster_fn() {
  float x;

  // init performance counters
  INIT_STATS();

  // executing the code multiple times to perform average statistics
  ENTER_STATS_LOOP();
  
  // start measuring
  START_STATS();

  x = iterative_pi(512);

  // stop measuring
  STOP_STATS();

  // end of the performance statistics loop
  EXIT_STATS_LOOP();


  printf ("PI = %f\n", x);
}
