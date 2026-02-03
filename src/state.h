#ifndef __RL_STATE_H__
#define __RL_STATE_H__

#include <stdbool.h>
#include <stdio.h>

typedef struct s_rl_sate {
    int id;
    char *label;
    void *context;
} RLState;

#endif
