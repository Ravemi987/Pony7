#include "config.h"
#include <stdlib.h>


RLConfig RLConfigDefault(void) {
    return (RLConfig){
        .gamma = 0.2,
        .epsilon = 1e-4,
        .alpha = 1,
        .steps = 10,
        .epochs = 100
    };
}
