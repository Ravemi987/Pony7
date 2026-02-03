#ifndef __RL_MODEL_H__
#define __RL_MODEL_H__

#include <stdbool.h>
#include <stdio.h>

#include "env.h"
#include "config.h"

typedef struct s_rl_model RLModel;

RLModel* RLModelCreate(RLEnv *userData);

void RLModelDelete(RLModel **m);

void RLModelSetConfig(RLModel *m, RLConfig cfg);

float *RLModelGetStatesValues(RLModel *m);

int *RLModelGetPolicy(RLModel *m);

float *RLModelGetQTable(RLModel *m);

RLConfig *RLModelGetConfig(RLModel *m);

void RLModelPrintStatesValues(RLModel *m);

void RLModelPrintPolicy(RLModel *m);

void RLModelPrintQTable(RLModel *m);

void valueIteration(RLModel *m);

void policyIteration(RLModel *m);

void QLearning(RLModel *m);

#endif
