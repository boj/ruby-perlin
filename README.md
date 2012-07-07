Perlin
======

A fast Perlin/Simplex noise generator.
Implemented as a Ruby C extension, it is considerably faster than the pure Ruby equivalent, [perlin_noise](https://github.com/junegunn/perlin_noise).

By default, the noise generated uses Perlin's Simplex functions, but can produce "Classic" Perlin noise
(not recommended, since it is significantly slower).

If a large number of noise values are required, requesting chunks of values can be faster, which can then be iterated
through at leisure (or use the block version of #chunk).

- [Benchmarks](https://github.com/Spooner/ruby-perlin/blob/master/bench/benchmarks.txt)

CREDITS
-------

- Written by Brian 'bojo' Jones (mojobojo@gmail.com)
- Optimizations thanks to Camille Goudeseune, Univ of Illinois, (cog@illinois.edu)
- Made into gem and extended by Bil Bas (bil.bagpuss@gmail.com)

- [Simplex noise functions copyright (c) 2007-2012 Eliot Eshelman](http://www.6by9.net/b/2012/02/03/simplex-noise-for-c-and-python)
- "Classic" noise functions based on http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

USE CASES
---------

One use for this gem would be to generate "randomness" for games, such as building levels for a rogue-alike game.

If generating textures, it is advised to use a Simplex noise shader instead of this gem, since shaders are considerably
faster than setting pixels, for example by using TexPlay. The issue is not so much that generating the noise is faster,
but that setting individual pixel colours from Ruby is very slow. However, if graphics shaders are not available, then
this could be used, but would be painfully slow for anything but tiny images.

INSTALL
-------

    $ gem install perlin --pre

USAGE
-----

    require 'perlin'

    # Takes values seed, persistence, and octaves
    gen = Perlin::Generator.new 123, 1.0, 1

    # Returns a 'height' value for 2 or 3-dimensional noise.
    gen[10, 20]        #=> 0.0011919947855197324
    gen[5, 22.2, 99.8] #=> 0.692631933498385

    # Returns a 2D array of 'heights' for a range (x, y, size_x, size_y, interval)
    noise = gen.chunk 1, 1, 2, 3, 1

    noise[1][2] #=> 0.28406014362476045
    p noise     #= > [[-0.9227624778765608, -0.0016387049134645594, -0.4764434188330739],
                #     [0.24490384926510073, -0.4764434188330739, 0.28406014362476045]]


    # Returns a 3D array of 'heights' for a range (x, y, size_x, size_y, interval)
    noise = gen.chunk 0, 0, 0, 2, 3, 4, 0.5
    noise.size       #=> 2
    noise[0].size    #=> 3
    noise[0][0].size #=> 4
    noise[1][2][3]   #=>  -0.1585322813268602

    # Can run in "Classic" Perlin mode (Not recommended - SLOW!)

    gen.classic = true
    gen[10, 20]        #=> -0.19936732947826385
