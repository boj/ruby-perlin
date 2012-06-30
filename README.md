# Ruby Perlin

A Ruby Perlin implementation based off
<http://freespace.virgin.net/hugo.elias/models/m_perlin.htm>

- Written by Brian 'bojo' Jones <mojobojo@gmail.com>
- Optimizations thanks to Camille Goudeseune, Univ of Illinois, <cog@illinois.edu>

## INSTALL

    $ gem install ruby-perlin

Copy the module to wherever you wish to use it.

## USAGE

    require 'perlin'

    # Takes values Seed, Persistence, and Octaves
    p = Perlin.new(123, 1, 1)

    # Returns a z value for (x, y)
    puts p.run(10, 20)

    # Returns a flat array of z's for range
    # (x_start, y_start, x_end, y_end)
    arr = p.return_chunk(0, 0, 16, 16)
