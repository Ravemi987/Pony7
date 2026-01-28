#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "utils.h"


int main(void) {

    srand(42);

    float T[] = {
        0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1
    };

    float R[] = {
        0, -1, -10, 0,
        -1, 0, 0, 0,
        0, -10, 0, 0,
        0, -1, -1, 0,
        0, -1, 0, 0,
        0, -1, 0, 10
    };

    RLEnv *env = RLEnvCreate(
        6, 4, T, R, 
        getTransitionState, getTransitionArray, getReward
    );

    RLModel *m = RLModelCreate(env);


    /* --- Value Iteration ---*/

    // valueIteration(m);

    // RLModelPrintStatesValues(m);
    // RLModelPrintPolicy(m);
    
    /* --- Policy Iteration ---*/

    RLConfig *config = RLModelGetConfig(m);
    RLConfigSetEpsilon(config, 0.1);

    // policyIteration(m);

    // RLModelPrintStatesValues(m);
    // RLModelPrintPolicy(m);

    /* --- Q-Learning --- */

    QLearning(m);
    RLModelPrintQTable(m);
    RLModelPrintPolicy(m);

    /* -------------------*/

    RLModelDelete(&m);
    RLEnvDelete(&env);

    return 0;
}
