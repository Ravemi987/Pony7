#include "env.h"
#include <stdlib.h>


/*
Créé un environnement
*/
RLEnv *RLEnvCreate(int nStates, int nActions,
                   float *tData, float *rData,
                   int (*TransitionState)(RLEnv*, int, int),
                   float *(*TransitionArray)(RLEnv*, int, int),
                   float (*Reward)(RLEnv*, int, int)) {
    RLEnv *env = malloc(sizeof(struct s_rl_env));

    // Dimensions
    env->nS = nStates;
    env->nA = nActions;

    // Données (les tableaux)
    env->transitions = tData;
    env->rewards = rData;

    // Fonctions d'accès aux données
    env->TransitionState = TransitionState;
    env->TransitionArray = TransitionArray;
    env->Reward = Reward;

    return env;
}

/*
Libère un environnement
*/
void RLEnvDelete(RLEnv **env) {
    if ((*env) == NULL) return;

    free(*env);
    *env = NULL;
}

/* Getters */

int RLEnvGetStatesNumber(RLEnv *e) {
    return e->nS;
}

int RLEnvGetActionsNumber(RLEnv *e) {
    return e->nA;
}

/*
Prend en entrée une matrice 3D: pour chaque état et chaque actions,
la probabilité d'atteindre chaque nouvel état.
Retourne un tableau 1D: les probabilités d'atteindre les nouveaux états
*/
float *getTransitionArray(RLEnv *e, int s, int a) {
    int offset = (s * e->nA * e->nS) + (a * e->nS);
    return &(e->transitions[offset]);
}

/*
Prend en entrée une matrice 3D: pour chaque état et chaque actions,
la probabilité d'atteindre chaque nouvel état.
Retourne le nouvel état choisit.
*/
int getTransitionState(RLEnv *e, int s, int a) {
    int offset = (s * e->nA * e->nS) + (a * e->nS);

    float max = 0;
    int new_state = 0;

    for (int next_s = 0; next_s < e->nS; ++next_s) {
        float v = e->transitions[offset + next_s];
        if (v > 0.0f && v >= max) {
            max = v;
            new_state = next_s;
        }
    }

    return new_state;
}

/*
Prend en entrée une matrice 2D: la récompense pour chaque état et action
Retourne la récompense.
*/
float getReward(RLEnv *e, int s, int a) {
    return e->rewards[s * e->nA + a];
}
