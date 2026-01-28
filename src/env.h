#ifndef __RL_ENV_H__
#define __RL_ENV_H__

#include <stdbool.h>
#include <stdio.h>

// Forward déclaration
typedef struct s_rl_model RLModel;

typedef struct s_rl_env RLEnv;

/* 
Environnement avec un tableau de transition
et un tableau de récompenses (1D).
Il contient aussi les fonctions d'accès à cet environnement
*/
struct s_rl_env {
    // Dimensions des données
    int nS;
    int nA;

    // Tableaux en entrée
    float *transitions;
    float *rewards;

    // Fonctions génériques de transition et de récompenses
    int (*TransitionState)(RLEnv*, int, int);
    float *(*TransitionArray)(RLEnv*, int, int);
    float (*Reward)(RLEnv*, int, int);
};

RLEnv *RLEnvCreate(int nStates, int nActions,
                   float *tData, float *rData,
                   int (*TransitionState)(RLEnv*, int, int),
                   float *(*Transition)(RLEnv*, int, int),
                   float (*Reward)(RLEnv*, int, int));

void RLEnvDelete(RLEnv **env);

int RLEnvGetStatesNumber(RLEnv *e);

float *getTransitionArray(RLEnv *e, int s, int a);

int getTransitionState(RLEnv *e, int s, int a);

float getReward(RLEnv *e, int s, int a);

int RLEnvGetActionsNumber(RLEnv *e);

#endif
