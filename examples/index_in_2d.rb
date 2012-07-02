require 'perlin'

def print_data(seed, persistence, octave, size_x, size_y)
  noise = Perlin::Generator.new seed, persistence, octave

  puts

  # Note that it would be significantly faster to use #chunk in this example.
  size_y.times do |y|
    str = ''

    size_x.times do |x|
      n = noise.run2d x, y
      str << "%7.2f" % n
    end

    puts str
  end
end

print_data 5, 1.0, 1, 10, 500

puts "\nDone!"