require 'perlin'

def print_data(seed, p, o, h, w, d)
  p = Perlin.new(seed, p, o)
  data = ''
  for z in 1..d
    for x in 1..w
      for y in 1..h
        n = p.run3d(z, x, y)
        s = "%.2f" % n
        for i in 1..(5 - s.size)
          s += " "
        end
        data += s
      end
      data += "\n"
    end
    puts data
    sleep(0.1)
  end
end

print_data(5, 1.0, 1, 10, 66, 100)
