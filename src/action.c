#include <stdlib.h>

#include "action.h"

RLAction RLActionCreate(char *name, void (*exec)(void *args), 
                       void *args) {
    return (RLAction) {
        .execute = exec,
        .args = args,
        .name = name
    };
}
