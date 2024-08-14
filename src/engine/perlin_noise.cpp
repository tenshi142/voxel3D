#include "perlin_noise.h"

PerlinNoise::PerlinNoise() {
    // Permutation table
    // Initialize with random values between 0 and 255
    p = new int[512];
    for (int i = 0; i < 512; ++i) {
        p[i] = rand() % 256;
    }
}

double PerlinNoise::noise(double x, double y, double z) const {
    // Calculate the Perlin noise value at the given coordinates
    int X = static_cast<int>(floor(x)) & 255;
    int Y = static_cast<int>(floor(y)) & 255;
    int Z = static_cast<int>(floor(z)) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                grad(p[BA], x - 1, y, z)),
                       lerp(u, grad(p[AB], x, y - 1, z),
                            grad(p[BB], x - 1, y - 1, z))),
               lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                            grad(p[BA + 1], x - 1, y, z - 1)),
                       lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                            grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

double PerlinNoise::fade(double t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) const {
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) const {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
