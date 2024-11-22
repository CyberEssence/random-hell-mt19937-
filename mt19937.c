#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define MT19937_N 624
#define MT19937_M 397
#define MT19937_W 32
#define MT19937_F ((uint32_t)0x9908b0df)
#define MT19937_U 11
#define MT19937_S 7
#define MT19937_T 15
#define MT19937_L 18
#define MT19937_A 0x9908b0df
#define MT19937_B 0x9d2c5680
#define MT19937_C 0xefc60000
#define MT19937_D 0xffffffff
#define MT19937_MASK_LOWER ((1ULL << MT19937_W) - 1)
#define MT19937_MASK_UPPER (~MT19937_MASK_LOWER)

uint32_t mt19937_state[MT19937_N];
int mt19937_index = MT19937_N + 1;

void twist() {
    for (int i = 0; i < MT19937_N; i++) {
        uint32_t x = (mt19937_state[i] & MT19937_MASK_UPPER) + (mt19937_state[(i + 1) % MT19937_N] & MT19937_MASK_LOWER);
        uint32_t xA = x >> 1;
        if (x % 2 != 0) {
            xA = xA ^ MT19937_A;
        }
        mt19937_state[i] = mt19937_state[(i + MT19937_M) % MT19937_N] ^ xA;
    }
    mt19937_index = 0;
}

void mt19937_init(uint32_t seed) {
    mt19937_state[0] = seed;
    for (int i = 1; i < MT19937_N; i++) {
        mt19937_state[i] = (MT19937_F * (mt19937_state[i - 1] ^ (mt19937_state[i - 1] >> 30)) + i) & 0xffffffff;
    }
}

uint32_t mt19937_extract() {
    if (mt19937_index >= MT19937_N) {
        if (mt19937_index > MT19937_N) {
            mt19937_init(5489);
        }
        twist();
    }
    
    uint32_t y = mt19937_state[mt19937_index];
    y = y ^ ((y >> MT19937_U) & MT19937_D);
    y = y ^ ((y << MT19937_S) & MT19937_B);
    y = y ^ ((y << MT19937_T) & MT19937_C);
    y = y ^ (y >> MT19937_L);
    
    mt19937_index++;
    return y;
}

int main() {
    uint32_t seed = time(NULL);
    mt19937_init(seed);
    
    printf("Generated random numbers:\n");
    for (int i = 0; i < 10; i++) {
        printf("%u ", mt19937_extract());
    }
    printf("\n");

    return 0;
}
