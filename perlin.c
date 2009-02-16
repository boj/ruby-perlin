/*
Ruby module that is built according to the Perlin Noise function
located at http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
*/

#include "ruby.h"
#include "math.h"

VALUE rb_cPerlin;

static long seed = 0;

typedef struct perlin_data {
	VALUE seed;
  VALUE persistence;
  VALUE octave;
} perlin_t;

void perlin_mark(perlin_t* self)
{
  rb_gc_mark(self->persistence);
  rb_gc_mark(self->octave);
}

void perlin_free(perlin_t* self)
{
  free(self);
}

VALUE perlin_allocate(VALUE klass)
{
  perlin_t *p = malloc(sizeof(perlin_t));
  p->persistence = Qnil;
  p->octave = Qnil;
  return Data_Wrap_Struct(klass, perlin_mark, perlin_free, p);
}

/*
The main initialize function which recieves the inputs persistence and octave.
*/
VALUE Perlin_init(VALUE self, VALUE seed_value, VALUE persistence, VALUE octave)
{
  perlin_t* perlin;
  Data_Get_Struct(self, perlin_t, perlin);
  perlin->persistence = persistence;
  perlin->octave = octave;

	seed = seed_value;
  
  return self;
}

float perlin_interpolate(float a, float b, float x)
{
  float ft = x * 3.1415927;
  float f = (1 - cos(ft)) * 0.5;
  return  a * (1 - f) + b * f;
}

float perlin_noise(int x, int y)
{
  long n = x + y * 57;
  n = (n << 13) ^ n;
  return (1.0 - ((n * (n * n * 15731*seed + 789221*seed) + 1376312589*seed) & 0x7fffffff) / 1073741824.0);
}

float perlin_smooth_noise(int x, int y)
{
  float corners = (perlin_noise(x - 1, y - 1) + perlin_noise(x + 1, y - 1) + perlin_noise(x - 1, y + 1) + perlin_noise(x + 1, y + 1) ) / 16;
  float sides   = (perlin_noise(x - 1, y) + perlin_noise(x + 1, y) + perlin_noise(x, y - 1) + perlin_noise(x, y + 1) ) /  8;
  float center  =  perlin_noise(x, y) / 4;
  return corners + sides + center;
}

float perlin_interpolated_noise(float x, float y)
{
  int integer_X    = (int)x;
  float fractional_X = x - integer_X;

  int integer_Y    = (int)y;
  float fractional_Y = y - integer_Y;

  float v1 = perlin_smooth_noise(integer_X,     integer_Y);
  float v2 = perlin_smooth_noise(integer_X + 1, integer_Y);
  float v3 = perlin_smooth_noise(integer_X,     integer_Y + 1);
  float v4 = perlin_smooth_noise(integer_X + 1, integer_Y + 1);

  float i1 = perlin_interpolate(v1, v2, fractional_X);
  float i2 = perlin_interpolate(v3, v4, fractional_X);
  
  return perlin_interpolate(i1, i2, fractional_Y);
}

/*
Takes points (x, y) and returns a height (z)
*/
VALUE perlin_run(VALUE self, VALUE x, VALUE y)
{
  float total = 0;
  perlin_t* perlin;
  Data_Get_Struct(self, perlin_t, perlin);
  int p = NUM2INT(perlin->persistence);
  int n = NUM2INT(perlin->octave);
  
  int i;
  float frequency, amplitude;
  for (i = 0; i < n; i++)
  {
    frequency = pow(2, i);
    amplitude = pow(p, i);
    total = total + perlin_interpolated_noise(NUM2INT(x) * frequency, NUM2INT(y) * frequency) * amplitude;
  }
  
  return rb_float_new(total);
}


/*
Returns a chunk of coordinates start from start_x to size_x, and start_y to size_y.
*/
VALUE perlin_return_chunk(VALUE self, VALUE start_x, VALUE start_y, VALUE size_x, VALUE size_y)
{
  VALUE arr = rb_ary_new();
  
  int i, j;
  for (i = NUM2INT(start_x); i < NUM2INT(size_x) + NUM2INT(start_x); i++)
  {
    VALUE row = rb_ary_new();
    for (j = NUM2INT(start_y); j < NUM2INT(size_y) + NUM2INT(start_y); j++)
    {
      VALUE val = perlin_run(self, INT2NUM(i), INT2NUM(j));
      rb_ary_push(row, val);
    }
    rb_ary_push(arr, row);
  }
  
  return arr;
}

void Init_perlin() {
  
  rb_cPerlin = rb_define_class("Perlin", rb_cObject);
  
  rb_define_alloc_func(rb_cPerlin, perlin_allocate);
  
  rb_define_method(rb_cPerlin, "initialize", Perlin_init, 3);
  rb_define_method(rb_cPerlin, "run", perlin_run, 2);
  rb_define_method(rb_cPerlin, "return_chunk", perlin_return_chunk, 4);
  
}

