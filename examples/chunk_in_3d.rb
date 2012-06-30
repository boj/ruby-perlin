require 'perlin'

def print_data(seed, persistence, octave, size_x, size_y, size_z)
  noise = Perlin::Noise.new seed, persistence, octave

  puts

  data = noise.chunk 0, 0, 0, size_x, size_y, size_z
  size_z.times do |z|
    size_y.times do |y|
      str = ''

      size_x.times do |x|
        str << "%7.2f" % data[x][y]
      end

      puts str
    end
  end
end

print_data 5, 1.0, 1, 10, 2000, 99

puts "\nDone!"