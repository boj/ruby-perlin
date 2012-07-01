/*
Ruby module that is built according to the Perlin Noise function
located at http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
*/

#include "perlin.h"

void Init_perlin() {
    VALUE jm_Module = rb_define_module("Perlin");
    VALUE rb_cPerlin = rb_define_class_under(jm_Module, "Generator", rb_cObject);

    rb_define_method(rb_cPerlin, "initialize", Perlin_Generator_init, 3);

    rb_define_method(rb_cPerlin, "seed=", Perlin_Generator_set_seed, 1);
    rb_define_method(rb_cPerlin, "persistence=", Perlin_Generator_set_persistence, 1);
    rb_define_method(rb_cPerlin, "octave=", Perlin_Generator_set_octave, 1);

    rb_define_method(rb_cPerlin, "run2d", Perlin_Generator_run2d, 2);
    rb_define_method(rb_cPerlin, "run3d", Perlin_Generator_run3d, 3);
    rb_define_method(rb_cPerlin, "chunk2d", Perlin_Generator_chunk2d, 4);
    rb_define_method(rb_cPerlin, "chunk3d", Perlin_Generator_chunk3d, 6);
}

