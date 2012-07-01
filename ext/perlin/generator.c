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

    seed = NUM2INT(rb_iv_get(self, "@seed")); // Store in global, for speed.

    return rb_float_new(perlin_octaves_2d(NUM2INT(x), NUM2INT(y), p, n));
}

/*
Takes points (x, y, z) and returns a height (n)
*/
VALUE Perlin_Generator_run3d(VALUE self, const VALUE x, const VALUE y, const VALUE z)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));

    seed = NUM2INT(rb_iv_get(self, "@seed")); // Store in global, for speed.

    return rb_float_new(perlin_octaves_3d(NUM2INT(x), NUM2INT(y), NUM2INT(z), p, n));
}

/*
Returns a chunk of coordinates starting from x, y and of size size_x, size_y.
*/
VALUE Perlin_Generator_chunk2d(VALUE self, VALUE x, VALUE y, VALUE size_x, VALUE size_y)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));

    VALUE arr, row;
    int i, j;

    int x_min = NUM2INT(x);
    int x_max = x_min + NUM2INT(size_x);
    int y_min = NUM2INT(y);
    int y_max = y_min + NUM2INT(size_y);

    seed = NUM2INT(rb_iv_get(self, "@seed")); // Store in global, for speed.

    if(rb_block_given_p())
    {
       for (i = y_min; i < y_max; i++)
       {
           for (j = x_min; j < x_max; j++)
           {
               rb_yield_values(3, rb_float_new(perlin_octaves_2d(j, i, p, n)), INT2NUM(j), INT2NUM(i));
           }
       }

       return Qnil;
    }
    else
    {
        arr = rb_ary_new();
        for (i = x_min; i < x_max; i++)
        {
            row = rb_ary_new();
            for (j = y_min; j < y_max; j++)
            {
                rb_ary_push(row, rb_float_new(perlin_octaves_2d(i, j, p, n)));
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
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));

    VALUE arr, row, column;
    int i, j, k;

    int x_min = NUM2INT(x);
    int x_max = x_min + NUM2INT(size_x);
    int y_min = NUM2INT(y);
    int y_max = y_min + NUM2INT(size_y);
    int z_min = NUM2INT(z);
    int z_max = z_min + NUM2INT(size_z);

    seed = NUM2INT(rb_iv_get(self, "@seed")); // Store in global, for speed.

    if(rb_block_given_p())
    {
        for (i = z_min; i < z_max; i++)
        {
            for (j = y_min; j < y_max; j++)
            {
                for (k = x_min; k < x_max; k++)
                {
                    rb_yield_values(4, rb_float_new(perlin_octaves_3d(k, j, i, p, n)), INT2NUM(k), INT2NUM(j), INT2NUM(i));
                }
            }
        }
        return Qnil;
    }
    else
    {
        arr = rb_ary_new();
        for (i = x_min; i < x_max; i++)
        {
            row = rb_ary_new();
            for (j = y_min; j < y_max; j++)
            {
                column = rb_ary_new();
                for (k = z_min; k < z_max; k++)
                {
                    rb_ary_push(column, rb_float_new(perlin_octaves_3d(i, j, k, p, n)));
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