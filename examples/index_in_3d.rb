require 'perlin'

def print_data(seed, persistence, octave, size_x, size_y, size_z)
  noise = Perlin::Noise.new seed, persistence, octave

  puts

  size_z.times do |z|
    size_y.times do |y|
      str = ''

      size_x.times do |x|
        n = noise.run3d x, y, z
        str << "%7.2f" % n
      end

      puts str
    end
  end
end

print_data 5, 1.0, 1, 10, 200, 99

puts "\nDone!"