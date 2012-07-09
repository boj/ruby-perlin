require 'perlin/version'

begin
  RUBY_VERSION =~ /(\d+.\d+)/
  require "perlin/#{$1}/perlin.so"
rescue LoadError
  require "perlin/perlin.so"
end

require 'perlin/generator'

module Perlin
end