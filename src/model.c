#include "model.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdint.h>
#include <math.h>


/*
Définition d'un modèle 
*/
struct s_rl_model {
    // Environnement contenant les données, les fonctions, ...
    RLEnv *userData;

    // Tableau de valeur des états
    float *stateValues;
    
    // Politique
    int *policy;

    // Config associée au modèle
    RLConfig config;
};

RLModel* RLModelCreate(RLEnv *userData) {
    RLModel *m = malloc(sizeof(struct s_rl_model));
    m->userData = userData;

    m->stateValues = calloc(userData->nS, sizeof(float));
    m->policy = calloc(userData->nS, sizeof(int));

    // On a une config par défaut
    m->config = RLConfigDefault();

    return m;
};

/*
Libère le modèle.
*/
void RLModelDelete(RLModel **m) {
    if ((*m) == NULL) return;

    free((*m)->stateValues);
    free((*m) ->policy);
    free(*(m));

    (*m) = NULL;
}

/* Getters */

float *RLModelGetStatesValues(RLModel *m) {
    return m->stateValues;
}

int *RLModelGetPolicy(RLModel *m) {
    return m->policy;
}

/* Algorithmes */

void valueIteration(RLModel *m) {
    float epsilon = m->config.epsilon;
    float gamma = m->config.gamma;
    float delta = DBL_MAX;
    RLEnv *env = m->userData;

    while (delta > epsilon) {
        delta = 0;

        for (int s = 0; s < env->nS; ++s) {
            float v_old = m->stateValues[s];
            float q_max = -DBL_MAX;

            for (int a = 0; a < env->nA; ++a) {
                float q = env->Reward(env, s, a) + gamma * sum(
                    env->Transition(env, s, a), m->stateValues, env->nS
                );

                if (q > q_max) {
                    q_max = q;
                    m->policy[s] = a;
                }
            }

            m->stateValues[s] = q_max;
            delta = fmax(delta, fabs(m->stateValues[s] - v_old));
        }
    }
}
