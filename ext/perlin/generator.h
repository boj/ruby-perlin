/*
 * class Perlin::Generator
 */

 #ifndef GENERATOR_H
#define GENERATOR_H

#include <ruby.h>

#include "classic.h"
#include "simplex.h"

extern long seed;

VALUE Perlin_Generator_set_seed(const VALUE self, const VALUE seed);
VALUE Perlin_Generator_set_persistence(const VALUE self, const VALUE persistence);
VALUE Perlin_Generator_set_octave(const VALUE self, const VALUE octave);
VALUE Perlin_Generator_set_classic(const VALUE self, const VALUE classic);

VALUE Perlin_Generator_run(const int argc, const VALUE *argv, const VALUE self);
VALUE Perlin_Generator_run2d(const VALUE self, const VALUE x, const VALUE y);
VALUE Perlin_Generator_run3d(const VALUE self, const VALUE x, const VALUE y, const VALUE z);

VALUE Perlin_Generator_chunk(const int argc, const VALUE *argv, const VALUE self);
VALUE Perlin_Generator_chunk2d(const VALUE self, const VALUE x, const VALUE y, const VALUE steps_x, const VALUE steps_y, VALUE interval);
VALUE Perlin_Generator_chunk3d(const VALUE self, const VALUE x, const VALUE y, const VALUE z, const VALUE steps_x, const VALUE steps_y, const VALUE steps_z, const VALUE interval);

VALUE Perlin_Generator_init(const VALUE self, const VALUE seed, const VALUE persistence, const VALUE octave, const VALUE classic);

#endif // GENERATOR_H