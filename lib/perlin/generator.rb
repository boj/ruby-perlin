module Perlin
  # Perlin noise generator.
  #
  # @!attribute [rw] seed
  #   @return [Integer]
  #
  # @!attribute [r] persistence
  #   @return [Float]
  #
  # @!attribute [r] octave
  #   @return [Integer]
  class Generator
    attr_reader :seed, :octave, :persistence
    def classic?; @classic; end

    # @!method initialize(seed, persistence, octave)
    #   Create a noise generator.
    #
    #   Using the same seed will always produce the same pattern. Animate a perlin 'texture' by altering the seed based on time.
    #
    #   @param seed [Integer] Seed value to create a different pattern.
    #   @param persistence [Float] Used to generate different frequencies/amplitudes of output.
    #   @param octave [Integer] Number of iterations to run (higher number of octaves takes more time)
    #   @option options :classic [Boolean] (false) Whether to use the slower Classic algorithm, rather than default (and much faster) Simplex.
    def initialize(seed, persistence, octave, options = {})
      options = {
          :classic => false,
      }.merge! options

      initialize_(seed, persistence, octave, options[:classic])
    end
    protected :initialize_ # Underlying C implementation.

    # @overload chunk(x, y, steps_x, steps_y, interval)
    #   Calculates a rectangular section of height (n) values and returns them as a 2D array.
    #
    #   This is much faster than accessing each point separately using {#[]}
    #
    #   @example
    #     noise = Perlin::Generator.new 123, 1.0, 1
    #     arr = noise.chunk 1, 1, 2, 3, 1.5
    #
    #     # access position 1, 2 (remember that arr is offset by the x, y value of the chunk)
    #     puts arr[0, 1] #=> -0.2208995521068573
    #
    #     p arr  #= > [[0.05753844603896141, -0.2208995521068573, 0.3973901569843292], [0.1383310854434967, -0.22248442471027374, 0.15600799024105072]]
    #
    #   @param x [Float]
    #   @param y [Float]
    #   @param steps_x [Integer]
    #   @param steps_y [Integer]
    #   @param interval [Float]
    #
    #   @return [Array<Array<Float>>] height (n) values within the rectangle.
    #
    # @overload chunk(x, y, steps_x, steps_y, interval) {|h, x, y| }
    #   Calculates a rectangular section of height (n) values and returns them as a 2D array.
    #
    #   This is much faster than accessing each point separately using {#[]}
    #
    #   @example
    #     noise = Perlin::Generator.new 123, 0.5, 3
    #     noise.chunk 1.0, 2.3, 3, 2, 1.5 do |h, x, y|
    #       # Use the height value, which is at x, y.
    #     end
    #
    #   @param x [Float]
    #   @param y [Float]
    #   @param steps_x [Integer]
    #   @param steps_y [Integer]
    #   @param interval [Float]
    #
    #   @yieldparam h [Float] Height at x, y
    #   @yieldparam x [Float]
    #   @yieldparam y [Float]
    #
    #   @return [nil]
    #
    # @overload chunk(x, y, z, size_x, size_y, size_z, interval)
    #   Calculates a rectangular section of height (n) values and returns them as a 3D array.
    #
    #   This is much faster than accessing each point separately using {#[]}
    #
    #   @example
    #     noise = Perlin::Generator.new 123, 0.5, 5
    #     arr = noise.chunk 6.0, 5.0, 4.0, 3, 2, 1, 1.5
    #
    #     # access position 2, 1, 0 (remember that arr is offset by the x, y and z value of the chunk)
    #     puts arr[2, 1, 0] #=>
    #
    #     p arr  #= >
    #
    #   @param x [Float]
    #   @param y [Float]
    #   @param z [Float]
    #   @param steps_x [Integer]
    #   @param steps_y [Integer]
    #   @param steps_z [Integer]
    #   @param interval [Float]
    #
    #   @return [Array<Array<Float>>] height (n) values within the rectangle.
    #
    # @overload chunk(x, y, z, size_x, size_y, size_z, interval) {|h, x, y| }
    #   Calculates a rectangular section of height (n) values and returns them as a 3D array.
    #
    #   This is much faster than accessing each point separately using {#[]}
    #
    #   @example
    #     noise = Perlin::Generator.new 123, 0.8, 3
    #     noise.chunk 6.0, 5.0, 4.0, 3, 2, 1, 1.5 do |h, x, y, z|
    #       # Use the height value, which is at x, y, z.
    #     end
    #
    #   @param x [Float]
    #   @param y [Float]
    #   @param z [Float]
    #   @param steps_x [Integer]
    #   @param steps_y [Integer]
    #   @param steps_z [Integer]
    #   @param interval [Float]
    #
    #   @yieldparam h [Float] Height at x, y, z
    #   @yieldparam x [Float]
    #   @yieldparam y [Float]
    #   @yieldparam z [Float]
    #
    #   @return [nil]
    def chunk(*args, &block)
      case args.size
        when 7
          raise ArgumentError, "steps must be >= 1" if args[3] < 1 || args[4] < 1 || args[5] < 1
          chunk3d *args, &block
        when 5
          raise ArgumentError, "steps must be >= 1" if args[2] < 1 || args[3] < 1
          chunk2d *args, &block
        else
          raise ArgumentError, "#{args.size} parameters not supported for chunks. Requires 5 parameters for 2D or 7 for 3D"
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
    #     noise = Perlin::Generator.new 123, 1.0, 1
    #
    #     # Returns a 'height' value for (x, y)
    #     puts noise[10, 20]  #=> 0.9004574418067932
    #
    #   @param x [Float]
    #   @param y [Float]
    #   @return [Float] height (n) value at the position
    #
    # @overload [](x, y, z)
    #   Gets height (n) value at a specific 3D position.
    #
    #   @example
    #     noise = Perlin::Generator.new 123, 1.0, 1
    #
    #     # Returns a 'height' value for (x, y, z)
    #     puts noise[10, 20, 30]  #=> 0.017745036631822586
    #
    #   @param x [Float]
    #   @param y [Float]
    #   @param z [Float]
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