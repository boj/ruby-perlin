#include "generator.h"

VALUE Perlin_Generator_set_seed(VALUE self, VALUE seed)
{
    rb_iv_set(self, "@seed", rb_funcall(seed, rb_intern("to_i"), 0));
}

VALUE Perlin_Generator_set_persistence(VALUE self, VALUE persistence)
{
   rb_iv_set(self, "@persistence", rb_funcall(persistence, rb_intern("to_f"), 0));
}

VALUE Perlin_Generator_set_octave(VALUE self, VALUE octave)
{
    rb_iv_set(self, "@octave", rb_funcall(octave, rb_intern("to_i"), 0));
}

/*
Takes points (x, y) and returns a height (n)
*/
VALUE Perlin_Generator_run2d(VALUE self, const VALUE x, const VALUE y)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));
    float total = 0.;
    float frequency = 1., amplitude = 1.;
    int i;

    seed = NUM2INT(rb_iv_get(self, "@seed")); // Store in global, for speed.

    for (i = 0; i < n; ++i)
    {
        total += perlin_interpolated_noise_2d(NUM2INT(x) * frequency, NUM2INT(y) * frequency) * amplitude;
        frequency *= 2;
        amplitude *= p;
    }

    return rb_float_new(total);
}

/*
Takes points (x, y, z) and returns a height (n)
*/
VALUE Perlin_Generator_run3d(VALUE self, const VALUE x, const VALUE y, const VALUE z)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));
    float total = 0.;
    float frequency = 1., amplitude = 1.;
    int i;

    seed = NUM2INT(rb_iv_get(self, "@seed")); // Store in global, for speed.

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
VALUE Perlin_Generator_chunk2d(VALUE self, VALUE x, VALUE y, VALUE size_x, VALUE size_y)
{
    VALUE arr, row;
    int i, j;

    if(rb_block_given_p())
    {
       for (i = NUM2INT(y); i < NUM2INT(size_y) + NUM2INT(y); i++)
       {
           for (j = NUM2INT(x); j < NUM2INT(size_x) + NUM2INT(x); j++)
           {
               rb_yield_values(3, Perlin_Generator_run2d(self, INT2NUM(j), INT2NUM(i)), INT2NUM(j), INT2NUM(i));
           }
       }

       return Qnil;
    }
    else
    {
        arr = rb_ary_new();
        for (i = NUM2INT(x); i < NUM2INT(size_x) + NUM2INT(x); i++)
        {
            row = rb_ary_new();
            for (j = NUM2INT(y); j < NUM2INT(size_y) + NUM2INT(y); j++)
            {
                rb_ary_push(row, Perlin_Generator_run2d(self, INT2NUM(i), INT2NUM(j)));
            }
            rb_ary_push(arr, row);
        }
        return arr;
    }
}

/*
Returns a chunk of coordinates starting from x, y, z and of size size_x, size_y, size_z.
*/
VALUE Perlin_Generator_chunk3d(VALUE self, VALUE x, VALUE y, VALUE z, VALUE size_x, VALUE size_y, VALUE size_z)
{
    VALUE arr, row, column;
    int i, j, k;

    if(rb_block_given_p())
    {
        for (i = NUM2INT(z); i < NUM2INT(size_z) + NUM2INT(z); i++)
        {
            for (j = NUM2INT(y); j < NUM2INT(size_y) + NUM2INT(y); j++)
            {
                for (k = NUM2INT(x); k < NUM2INT(size_x) + NUM2INT(x); k++)
                {
                    rb_yield_values(4, Perlin_Generator_run3d(self, INT2NUM(k), INT2NUM(j), INT2NUM(i)), INT2NUM(k), INT2NUM(j), INT2NUM(i));
                }
            }
        }
        return Qnil;
    }
    else
    {
        arr = rb_ary_new();
        for (i = NUM2INT(x); i < NUM2INT(size_x) + NUM2INT(x); i++)
        {
            row = rb_ary_new();
            for (j = NUM2INT(y); j < NUM2INT(size_y) + NUM2INT(y); j++)
            {
                column = rb_ary_new();
                for (k = NUM2INT(z); k < NUM2INT(size_z) + NUM2INT(z); k++)
                {
                    rb_ary_push(column, Perlin_Generator_run3d(self, INT2NUM(i), INT2NUM(j), INT2NUM(k)));
                }
                rb_ary_push(row, column);
            }
            rb_ary_push(arr, row);
        }
        return arr;
    }
}

/*
The main initialize function which receives the inputs persistence and octave.
*/
VALUE Perlin_Generator_init(VALUE self, VALUE seed, VALUE persistence, VALUE octave)
{
    Perlin_Generator_set_seed(self, seed);
    Perlin_Generator_set_persistence(self, persistence);
    Perlin_Generator_set_octave(self, octave);

    return self;
}