require 'perlin'

def print_data(seed, persistence, octave, size_x, size_y, size_z, interval)
  noise = Perlin::Generator.new seed, persistence, octave

  puts

  data = noise.chunk 0, 0, 0, size_x, size_y, size_z, interval
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

print_data 5, 1.0, 1, 10, 500, 99, 1.9

puts "\nDone!"