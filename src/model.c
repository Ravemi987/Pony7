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

    // Q-table
    float *QTable;

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
    m->QTable = calloc(userData->nS * userData->nA, sizeof(float));

    // On a une config par défaut
    m->config = RLConfigCreate();

    return m;
};

/*
Libère le modèle.
*/
void RLModelDelete(RLModel **m) {
    if ((*m) == NULL) return;

    free((*m)->stateValues);
    free((*m) ->policy);
    free((*m)->QTable);
    free(*(m));

    (*m) = NULL;
}

/* Setters */

void RLModelSetConfig(RLModel *m, RLConfig cfg) {
    m->config = cfg;
}

/* Getters */

float *RLModelGetStatesValues(RLModel *m) {
    return m->stateValues;
}

int *RLModelGetPolicy(RLModel *m) {
    return m->policy;
}

float *RLModelGetQTable(RLModel *m) {
    return m->QTable;
}

RLConfig* RLModelGetConfig(RLModel *m) {
    return &(m->config);
}

/* Printing */

void RLModelPrintStatesValues(RLModel *m) {
    printFloatArray(m->stateValues, m->userData->nS);
    printf("\n");
}

void RLModelPrintPolicy(RLModel *m) {
    printIntArray(m->policy, m->userData->nS);
    printf("\n");
}

void RLModelPrintQTable(RLModel *m) {
    printFloatMatrix(m->QTable, m->userData->nS, m->userData->nA);
    printf("\n");
}


/* Algorithmes */

/* ------------------------------- */

void valueIteration(RLModel *m) {
    float epsilon = m->config.epsilon;
    float gamma = m->config.gamma;
    RLEnv *env = m->userData;
    float delta = DBL_MAX;

    while (delta > epsilon) {
        delta = 0;

        for (int s = 0; s < env->nS; ++s) {
            float v_old = m->stateValues[s];
            float q_max = -DBL_MAX;

            for (int a = 0; a < env->nA; ++a) {
                float q = env->Reward(env, s, a) + gamma * sum(
                    env->TransitionArray(env, s, a), m->stateValues, env->nS
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

/* ------------------------------- */

void policyEvaluation(RLModel *m, int *policy) {
    float epsilon = m->config.epsilon;
    float gamma = m->config.gamma;
    RLEnv *env = m->userData;
    float delta = DBL_MAX;

    while (delta > epsilon) {
        delta = 0;

        for (int s = 0; s < env->nS; ++s) {
            float v_old = m->stateValues[s];

            int a = policy[s];

            m->stateValues[s] = env->Reward(env, s, a) + gamma * sum(
                env->TransitionArray(env, s, a), m->stateValues, env->nS
            );

            delta = fmax(delta, fabs(m->stateValues[s] - v_old));
        }
    }
}

bool policyImprovement(RLModel *m, int *policy) {
    float gamma = m->config.gamma;
    RLEnv *env = m->userData;

    bool policy_stable = true;

    for (int s = 0; s < env->nS; ++s) {
        int old_action = policy[s];

        float q_max = -DBL_MAX;
        int best_action = old_action;

        for (int a = 0; a < env->nA; ++a) {
            float q = env->Reward(env, s, a) + gamma * sum(
                env->TransitionArray(env, s, a), m->stateValues, env->nS
            );

            if (q > q_max + 1e-7) {
                q_max = q; 
                best_action = a;
            }
        }

        policy[s] = best_action;

        if (old_action != best_action) policy_stable = false;
    }

    return policy_stable;
}

void policyIteration(RLModel *m) {
    arrayRandom(m->policy, m->userData->nS, m->userData->nA);

    bool policy_stable = false;

    while (!policy_stable) {
        policyEvaluation(m, m->policy);
        policy_stable = policyImprovement(m, m->policy);
    }
}

/* ------------------------------- */

void QLearning(RLModel *m) {
    float alpha = m->config.alpha;
    float gamma = m->config.gamma;
    RLEnv *env = m->userData;

    for (int epoch = 0; epoch < m->config.epochs; ++epoch) {
        int state = 0;

        for (int step = 0; step < m->config.steps; ++step) {
            float r = (float)rand() / (float)RAND_MAX;
            int action;

            if (r < m->config.epsilon) {
                action =  rand() % env->nA;
            } else {
                action = arrayMaxIndex(&(m->QTable[state * env->nA]), env->nA);
            }

            m->policy[state] = action;

            int next_state = env->TransitionState(env, state, action);
            float reward = env->Reward(env, state, action);

            float best_next_q = arrayMax(&(m->QTable[next_state * env->nA]), env->nA);
            m->QTable[state * env->nA + action] += alpha * (reward + (gamma * best_next_q) - m->QTable[state * env->nA + action]);

            state = next_state;
        }
    }
}
