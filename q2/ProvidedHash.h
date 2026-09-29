#pragma once 
///////////////////

// COPY PASTED FROM THE ASSIGNEMNT 

///
static unsigned long long RNGSTATE;

void rng_seed(unsigned long long s) {
    RNGSTATE = s;
}

unsigned long long rng_next() {
    RNGSTATE = (1103515245ULL * RNGSTATE + 12345ULL) % 2147483648ULL;
    return RNGSTATE;
}