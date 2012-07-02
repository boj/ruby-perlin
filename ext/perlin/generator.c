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

VALUE Perlin_Generator_set_classic(VALUE self, VALUE classic)
{
    rb_iv_set(self, "@classic", classic);
}


/*
Takes points (x, y) and returns a height (n)
*/
VALUE Perlin_Generator_run2d(VALUE self, const VALUE x, const VALUE y)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));
    const VALUE classic = rb_iv_get(self, "@classic");

    seed = NUM2LONG(rb_iv_get(self, "@seed")); // Store in global, for speed.

    if(RTEST(classic))
    {
        return rb_float_new(perlin_octaves_2d(NUM2DBL(x), NUM2DBL(y), p, n));
    }
    else
    {
        return rb_float_new(octave_noise_2d(n, p, 1.0, NUM2DBL(x), NUM2DBL(y)));
    }
}

/*
Takes points (x, y, z) and returns a height (n)
*/
VALUE Perlin_Generator_run3d(VALUE self, const VALUE x, const VALUE y, const VALUE z)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));
    const VALUE classic = rb_iv_get(self, "@classic");

    seed = NUM2LONG(rb_iv_get(self, "@seed")); // Store in global, for speed.

    if(RTEST(classic))
    {
        return rb_float_new(perlin_octaves_3d(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z), p, n));
    }
    else
    {
        return rb_float_new(octave_noise_3d(n, p, 1.0, NUM2DBL(x), NUM2DBL(y), NUM2DBL(z)));
    }

}

/*
Returns a chunk of coordinates starting from x, y and of size steps_x, steps_y with interval.
*/
VALUE Perlin_Generator_chunk2d(VALUE self, VALUE x, VALUE y, VALUE steps_x, VALUE steps_y, VALUE interval)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));
    const int is_classic = RTEST(rb_iv_get(self, "@classic"));

    VALUE arr, row;
    int i, j;

    float x_min = NUM2DBL(x), y_min = NUM2DBL(y);
    float _x = x_min, _y = y_min;
    const int _steps_x = NUM2INT(steps_x), _steps_y = NUM2INT(steps_y);
    const float _interval = NUM2DBL(interval);

    seed = NUM2LONG(rb_iv_get(self, "@seed")); // Store in global, for speed.

    if(rb_block_given_p())
    {
        // Iterate through x, then y [0, 0], [1, 0], [2, 0]...
        _x = x_min;
        for (i = 0; i < _steps_x; i++)
        {
           _y = y_min;
           for (j = 0; j < _steps_y; j++)
           {
                if(is_classic)
                {
                    rb_yield_values(3, rb_float_new(perlin_octaves_2d(_x, _y, p, n)), rb_float_new(_x), rb_float_new(_y));
                }
                else
                {
                    rb_yield_values(3, rb_float_new(octave_noise_2d(n, p, 1.0, _x, _y)), rb_float_new(_x), rb_float_new(_y));
                }

                _y += _interval;
           }
           _x += _interval;
       }

       return Qnil;
    }
    else
    {
        // 2D array can be indexed with arr[x][y]
        arr = rb_ary_new();
        _x = x_min;
        for (i = 0; i < _steps_x; i++)
        {
            row = rb_ary_new();
            _y = y_min;
            for (j = 0; j < _steps_y; j++)
            {
                if(is_classic)
                {
                    rb_ary_push(row, rb_float_new(perlin_octaves_2d(_x, _y, p, n)));
                }
                else
                {
                    rb_ary_push(row, rb_float_new(octave_noise_2d(n, p, 1.0, _x, _y)));
                }

                _y += _interval;
            }
            rb_ary_push(arr, row);
            _x += _interval;
        }
        return arr;
    }
}

/*
Returns a chunk of coordinates starting from x, y, z and of size steps_x, steps_y, size_z with interval.
*/
VALUE Perlin_Generator_chunk3d(VALUE self, VALUE x, VALUE y, VALUE z, VALUE steps_x, VALUE steps_y, VALUE steps_z, VALUE interval)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));
    const int is_classic = RTEST(rb_iv_get(self, "@classic"));

    VALUE arr, row, column;
    int i, j, k;

    float x_min = NUM2DBL(x), y_min = NUM2DBL(y), z_min = NUM2DBL(z);
    float _x = x_min, _y = y_min, _z = z_min;
    const int _steps_x = NUM2INT(steps_x), _steps_y = NUM2INT(steps_y), _steps_z = NUM2INT(steps_z);
    const float _interval = NUM2DBL(interval);

    seed = NUM2LONG(rb_iv_get(self, "@seed")); // Store in global, for speed.

    if(rb_block_given_p())
    {
        _x = x_min;
        for (i = 0; i < _steps_x; i++)
        {
            _y = y_min;
            for (j = 0; j < _steps_y; j++)
            {
                _z = z_min;
                for (k = 0; k < _steps_z; k++)
                {
                    if(is_classic)
                    {
                        rb_yield_values(4, rb_float_new(perlin_octaves_3d(_x, _y, _z, p, n)), rb_float_new(_x), rb_float_new(_y), rb_float_new(_z));
                    }
                    else
                    {
                        rb_yield_values(4, rb_float_new(octave_noise_3d(n, p, 1.0, _x, _y, _z)), rb_float_new(_x), rb_float_new(_y), rb_float_new(_z));
                    }

                    _z += _interval;
                }
                _y += _interval;
            }
            _x += _interval;
        }
        return Qnil;
    }
    else
    {
        arr = rb_ary_new();
        _x = x_min;
        for (i = 0; i < _steps_x; i++)
        {
            row = rb_ary_new();
            _y = y_min;
            for (j = 0; j < _steps_y; j++)
            {
                column = rb_ary_new();
                _z = z_min;
                for (k = 0; k < _steps_z; k++)
                {
                    if(is_classic)
                    {
                        rb_ary_push(column, rb_float_new(perlin_octaves_3d(_x, _y, _z, p, n)));
                    }
                    else
                    {
                        rb_ary_push(column, rb_float_new(octave_noise_3d(n, p, 1.0, _x, _y, _z)));
                    }

                    _z += _interval;
                }
                rb_ary_push(row, column);
                _y += _interval;
            }
            rb_ary_push(arr, row);
            _x += _interval;
        }
        return arr;
    }
}

/*
The main initialize function which receives the inputs persistence and octave.
*/
VALUE Perlin_Generator_init(VALUE self, VALUE seed, VALUE persistence, VALUE octave, VALUE classic)
{
    Perlin_Generator_set_seed(self, seed);
    Perlin_Generator_set_persistence(self, persistence);
    Perlin_Generator_set_octave(self, octave);
    Perlin_Generator_set_classic(self, classic);

    return self;
}