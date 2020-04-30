#include "nbodysim.h"
#include "solarsystemdata.h"
#include <stdlib.h>

int main() {
  simdata_t *sdata = simdata_create(2, 1, 2);
  sdata->data[0] = 10;
  sdata->data[1] = 0;
  sdata->data[2] = 0;
  sdata->data[3] = 0;
  sdata->data[4] = 10;
  sdata->data[5] = -10;
  sdata->data[6] = 0;
  sdata->data[7] = 0;
  sdata->data[8] = 0;
  sdata->data[9] = 10;

  // run_simulation(sdata, INT_EULER, FORCE_NEWTONIAN, .1, 100);

  float *julian_dates;
  int num_dates = get_julian_dates(SATURN,&julian_dates);

  for(int i = 0; i < num_dates; i++) {
    printf("DATE - %f\n",julian_dates[i]);
  }

  return 0;
}
