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

    friend void twist_MT19937(MT19937& mt);

public:
    MT19937(unsigned long seed) {
        mt[0] = seed;
        for (int i = 1; i < 624; i++) {
            mt[i] = 0xffffffff & (1812433253 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
        }
        index = 0;
    }

    void setMt(int index, long unsigned int value) {
        mt[index] = value;
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

void twist_MT19937(MT19937& mt) {
        mt.twist();
}

int calcSleepTime(int min_sleep_time, int max_sleep_time) {
        return rand() % (max_sleep_time - min_sleep_time + 1) + min_sleep_time;
}

int main() {
    srand(time(0));
    int minSleepTime = 40;
    int maxSleepTime = 100;

    int firstSleepTime = calcSleepTime(minSleepTime, maxSleepTime);
    std::cout << "Sleep time 1: " << firstSleepTime << std::endl;
    sleep(firstSleepTime);

    unsigned long firstSeed = time(0);
    MT19937 rng(firstSeed);
    std::cout << "Actual seed: " << firstSeed << std::endl;

    int secondSleepTime = calcSleepTime(minSleepTime, maxSleepTime);
    std::cout << "Sleep time 2: " << secondSleepTime << std::endl;
    sleep(secondSleepTime);

    unsigned long number = rng.temper();
    unsigned long secondSeed = time(0);
    for (unsigned long i = time(0) - maxSleepTime; i < time(0) - minSleepTime; i++) {
        MT19937 rng(i);
        unsigned long tmpNum = rng.temper();
        if (tmpNum == number) {
            secondSeed = i;
            std::cout << "Guessed seed: " << secondSeed << std::endl;
            break;
        }
    }

    return 0;
}
