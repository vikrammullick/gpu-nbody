#include "nbodysim.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define G (6.673 * pow(10, -11))
#define G (6.673 * pow(10, -11) * 13.3153474)

void getForce(float *force, float *position, float *features,
              float *positionActor, float *featuresActor, simdata_t *sdata) {

  float feature = features[0];
  float featureActor = featuresActor[0];

  float distance = 0;
  for (int i = 0; i < sdata->posdim; i++) {
    float pos = position[i];
    float posActor = positionActor[i];
    float deltaPos = pos - posActor;
    distance += deltaPos * deltaPos;
  }
  distance = sqrt(distance);

  for (int i = 0; i < sdata->posdim; i++) {
    float pos = position[i];
    float posActor = positionActor[i];
    float deltaPos = pos - posActor;

    float f =
        feature * featureActor / distance / distance * deltaPos / distance;
    force[i] -= f;
  }
}

void leapfrog_part1(float timeStep, simdata_t *sdata) {
  float *position, *velocity;
  for(int i = 0; i < sdata->nparticles; i++) {
    position = simdata_pos_ptr(sdata, i);
    velocity = simdata_vel_ptr(sdata, i);
    for(int j = 0; j < sdata->posdim; j++){
      position[j] += 0.5 * timeStep * velocity[j];
    }
  }
}

void leapfrog_part2(float* accelerations, float timeStep, simdata_t *sdata) {
  float *position, *velocity,*acceleration;
  for(int i = 0; i < sdata->nparticles; i++) {
    position = simdata_pos_ptr(sdata, i);
    velocity = simdata_vel_ptr(sdata, i);
    acceleration = accelerations + i * sdata->posdim;
    for(int j = 0; j < sdata->posdim; j++){
      velocity[j] += timeStep * acceleration[j];
      position[j] += 0.5 * timeStep * velocity[j];
    }
  }
}

void euler_part2(float *accelerations, float timeStep, simdata_t *sdata) {
  float *position, *velocity,*acceleration;
  for (int j = 0; j < sdata->nparticles; j++) {
    position = simdata_pos_ptr(sdata, j);
    velocity = simdata_vel_ptr(sdata, j);
    acceleration = accelerations + j * sdata->posdim;
    for (int i = 0; i < sdata->posdim; i++) {
      position[i] += velocity[i] * timeStep + 0.5 * acceleration[i] * timeStep * timeStep;
      velocity[i] += acceleration[i] * timeStep;
    }
  }
}

void dump(simdata_t *sdata, int step) {
  printf("=============== STEP %d ===============\n", step);
  float *pos, *vel;
  for (int i = 0; i < sdata->nparticles; i++) {
    pos = simdata_pos_ptr(sdata, i);
    vel = simdata_vel_ptr(sdata, i);
    printf("\t(%.04f, %.04f,%.04f)\t(%.04f, %.04f,%.04f)\n", pos[0], pos[1], pos[2], vel[0],
           vel[1],vel[2]);
  }
}

void run_simulation(simdata_t *sdata, integrator_t int_type, force_t force_type,
                    float time_step, int steps) {
  float *accelerations =
      (float *)malloc(sizeof(float) * sdata->posdim * sdata->nparticles);
  for (int step = 0; step < steps; step++) {

    switch(int_type) {
      case INT_LEAPFROG:
        leapfrog_part1(time_step,sdata);
        break;
      default:
        break;
    }


    for (int i = 0; i < sdata->nparticles; i++) {
      float *acceleration = &accelerations[i * sdata->posdim];
      memset(acceleration, 0, sdata->posdim * sizeof(float));

      float *position = simdata_pos_ptr(sdata, i);
      float *features = simdata_feat_ptr(sdata, i);

      for (int j = 0; j < sdata->nparticles; j++) {
        if (i == j)
          continue;
        float *positionActor = simdata_pos_ptr(sdata, j);
        float *featuresActor = simdata_feat_ptr(sdata, j);

        getForce(acceleration, position, features, positionActor, featuresActor,
                 sdata);
      }
      for (int j = 0; j < sdata->posdim; j++) {
        acceleration[j] = acceleration[j] * G / features[0];
      }
    }

    switch(int_type){
      case INT_LEAPFROG:
        leapfrog_part2(accelerations,time_step,sdata);
        break;
      default:
        euler_part2(accelerations,time_step,sdata);
    }
    // dump(sdata, step);
  }
  free(accelerations);
}
