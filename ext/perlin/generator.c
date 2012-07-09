#include "generator.h"

VALUE Perlin_Generator_set_seed(const VALUE self, const VALUE seed)
{
    rb_iv_set(self, "@seed", rb_funcall(seed, rb_intern("to_i"), 0));
}

VALUE Perlin_Generator_set_persistence(const VALUE self, const VALUE persistence)
{
   rb_iv_set(self, "@persistence", rb_funcall(persistence, rb_intern("to_f"), 0));
}

VALUE Perlin_Generator_set_octave(const VALUE self, const VALUE octave)
{
    rb_iv_set(self, "@octave", rb_funcall(octave, rb_intern("to_i"), 0));
}

VALUE Perlin_Generator_set_classic(const VALUE self, const VALUE classic)
{
    rb_iv_set(self, "@classic", classic);
}

// x, y
// x, y, z
VALUE Perlin_Generator_run(const int argc, const VALUE *argv, const VALUE self)
{
    VALUE x, y, z;

    rb_scan_args(argc, argv, "21", &x, &y, &z);

    switch(argc)
    {
        case 2:
            Perlin_Generator_run2d(self, x, y);
            break;

        case 3:
            Perlin_Generator_run3d(self, x, y, z);
            break;

        default:
            rb_raise(rb_eArgError, "%d parameters not supported (2D and 3D are)", argc);
    }
}

/*
Takes points (x, y) and returns a height (n)
*/
VALUE Perlin_Generator_run2d(const VALUE self, const VALUE x, const VALUE y)
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
        return rb_float_new(octave_noise_3d(n, p, 1.0, NUM2DBL(x), NUM2DBL(y), seed * SEED_OFFSET));
    }
}

/*
Takes points (x, y, z) and returns a height (n)
*/
VALUE Perlin_Generator_run3d(const VALUE self, const VALUE x, const VALUE y, const VALUE z)
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
        return rb_float_new(octave_noise_4d(n, p, 1.0, NUM2DBL(x), NUM2DBL(y), NUM2DBL(z), seed * SEED_OFFSET));
    }

}

// x, y steps_x, steps_y, interval
// x, y, z, steps_x, steps_y, steps_z, interval
VALUE Perlin_Generator_chunk(const int argc, const VALUE *argv, const VALUE self)
{
    VALUE a, b, c, d, e, f, g;

    rb_scan_args(argc, argv, "52", &a, &b, &c, &d, &e, &f, &g);

    switch(argc)
    {
        case 5:
            Perlin_Generator_chunk2d(self, a, b, c, d, e);
            break;

        case 7:
            Perlin_Generator_chunk3d(self, a, b, c, d, e, f, g);
            break;

        default:
            rb_raise(rb_eArgError, "%d parameters not supported (5 for 2D and 7 for 3D are)", argc);
    }
}

/*
Returns a chunk of coordinates starting from x, y and of size steps_x, steps_y with interval.
*/
VALUE Perlin_Generator_chunk2d(const VALUE self, const VALUE x, const VALUE y, const VALUE steps_x, const VALUE steps_y, const VALUE interval)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));
    const int is_classic = RTEST(rb_iv_get(self, "@classic"));

    VALUE arr, row;
    int i, j;

    const float x_min = NUM2DBL(x), y_min = NUM2DBL(y);
    float _x, _y;
    const int _steps_x = NUM2INT(steps_x), _steps_y = NUM2INT(steps_y);
    const float _interval = NUM2DBL(interval);

    if(_steps_x < 1 || _steps_y < 1)
    {
        rb_raise(rb_eArgError, "steps must be >= 1");
    }

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
                    rb_yield_values(3, rb_float_new(perlin_octaves_2d(_x, _y, p, n)),
                                    rb_float_new(_x), rb_float_new(_y));
                }
                else
                {
                    rb_yield_values(3, rb_float_new(octave_noise_3d(n, p, 1.0, _x, _y, seed * SEED_OFFSET)),
                                    rb_float_new(_x), rb_float_new(_y));
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
                    rb_ary_push(row, rb_float_new(octave_noise_3d(n, p, 1.0, _x, _y, seed * SEED_OFFSET)));
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
VALUE Perlin_Generator_chunk3d(const VALUE self, const VALUE x, const VALUE y, const VALUE z, const VALUE steps_x, const VALUE steps_y, const VALUE steps_z, const VALUE interval)
{
    const float p = RFLOAT_VALUE(rb_iv_get(self, "@persistence"));
    const int n = NUM2INT(rb_iv_get(self, "@octave"));
    const int is_classic = RTEST(rb_iv_get(self, "@classic"));

    VALUE arr, row, column;
    int i, j, k;

    const float x_min = NUM2DBL(x), y_min = NUM2DBL(y), z_min = NUM2DBL(z);
    float _x, _y, _z;
    const int _steps_x = NUM2INT(steps_x), _steps_y = NUM2INT(steps_y), _steps_z = NUM2INT(steps_z);
    const float _interval = NUM2DBL(interval);

    if(_steps_x < 1 || _steps_y < 1 || _steps_z < 1)
    {
        rb_raise(rb_eArgError, "steps must be >= 1");
    }

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
                        rb_yield_values(4, rb_float_new(perlin_octaves_3d(_x, _y, _z, p, n)),
                                        rb_float_new(_x), rb_float_new(_y), rb_float_new(_z));
                    }
                    else
                    {
                        rb_yield_values(4, rb_float_new(octave_noise_4d(n, p, 1.0, _x, _y, _z, seed * SEED_OFFSET)),
                                        rb_float_new(_x), rb_float_new(_y), rb_float_new(_z));
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
                        rb_ary_push(column, rb_float_new(octave_noise_4d(n, p, 1.0, _x, _y, _z, seed * SEED_OFFSET)));
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