require 'perlin'

def print_data(seed, p, o)
  p = Perlin.new(seed, p, o)
  data = ''
  for y in 1..8
    for x in  1..16
      n = p.run(x, y)
      if n >= 0.9
        data += '^'
      elsif n < 0.9 and n > 0.7
        data += '%'
      elsif n < 0.7 and n > 0.5
        data += '*'
      elsif n < 0.5 and n > 0.3
        data += '#'
      elsif n < 0.3 and n > 0.1
        data += '.'
      elsif n < 0.1 and n > -0.1
        data += ','
      else
        data += '~'
      end
    end
    data += "\n"
  end
  puts data
end

def create_html(seed, name, p, o)
  p = Perlin.new(seed, p, o)
  a = p.return_chunk(0, 0, 64, 64)
  data = "<table>\n"
  for r in a
    data += "<tr>\n"
    for z in r
      if z >= 0.9
        color = "brown"
        symbol = "^"
      elsif z < 0.9 and z > 0.7
        color = "orange"
        symbol = "%"
      elsif z < 0.7 and z > 0.5
        color = "red"
        symbol = "*"
      elsif z < 0.5 and z > 0.3
        color = "yellow"
        symbol = "#"
      elsif z < 0.3 and z > 0.1
        color = "lightgreen"
        symbol = "."
      elsif z < 0.1 and z > -0.1
        color = "green"
        symbol = ","
      elsif z < -0.1
        color = "blue"
        symbol = "~"
      end
      data += "<td style=\"background-color: %s; width: 10px; height: 10px;\""">%s</td>\n" % [color, symbol]
    end
    data += "</tr>\n"
  end
  data += "</table>\n"
  f = File.open("#{name}.html", "w")
  f.write(data)
  f.close
end

print_data(321, 1, 1)
create_html(321, "perlin", 1, 8)