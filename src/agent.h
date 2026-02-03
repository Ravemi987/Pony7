#ifndef __RL_AGENT_H__
#define __RL_AGENT_H__

#include "model.h"

void executeAgent(RLModel *m);

void scanPorts(void *args);
void exploitVuln(void *args);
void lateralMovement(void *args);
void exfiltration(void *args);

#endif
