/*
 * "Classic" Perlin noise generation.
 */

#ifndef CLASSIC_H
#define CLASSIC_H

#include <math.h>

extern long seed;

static inline float perlin_interpolate(const float a, const float b, const float x);

static inline float perlin_noise_2d(const int x, const int y);
static float perlin_smooth_noise_2d(const int x, const int y);
float perlin_interpolated_noise_2d(const float x, const float y);

static inline float perlin_noise_3d(const int x, const int y, const int z);
static float perlin_smooth_noise_3d(const int x, const int y, const int z);
float perlin_interpolated_noise_3d(const float x, const float y, const float z);

#endif // CLASSIC_H