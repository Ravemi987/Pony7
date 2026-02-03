#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "utils.h"
#include "action.h"
#include "state.h"
#include "agent.h"

/*
TODO:
config (new config a chaque fois pas fonction une par une)
passer Ation et Etat aux algo du modele pour pouvoir enregistrer les infos
(interface)
*/


typedef void (*ActionFunc)(void *context);


/* Lit les données d'entrée depuis un fichier */
int generateSimpleExampleMatrices(RLState *sArray, RLAction*aArray,
                                   float **T, float **R) {
    if (sArray == NULL || aArray == NULL) return -1;

    return readMatricesFromFile("matrices_cyber.txt", T, R);
}


int main(void) {
    if (system("touch /tmp/secret_hash.txt") != 0) return -1;

    srand(42);

    char* state_names[] = {
        "ls", "Crack", "Sudo", "Curl"
    };

    char* action_names[] = {
        "Scan_Ports", "Exploit_Vuln", "Mouvement_Lateral", "Exfiltration"
    };

    ActionFunc actionsCalls[] = {
        scanPorts, exploitVuln, lateralMovement, exfiltration
    };

    /* ------ */

    int nStates = 4;
    int nActions = 4;

    RLState states[nStates];
    RLAction actions[nActions];

    for (int i = 0; i < nStates; ++i) {
        states[i].id = i;
        states[i].label = state_names[i];
        states[i].context = NULL;
    }

    for (int j = 0; j < nActions; ++j) {
        actions[j].id = j;
        actions[j].name = action_names[j];
        actions[j].execute = actionsCalls[j];
        actions[j].args = NULL;
    }

    /* ------ */

    RLEnv *env = RLEnvCreate(
        nStates, nActions, 
        states, actions,
        generateSimpleExampleMatrices
    );

    RLModel *m = RLModelCreate(env);

    RLConfig config = RLConfigCreate();
    RLConfigSetEpsilon(&config, 0.1);
    RLConfigSetGamma(&config, 0.9);
    RLModelSetConfig(m, config);

    /* --- Q-Learning --- */

    QLearning(m);
    RLModelPrintQTable(m);
    RLModelPrintPolicy(m);

    /* -------- Agent -------- */

    executeAgent(m);

    /* ----------------------- */

    RLModelDelete(&m);
    RLEnvDelete(&env);

    return 0;
}
