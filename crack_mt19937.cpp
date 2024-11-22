#include <iostream>
#include <random>
#include <ctime>
#include <unistd.h>

unsigned long temper(unsigned long y) {
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= (y >> 18);
    return y;
}

class MT19937 {
private:
    unsigned long mt[624];
    int index;

    void twist() {
        for (int i = 0; i < 624; i++) {
            unsigned long y = (mt[i] & 0x80000000) + (mt[(i + 1) % 624] & 0x7fffffff);
            mt[i] = mt[(i + 397) % 624] ^ (y >> 1);
            if (y % 2 != 0) {
                mt[i] ^= 0x9908b0df;
            }
        }
        index = 0;
    }

public:
    MT19937(unsigned long seed) {
        mt[0] = seed;
        for (int i = 1; i < 624; i++) {
            mt[i] = 0xffffffff & (1812433253 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
        }
        index = 0;
    }

    unsigned long temper() {
        if (index == 0) {
            twist();
        }

        unsigned long y = mt[index];
        y = y ^ (y >> 11);
        y = y ^ ((y << 7) & 0x9D2C5680);
        y = y ^ ((y << 15) & 0xEFC60000);
        y = y ^ (y >> 18);

        index = (index + 1) % 624;
        return y;
    }
};

int main() {
    srand(time(0));
    int min_sleep_time = 40;
    int max_sleep_time = 100;

    int sleep_time_1 = rand() % (max_sleep_time - min_sleep_time + 1) + min_sleep_time;
    std::cout << "Sleep time 1: " << sleep_time_1 << std::endl;
    sleep(sleep_time_1);

    unsigned long seed1 = time(0);
    MT19937 rng(seed1);
    std::cout << "Actual seed: " << seed1 << std::endl;

    int sleep_time_2 = rand() % (max_sleep_time - min_sleep_time + 1) + min_sleep_time;
    std::cout << "Sleep time 2: " << sleep_time_2 << std::endl;
    sleep(sleep_time_2);

    unsigned long number = rng.temper();
    unsigned long seed2 = time(0);
    for (unsigned long i = time(0) - max_sleep_time; i < time(0) - min_sleep_time; i++) {
        MT19937 rng(i);
        unsigned long tmp_num = rng.temper();
        if (tmp_num == number) {
            seed2 = i;
            std::cout << "Guessed seed: " << seed2 << std::endl;
            break;
        }
    }

    return 0;
}
