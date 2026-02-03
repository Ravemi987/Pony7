#include <stdio.h>
#include <stdlib.h>

#include "agent.h"
#include "action.h"
#include "state.h"


void scanPorts(void *args) {
    if (args) return;

    printf("Recherche de hash...\n");
    if (system("ls /tmp/secret_hash.txt > /dev/null 2>&1") != 0) return;
}

void exploitVuln(void *args) {
    if (args) return;

    printf("Cassage du hash avec hashcat (simule)...\n");
    if (system("sleep 1 && echo 'Password cracked: 123456'") != 0) return;
}

void lateralMovement(void *args) {
    if (args) return;

    printf("Tentative de sudo...\n");
    printf("Accès ROOT accordé.\n");
}

void exfiltration(void *args) {
    if (args) return;

    printf("Recuperation de la cible...\n");
    if (system("curl -s http://google.com | head -n 1") != 0) return;
}


void executeAgent(RLModel *m) {
    int currentState = 0;
    // On définit le but selon le nombre d'états (souvent le dernier)
    int goalState = 3; 
    
    printf("--- Lancement de l'Agent ---\n");

    // Sécurité : ne pas dépasser un nombre de pas maximum pour éviter les boucles infinies
    int steps = 0;
    while (currentState != goalState && steps < 20) {
        RLAction action = RLModelGetActionFromState(m, currentState);

        printf("\n[Etat %d] L'IA decide de faire : %s\n", currentState, action.name);
        
        action.execute(action.args);
        
        int nextState = RLModelGetNextState(m, currentState, action.id);
        
        if (nextState == currentState && action.id != 3) {
            printf("L'action a échoué (reste sur place).\n");
        }
        
        currentState = nextState;
        steps++;
    }

    RLAction action = RLModelGetActionFromState(m, currentState);
    printf("\n[Etat %d] L'IA decide de faire : %s\n", currentState, action.name);
    action.execute(action.args);

    if (currentState == goalState) {
        printf("\nMISSION REUSSIE.\n");
    } else {
        printf("\nMISSION ECHOUEE (Max steps atteint).\n");
    }

    printf("total reward: %f\n", RLModelGetReward(m, currentState, action.id));
}
