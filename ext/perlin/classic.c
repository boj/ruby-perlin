#include "classic.h"

long seed = 0; // Speeds up lookup of this value when it is accessed a LOT. PROBABLY NOT THREAD SAFE!

static inline float perlin_interpolate(const float a, const float b, const float x)
{
    const float ft = x * M_PI;
    const float f = (1 - cos(ft)) * 0.5;
    return    a * (1 - f) + b * f;
}


// 2D ------------------------------------------------------------------

static inline float perlin_noise_2d(const int x, const int y)
{
    long n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731*seed + 789221*seed) + 1376312589*seed) & 0x7fffffff) / 1073741824.0);
}

static float perlin_smooth_noise_2d(const int x, const int y)
{
    const float corners = (
        perlin_noise_2d(x - 1, y - 1) +
        perlin_noise_2d(x + 1, y - 1) +
        perlin_noise_2d(x - 1, y + 1) +
        perlin_noise_2d(x + 1, y + 1)
        ) / 4;
    const float sides     = (
        perlin_noise_2d(x - 1, y) +
        perlin_noise_2d(x + 1, y) +
        perlin_noise_2d(x, y - 1) +
        perlin_noise_2d(x, y + 1)
        ) /    4;
    const float center    =  perlin_noise_2d(x, y) / 4;
    return corners + sides + center;
}

float perlin_interpolated_noise_2d(const float x, const float y)
{
    const int integer_X         = (int)x;
    const float fractional_X    = x - integer_X;

    const int integer_Y         = (int)y;
    const float fractional_Y    = y - integer_Y;

    const float v1 = perlin_smooth_noise_2d(integer_X,         integer_Y);
    const float v2 = perlin_smooth_noise_2d(integer_X + 1, integer_Y);
    const float v3 = perlin_smooth_noise_2d(integer_X,         integer_Y + 1);
    const float v4 = perlin_smooth_noise_2d(integer_X + 1, integer_Y + 1);

    const float i1 = perlin_interpolate(v1, v2, fractional_X);
    const float i2 = perlin_interpolate(v3, v4, fractional_X);

    return perlin_interpolate(i1, i2, fractional_Y);
}

float perlin_octaves_2d(const float x, const float y, const float p, const float n)
{
    float total = 0.;
    float frequency = 1., amplitude = 1.;
    int i;

    for (i = 0; i < n; ++i)
    {
        total += perlin_interpolated_noise_2d(x * frequency, y * frequency) * amplitude;
        frequency *= 2;
        amplitude *= p;
    }

    return total;
}

// 3D ------------------------------------------------------------------

static inline float perlin_noise_3d(const int x, const int y, const int z)
{
    long n = x + y + z * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731*seed + 789221*seed) + 1376312589*seed) & 0x7fffffff) / 1073741824.0);
}

static float perlin_smooth_noise_3d(const int x, const int y, const int z)
{
    const float corners = (
        perlin_noise_3d(x - 1, y - 1, z + 1) +
        perlin_noise_3d(x + 1, y - 1, z + 1) +
        perlin_noise_3d(x - 1, y + 1, z + 1) +
        perlin_noise_3d(x + 1, y + 1, z + 1) +
        perlin_noise_3d(x - 1, y - 1, z - 1) +
        perlin_noise_3d(x + 1, y - 1, z - 1) +
        perlin_noise_3d(x - 1, y + 1, z - 1) +
        perlin_noise_3d(x + 1, y + 1, z - 1)
        ) / 16;
    const float sides = (
        perlin_noise_3d(x - 1, y, z + 1) +
        perlin_noise_3d(x + 1, y, z + 1) +
        perlin_noise_3d(x, y - 1, z + 1) +
        perlin_noise_3d(x, y + 1, z + 1) +
        perlin_noise_3d(x - 1, y, z - 1) +
        perlin_noise_3d(x + 1, y, z - 1) +
        perlin_noise_3d(x, y - 1, z - 1) +
        perlin_noise_3d(x, y + 1, z - 1) +
        perlin_noise_3d(x - 1, y - 1, z) +
        perlin_noise_3d(x + 1, y - 1, z) +
        perlin_noise_3d(x + 1, y + 1, z) +
        perlin_noise_3d(x - 1, y + 1, z)
        ) / 8;
    const float center = perlin_noise_2d(x, y) / 12;
    return corners + sides + center;
}

float perlin_interpolated_noise_3d(const float x, const float y, const float z)
{
    const int integer_X                = (int)x;
    const float fractional_X    = x - integer_X;

    const int integer_Y                = (int)y;
    const float fractional_Y    = y - integer_Y;

    const int integer_Z                = (int)z;
    const float fractional_Z    = z - integer_Z;

    const float a = perlin_smooth_noise_3d(integer_X,         integer_Y,             integer_Z);
    const float b = perlin_smooth_noise_3d(integer_X + 1, integer_Y,             integer_Z);
    const float c = perlin_smooth_noise_3d(integer_X,         integer_Y + 1,     integer_Z);
    const float d = perlin_smooth_noise_3d(integer_X,         integer_Y,             integer_Z + 1);
    const float e = perlin_smooth_noise_3d(integer_X + 1, integer_Y + 1,     integer_Z);
    const float f = perlin_smooth_noise_3d(integer_X,         integer_Y + 1,     integer_Z + 1);
    const float g = perlin_smooth_noise_3d(integer_X + 1, integer_Y,             integer_Z + 1);
    const float h = perlin_smooth_noise_3d(integer_X + 1, integer_Y + 1,     integer_Z + 1);

    const float i1 = perlin_interpolate(a, b, fractional_X);
    const float i2 = perlin_interpolate(c, d, fractional_X);
    const float i3 = perlin_interpolate(e, f, fractional_X);
    const float i4 = perlin_interpolate(g, h, fractional_X);

    const float y1 = perlin_interpolate(i1, i2, fractional_Y);
    const float y2 = perlin_interpolate(i3, i4, fractional_Y);

    return perlin_interpolate(y1, y2, fractional_Z);
}

float perlin_octaves_3d(const float x, const float y, const float z, const float p, const float n)
{
    float total = 0.;
    float frequency = 1., amplitude = 1.;
    int i;

    for (i = 0; i < n; ++i)
    {
        total += perlin_interpolated_noise_3d(x * frequency, y * frequency, z * frequency) * amplitude;
        frequency *= 2;
        amplitude *= p;
    }

    return total;
}

