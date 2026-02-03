#ifndef __RL_ENV_H__
#define __RL_ENV_H__

#include <stdbool.h>
#include <stdio.h>

#include "state.h"
#include "action.h"

// Forward déclaration
typedef struct s_rl_model RLModel;

typedef struct s_rl_env RLEnv;

/* 
Environnement avec un tableau de transition (3D)
et un tableau de récompenses (1D).
Il contient aussi les fonctions d'accès à cet environnement.
Un environnement est une interface.
Il est connu de l'utilisateur qui lui donne des fonctions de transitions
et de récompenses. En effet, ces fonctions peuvent être dynamiques 
(Si tel état réuni telles conditions, alors la transition sera ...), 
plutôt qu'accéder à un tableau.
*/
struct s_rl_env {
    // Dimensions des données
    int nS;
    int nA;

    // Tableaux en entrée
    RLState *states;
    RLAction *actions;

    // Tableaux de données
    float *transitions;
    float *rewards;

    // Fonction de générations des tableaux de données
    int (*Generate)(RLState *, RLAction*, float**, float**);

    // Fonctions génériques de transition et de récompenses
    // Ces fonctions peuvent être dynamique (pas toujours un accès à un tableau)
    int (*TransitionState)(RLEnv*, int, int);
    float *(*TransitionArray)(RLEnv*, int, int);
    float (*Reward)(RLEnv*, int, int);
};

RLEnv *RLEnvCreate(int nStates, int nActions,
                   RLState *states, RLAction *actions,
                   int (*Generate)(RLState *, RLAction*, float**, float**),
                   int (*TransitionState)(RLEnv*, int, int),
                   float *(*TransitionArray)(RLEnv*, int, int),
                   float (*Reward)(RLEnv*, int, int));

void RLEnvDelete(RLEnv **env);

int RLEnvGetStatesNumber(RLEnv *e);

float *getTransitionArray(RLEnv *e, int s, int a);

int getTransitionState(RLEnv *e, int s, int a);

float getReward(RLEnv *e, int s, int a);

int RLEnvGetActionsNumber(RLEnv *e);

#endif
