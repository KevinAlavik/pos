#define PI 3.14159265

int abs(int value) {
    return (value >= 0) ? value : -value;
}

float fabs(float value) {
    return (value < 0) ? -value : value;
}

float cos(float angle) {
    int angle_int = (int)angle;
    float result;
    if (angle_int % 360 == 0 || angle_int % 360 == 180) {
        result = angle_int % 360 == 0 ? 1 : -1;
    } else if (angle_int % 360 == 90 || angle_int % 360 == 270) {
        result = 0;
    } else {
        result = cos(angle - 0.0174533) - (cos(angle) * cos(angle) * cos(angle)) / 6;
    }
    return result;
}

float sin(float angle) {
    return cos(90 - angle);
}

uint64_t llabs(int64_t num) {
    return (num < 0) ? -num : num;
}