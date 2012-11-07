## Note - November 7th, 2012

This code has been extended, packaged as a gem, and maintained by Bil Bas [here](https://github.com/Spooner/ruby-perlin).

# Ruby Perlin

A Ruby Perlin implementation based off
<http://freespace.virgin.net/hugo.elias/models/m_perlin.htm>

- Written by Brian 'bojo' Jones <mojobojo@gmail.com>
- Optimizations thanks to Camille Goudeseune, Univ of Illinois, <cog@illinois.edu>

## INSTALL

		$ ruby extconf.rb
		$ make

Copy the module to wherever you wish to use it.

## USAGE

``` ruby
require 'perlin'

# Takes values Seed, Persistence, and Octaves
p = Perlin.new(123, 1, 1)
# Returns a z value for (x, y)
puts p.run(10, 20) 
# Returns an array of z's for range 
# (x_start, y_start, x_end, y_end)
arr = p.return_chunk(0, 0, 16, 16)
```
