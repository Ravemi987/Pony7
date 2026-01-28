#include <stdio.h>

#include "model.h"


int main(void) {

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

    // On définit un environnement avec nos tableaux et fonctions
    RLEnv *env = RLEnvCreate(6, 4, T, R, getTransition, getReward);

    // On créé le modèle en passant l'environnement
    RLModel *m = RLModelCreate(env);

    // On exécute l'algorithme
    valueIteration(m);

    // On affiche les résultats
    printFloatArray(RLModelGetStatesValues(m), RLEnvGetStatesNumber(env));
    printIntArray(RLModelGetPolicy(m), RLEnvGetStatesNumber(env));

    // On clean
    RLModelDelete(&m);
    RLEnvDelete(&env);

    return 0;
}
