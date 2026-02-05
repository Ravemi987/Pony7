#ifndef __RL_AGENT_H__
#define __RL_AGENT_H__

#include "model.h"

typedef struct s_rl_agent {
    char *name;
    int initialState;
    int goalState;
    int maxIter;
    float reward;
    bool sucess;
} RLAgent;

void executeAgent(RLModel *m, RLAgent* a);

#endif
