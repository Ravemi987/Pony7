#include <stdio.h>
#include <stdlib.h>

#include "agent.h"
#include "action.h"
#include "state.h"


void executeAgent(RLModel *m, RLAgent *agent) {
    int currentState = agent->initialState;
    int goalState = agent->goalState; 
    
    printf("--- Lancement de l'Agent ---\n");

    int steps = 0;

    while (currentState != goalState && steps < agent->maxIter) {
        RLAction action = RLModelGetBestAction(m, currentState);
        
        action.execute(action.args);
        
        int nextState = RLModelGetNextState(m, currentState, action.id);
        
        currentState = nextState;
        steps++;
    }

    RLAction action = RLModelGetBestAction(m, currentState);
    action.execute(action.args);

    agent->reward = RLModelGetReward(m, currentState, action.id);
    agent->sucess = (currentState == goalState);
}
