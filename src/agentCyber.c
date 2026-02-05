// #include <stdio.h>
// #include <stdlib.h>

// #include "model.h"
// #include "utils.h"
// #include "action.h"
// #include "state.h"
// #include "agent.h"


// /* Typedef */

// typedef void (*ActionFunc)(void *context);


// /* Implémentation de l'interface de l'agent */

// void scanPorts(void *args) {
//     if (args) return;

//     printf("Recherche de hash...\n");
//     if (system("ls /tmp/secret_hash.txt > /dev/null 2>&1") != 0) return;
// }

// void exploitVuln(void *args) {
//     if (args) return;

//     printf("Cassage du hash avec hashcat (simule)...\n");
//     if (system("sleep 1 && echo 'Password cracked: 123456'") != 0) return;
// }

// void lateralMovement(void *args) {
//     if (args) return;

//     printf("Tentative de sudo...\n");
//     printf("Accès ROOT accordé.\n");
// }

// void exfiltration(void *args) {
//     if (args) return;

//     printf("Recuperation de la cible...\n");
//     if (system("curl -s http://google.com | head -n 1") != 0) return;
// }


// /* Implémentation de l'interface pour la génération des tableaux de données (transitions et récompenses) */

// int generateSimpleExampleMatrices(RLState *sArray, RLAction*aArray,
//                                    float **T, float **R) {
//     if (sArray == NULL || aArray == NULL) return -1;

//     return readMatricesFromFile("matrices_cyber.txt", T, R);
// }

// /* Programme principal */

// int main(void) {
//     if (system("touch /tmp/secret_hash.txt") != 0) return -1;

//     srand(42);

//     /* Définition des états et des actions */

//     char* state_names[] = {
//         "ls", "Crack", "Sudo", "Curl"
//     };

//     char* action_names[] = {
//         "Scan_Ports", "Exploit_Vuln", "Mouvement_Lateral", "Exfiltration"
//     };

//     ActionFunc actionsCalls[] = {
//         scanPorts, exploitVuln, lateralMovement, exfiltration
//     };

//     /* Instantiation des tableaux de données  */

//     int nStates = 4;
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
//         actions[j].execute = actionsCalls[j];
//         actions[j].args = NULL;
//     }

//     /* Création de l'environnement et du modèle */

//     RLEnv *env = RLEnvCreate(
//         nStates, nActions, 
//         states, actions,
//         generateSimpleExampleMatrices
//     );

//     RLModel *m = RLModelCreate(env);

//     RLConfig config = {
//         .gamma = 0.9,
//         .epsilon = 0.1,
//         .alpha = 1,
//         .steps = 10,
//         .epochs = 100
//     };

//     RLModelSetConfig(m, config);

//     /* --- Q-Learning --- */

//     QLearning(m);
//     RLModelPrintQTable(m);
//     RLModelPrintPolicy(m);

//     /* -------- Création et Exécution de l'Agent -------- */

//     RLAgent agent = {
//         .name = "Cyber",
//         .initialState = 0, 
//         .goalState = 3,
//         .maxIter = 20,
//         .reward = 0,
//         .sucess = false
//     };

//     executeAgent(m, &agent);

//     if (agent.sucess) {
//         printf("MISSION REUSSIE\n");
//     } else {
//         printf("ECHEC\n");
//     }

//     /* ----------------------- */

//     RLModelDelete(&m);
//     RLEnvDelete(&env);

//     return 0;
// }
