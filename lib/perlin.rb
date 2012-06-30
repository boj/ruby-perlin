$LOAD_PATH.unshift File.expand_path("../perlin", __FILE__)
require 'version'

require 'perlin.so'

module Perlin
# Perlin noise generator.
#
# @!attribute [r] persistence
#   @return [Float]
#
# @!attribute [r] octave
#   @return [Integer]
class Noise
  attr_reader :octave, :persistence

  # @overload chunk(x, y, size_x, size_y)
  #   Calculates a rectangular section of height (n) values and returns them as a 2D array.
  #
  #   This is much faster than accessing each point separately using {#[]}
  #
  #   @example
  #     noise = Perlin::Noise.new 123, 1.0, 1
  #     arr = noise.chunk 1, 1, 2, 3
  #
  #     # access position 1, 2 (remember that arr is offset by the x, y value of the chunk)
  #     puts arr[0, 1] #=> -0.2208995521068573
  #
  #     p arr  #= > [[0.05753844603896141, -0.2208995521068573, 0.3973901569843292], [0.1383310854434967, -0.22248442471027374, 0.15600799024105072]]
  #
  #   @param x [Integer]
  #   @param y [Integer]
  #   @param width [Integer]
  #   @param height [Integer]
  #   @return [Array<Array<Float>>] height (n) values within the rectangle.
  def chunk(*args)
    case args.size
      when 6
        raise ImplementationError, "3d noise is not supported in chunks yet"
      when 4
        raise ArgumentError, "Can't have negative size_x or size_y" if args[2] < 0 or args[3] < 0
        chunk2d *args
      else
        raise ArgumentError, "#{args.size} dimensional noise generation is not supported for chunks. 2D only, using (x, y, width, height)"
    end
  end

  # Gets height (n) at a point in 2D or 3D space.
  #
  # This is much slower, if accessing many points, than using {#chunk}
  #
  # @overload [](x, y)
  #   Gets height (n) value at a specific 2D position.
  #
  #   @example
  #     noise = Perlin::Noise.new 123, 1.0, 1
  #
  #     # Returns a 'height' value for (x, y)
  #     puts noise[10, 20]  #=> 0.9004574418067932
  #
  #   @param x [Integer]
  #   @param y [Integer]
  #   @return [Float] height (n) value at the position
  #
  # @overload [](x, y, z)
  #   Gets height (n) value at a specific 3D position.
  #
  #   @example
  #     noise = Perlin::Noise.new 123, 1.0, 1
  #
  #     # Returns a 'height' value for (x, y, z)
  #     puts noise[10, 20, 30]  #=> 0.017745036631822586
  #
  #   @param x [Integer]
  #   @param y [Integer]
  #   @param z [Integer]
  #   @return [Float]  height (n) value at the position
  def [](*args)
    case args.size
      when 3
        run3d *args
      when 2
        run2d *args
      else
        raise ArgumentError, "#{args.size} dimensional noise generation is not supported (2D and 3D are)"
    end
  end

  alias_method :run, :[]
end
end