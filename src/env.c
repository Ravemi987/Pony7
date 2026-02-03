#include "env.h"

#include <stdlib.h>


int RLEnvInit(RLEnv *env) {
    return env->Generate(
        env->states, env->actions,
        &(env->transitions), &(env->rewards)
    );
}

/*
Créé un environnement
*/
RLEnv *RLEnvCreate(int nStates, int nActions,
                   RLState *states, RLAction *actions,
                   int (*Generate)(RLState *, RLAction*, float**, float**),
                   int (*TransitionState)(RLEnv*, int, int),
                   float *(*TransitionArray)(RLEnv*, int, int),
                   float (*Reward)(RLEnv*, int, int)) {
    RLEnv *env = malloc(sizeof(struct s_rl_env));

    // Dimensions
    env->nS = nStates;
    env->nA = nActions;

    // Tableaux en entrée
    env->states = states;
    env->actions = actions;

    // Tableaux de données
    env->transitions = NULL;
    env->rewards = NULL;

    // Fonction de génération des données
    env->Generate = Generate;

    // Fonctions d'accès aux données
    env->TransitionState = TransitionState;
    env->TransitionArray = TransitionArray;
    env->Reward = Reward;

    if (RLEnvInit(env) == -1) return NULL;

    return env;
}

/*
Libère un environnement
*/
void RLEnvDelete(RLEnv **env) {
    if ((*env) == NULL) return;
    free((*env)->transitions);
    free((*env)->rewards);
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
Prend en entrée une matrice 2D: la récompense pour chaque état et action
Retourne la récompense.
*/
float getReward(RLEnv *e, int s, int a) {
    return e->rewards[s * e->nA + a];
}

/*
Prend en entrée un environnement: pour chaque état et chaque actions,
on retourne les probabilité d'atteindre chaque nouvel état.
Retourne un tableau 1D: les probabilités d'atteindre les nouveaux états.
*/
float *getTransitionArray(RLEnv *e, int s, int a) {
    int offset = (s * e->nA * e->nS) + (a * e->nS);
    return &(e->transitions[offset]);
}

/*
Prend en entrée un environnement: pour chaque état et chaque actions,
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