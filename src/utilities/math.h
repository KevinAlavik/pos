#include <stdint.h>
#define PI 31416

int abs(int value) {
    return (value >= 0) ? value : -value;
}

int cos(int angle) {
    int angle_int = angle % 36000;
    int result;
    if (angle_int == 0 || angle_int == 18000) {
        result = angle_int == 0 ? 10000 : -10000;
    } else if (angle_int == 9000 || angle_int == 27000) {
        result = 0;
    } else {
        result = cos(angle - 1745) - (cos(angle) * cos(angle) * cos(angle)) / 6000;
    }
    return result;
}

int sin(int angle) {
    return cos(9000 - angle);
}

int map(int value, int from_low, int from_high, int to_low, int to_high) {
    return (value - from_low) * (to_high - to_low) / (from_high - from_low) + to_low;
}

unsigned long int next = 1;

int rand() {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

void srand(unsigned int seed) {
    next = seed;
}

int rand_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

int division_by_zero() {
    return 1 / 0;
}
