Ruby Perlin
===========

A Perlin noise implementation based off
<http://freespace.virgin.net/hugo.elias/models/m_perlin.htm>. Implemented as a Ruby C extension, it is considerably faster than the pure Ruby equivalent, [perlin_noise](https://github.com/junegunn/perlin_noise).

- Written by Brian 'bojo' Jones <mojobojo@gmail.com>
- Optimizations thanks to Camille Goudeseune, Univ of Illinois, <cog@illinois.edu>
- Made into gem by Bil Bas (bil.bagpuss@gmail.com)

INSTALL
-----

    $ gem install ruby-perlin

USAGE
-----

    require 'perlin'

    # Takes values seed, persistence, and octaves
    noise = Perlin::Noise.new 123, 1, 1

    # Returns a 'height' value for (x, y)
    puts noise[10, 20]  #=> 0.9004574418067932

    # Returns a 2D array of 'heights' for a range (x, y, width, height)
    arr = noise.chunk 1, 1, 2, 3

    p arr  #= > [[0.05753844603896141, -0.2208995521068573, 0.3973901569843292],
           #    [0.1383310854434967, -0.22248442471027374, 0.15600799024105072]]
