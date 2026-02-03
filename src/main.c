#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "utils.h"
#include "action.h"
#include "state.h"


/* Lit les données d'entrée depuis un fichier */
int generateSimpleExampleMatrices(RLState *sArray, RLAction*aArray,
                                   float **T, float **R) {
    if (sArray == NULL || aArray == NULL) return -1;

    return readMatricesFromFile("matrices.txt", T, R);
}


int main(void) {

    srand(42);

    // On définit les états
    RLState s0 = {.id = 0, .label = "e0", .context = NULL};
    RLState s1 = {.id = 1, .label = "e1", .context = NULL};
    RLState s2 = {.id = 2, .label = "e2", .context = NULL};
    RLState s3 = {.id = 3, .label = "e3", .context = NULL};
    RLState s4 = {.id = 4, .label = "e4", .context = NULL};
    RLState s5 = {.id = 5, .label = "e5", .context = NULL};

    // On définit les actions
    RLAction a0 = {"droite", NULL, NULL};
    RLAction a1 = {"gauche", NULL, NULL};
    RLAction a2 = {"sauter", NULL, NULL};
    RLAction a3 = {"manger", NULL, NULL};

    // On créé un tablea d'états et un tableau d'actions

    int nStates = 6;
    int nActions = 4;

    RLState states[] = {s0, s1, s2, s3, s4, s5};
    RLAction actions[] = {a0, a1, a2, a3};

    RLEnv *env = RLEnvCreate(
        nStates, nActions, 
        states, actions,
        generateSimpleExampleMatrices, 
        getTransitionState, getTransitionArray, getReward
    );

    RLModel *m = RLModelCreate(env);

    RLConfig config = RLConfigCreate();
    RLConfigSetEpsilon(&config, 0.1);
    RLModelSetConfig(m, config);

    /* --- Q-Learning --- */

    QLearning(m);
    RLModelPrintQTable(m);
    RLModelPrintPolicy(m);

    /* -------------------*/

    RLModelDelete(&m);
    RLEnvDelete(&env);

    return 0;
}
