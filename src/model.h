#ifndef __RL_MODEL_H__
#define __RL_MODEL_H__

#include <stdbool.h>
#include <stdio.h>

#include "env.h"
#include "config.h"

typedef struct s_rl_model RLModel;

RLModel* RLModelCreate(RLEnv *userData);

void RLModelDelete(RLModel **m);

float *RLModelGetStatesValues(RLModel *m);

int *RLModelGetPolicy(RLModel *m);

void valueIteration(RLModel *m);

#endif
