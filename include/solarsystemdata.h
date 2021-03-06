#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_LINES (500)

const char *get_field(char *line, int num);

typedef enum celestial_t {
  SUN = 0,
  MERCURY = 1,
  VENUS = 2,
  EARTH = 3,
  MARS = 4,
  JUPITER = 5,
  SATURN = 6,
  URANUS = 7,
  NEPTUNE = 8,
  PLUTO = 9
} celestial_t;

const char *get_planet_filename(celestial_t planet);

float get_planet_mass(celestial_t planet);

// output to julian dates, int is number of lines
int get_julian_dates(celestial_t planet, float **julian_dates);

float relative_error(float *estimate, float *real, int dim);

void load_index_vectors(int index, float *position_dest, float *velocity_dest,
                        celestial_t planet);
