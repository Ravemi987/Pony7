// #include <stdio.h>
// #include <stdlib.h>

// #include "model.h"
// #include "utils.h"
// #include "action.h"
// #include "state.h"


// /* Lit les données d'entrée depuis un fichier */
// int generateSimpleExampleMatrices(RLState *sArray, RLAction*aArray,
//                                    float **T, float **R) {
//     if (sArray == NULL || aArray == NULL) return -1;

//     return readMatricesFromFile("matrices.txt", T, R);
// }


// int main(void) {

//     srand(42);

//     char* state_names[] = {
//         "case0", "case1", "case2",
//         "case3", "case4", "case5"
//     };

//     char* action_names[] = {
//         "droite", "gauche", "sauter", "manger"
//     };

//     /* ------ */

//     int nStates = 6;
//     int nActions = 4;

//     RLState states[nStates];
//     RLAction actions[nActions];

//     for (int i = 0; i < nStates; ++i) {
//         states[i].id = i;
//         states[i].label = state_names[i];
//         states[i].context = NULL;
//     }

//     for (int j = 0; j < nActions; ++j) {
//         actions[j].id = j;
//         actions[j].name = action_names[j];
//         actions[j].args = NULL;
//         actions[j].execute = NULL;
//     }

//     /* ------ */

//     RLEnv *env = RLEnvCreate(
//         nStates, nActions, 
//         states, actions,
//         generateSimpleExampleMatrices
//     );

//     RLModel *m = RLModelCreate(env);

//     RLConfig config = RLConfigCreate();
//     RLConfigSetEpsilon(&config, 0.1);
//     RLModelSetConfig(m, config);

//     /* --- Q-Learning --- */

//     QLearning(m);
//     RLModelPrintQTable(m);
//     RLModelPrintPolicy(m);

//     /* -------------------*/

//     RLModelDelete(&m);
//     RLEnvDelete(&env);

//     return 0;
// }
