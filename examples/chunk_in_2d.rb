require 'perlin'

def print_data(seed, persistence, octave, size_x, size_y, interval)
  noise = Perlin::Generator.new seed, persistence, octave

  puts

  data = noise.chunk 0, 0, size_x, size_y, interval
  size_y.times do |y|
    str = ''

    size_x.times do |x|
      str << "%7.2f" % data[x][y]
    end

    puts str
  end
end

print_data 5, 1.0, 1, 10, 500, 1

puts "\nDone!"