/*
Ruby module that is built according to the Perlin Noise function
located at http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
*/

#include "ruby.h"
#include "math.h"

VALUE rb_cPerlin;

static long seed = 0;

/*
The main initialize function which receives the inputs persistence and octave.
*/
VALUE Perlin_init(VALUE self, VALUE seed_value, VALUE persistence, VALUE octave)
{
    persistence = rb_funcall(persistence, rb_intern("to_f"), 0);
    rb_iv_set(self, "@persistence", persistence);
    rb_iv_set(self, "@octave", NUM2INT(octave));

    seed = seed_value;

    return self;
}

inline float perlin_interpolate(const float a, const float b, const float x)
{
    const float ft = x * M_PI;
    const float f = (1 - cos(ft)) * 0.5;
    return    a * (1 - f) + b * f;
}

inline float perlin_noise(const int x, const int y)
{
    long n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731*seed + 789221*seed) + 1376312589*seed) & 0x7fffffff) / 1073741824.0);
}

float perlin_smooth_noise(const int x, const int y)
{
    const float corners = (
        perlin_noise(x - 1, y - 1) +
        perlin_noise(x + 1, y - 1) +
        perlin_noise(x - 1, y + 1) +
        perlin_noise(x + 1, y + 1)
        ) / 4;
    const float sides     = (
        perlin_noise(x - 1, y) +
        perlin_noise(x + 1, y) +
        perlin_noise(x, y - 1) +
        perlin_noise(x, y + 1)
        ) /    4;
    const float center    =  perlin_noise(x, y) / 4;
    return corners + sides + center;
}

float perlin_interpolated_noise(const float x, const float y)
{
    const int integer_X         = (int)x;
    const float fractional_X    = x - integer_X;

    const int integer_Y         = (int)y;
    const float fractional_Y    = y - integer_Y;

    const float v1 = perlin_smooth_noise(integer_X,         integer_Y);
    const float v2 = perlin_smooth_noise(integer_X + 1, integer_Y);
    const float v3 = perlin_smooth_noise(integer_X,         integer_Y + 1);
    const float v4 = perlin_smooth_noise(integer_X + 1, integer_Y + 1);

    const float i1 = perlin_interpolate(v1, v2, fractional_X);
    const float i2 = perlin_interpolate(v3, v4, fractional_X);

    return perlin_interpolate(i1, i2, fractional_Y);
}

inline float perlin_noise_3d(const int x, const int y, const int z)
{
    long n = x + y + z * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731*seed + 789221*seed) + 1376312589*seed) & 0x7fffffff) / 1073741824.0);
}

float perlin_smooth_noise_3d(const int x, const int y, const int z)
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
    const float center = perlin_noise(x, y) / 12;
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

/*
Takes points (x, y) and returns a height (n)
*/
VALUE perlin_run2d(VALUE self, const VALUE x, const VALUE y)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = rb_iv_get(self, "@octave");
    float total = 0.;
    float frequency = 1., amplitude = 1.;
    int i;
    for (i = 0; i < n; ++i)
    {
        total += perlin_interpolated_noise(NUM2INT(x) * frequency, NUM2INT(y) * frequency) * amplitude;
        frequency *= 2;
        amplitude *= p;
    }

    return rb_float_new(total);
}

/*
Takes points (x, y, z) and returns a height (n)
*/
VALUE perlin_run3d(VALUE self, const VALUE x, const VALUE y, const VALUE z)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = rb_iv_get(self, "@octave");
    float total = 0.;
    float frequency = 1., amplitude = 1.;
    int i;
    for (i = 0; i < n; ++i)
    {
        total += perlin_interpolated_noise_3d(NUM2INT(x) * frequency, NUM2INT(y) * frequency, NUM2INT(z) * frequency) * amplitude;
        frequency *= 2;
        amplitude *= p;
    }

    return rb_float_new(total);
}

/*
Returns a chunk of coordinates starting from x, y and of size size_x, size_y.
*/
VALUE perlin_chunk2d(VALUE self, VALUE x, VALUE y, VALUE size_x, VALUE size_y)
{
    VALUE arr = rb_ary_new();
    int i, j;
    for (i = NUM2INT(x); i < NUM2INT(size_x) + NUM2INT(x); i++)
    {
        VALUE row = rb_ary_new();
        for (j = NUM2INT(y); j < NUM2INT(size_y) + NUM2INT(y); j++)
        {
            rb_ary_push(row, perlin_run2d(self, INT2NUM(i), INT2NUM(j)));
        }
        rb_ary_push(arr, row);
    }
    return arr;
}

void Init_perlin() {
    VALUE jm_Module = rb_define_module("Perlin");
    VALUE rb_cPerlin = rb_define_class_under(jm_Module, "Noise", rb_cObject);

    rb_define_method(rb_cPerlin, "initialize", Perlin_init, 3);
    rb_define_method(rb_cPerlin, "run2d", perlin_run2d, 2);
    rb_define_method(rb_cPerlin, "run3d", perlin_run3d, 3);
    rb_define_method(rb_cPerlin, "chunk2d", perlin_chunk2d, 4);
}

