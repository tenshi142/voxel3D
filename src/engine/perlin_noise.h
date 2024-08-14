#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <stdlib.h>
#include <math.h>

class PerlinNoise {
public:
    PerlinNoise();
    double noise(double x, double y, double z) const;

private:
    int* p;

    double fade(double t) const;
    double lerp(double t, double a, double b) const;
    double grad(int hash, double x, double y, double z) const;
};

#endif // PERLIN_NOISE_H
