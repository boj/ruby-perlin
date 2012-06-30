begin
  require 'devkit' # only used on windows
rescue LoadError
end

require File.expand_path('../lib/perlin/version', __FILE__)

require 'rake/extensiontask'

require 'yard'
require 'redcloth'

# somewhere in your Rakefile, define your gem spec
spec = Gem::Specification.new do |s|
  s.name = 'ruby-perlin'
  s.version = Perlin::VERSION
  s.date = Time.now.strftime '%Y-%m-%d'
  s.authors = ["Brian 'bojo' Jones", 'Camille Goudeseune', 'Bil Bas']

  s.summary = 'Perlin Noise C extension'
  s.description = <<-END
#{s.summary}

A Perlin noise implementation based of
<http://freespace.virgin.net/hugo.elias/models/m_perlin.htm>. Implemented as a Ruby C extension.

Written by Brian 'bojo' Jones (mojobojo@gmail.com)
Optimizations thanks to Camille Goudeseune, Univ of Illinois, (cog@illinois.edu)
Made into gem by Bil Bas (bil.bagpuss@gmail.com)
  END

  s.email = %w<mojobojo@gmail.com>
  s.files = Dir.glob %w<CHANGELOG LICENSE Rakefile README.md lib/**/*.* ext/**/*.c ext/**/*.rb examples/**/*.*>
  s.homepage = 'https://github.com/boj/ruby-perlin'
  s.licenses = %w<MIT>
  s.extensions << 'ext/perlin/extconf.rb'
  s.rubyforge_project = 'ruby-perlin'
  s.test_files = []
  s.has_rdoc = 'yard'

  s.add_development_dependency 'rake-compile', '~> 0.8.1'
end

Gem::PackageTask.new spec do
end

Rake::ExtensionTask.new 'perlin', spec do |ext|
  ext.lib_dir = 'lib/perlin'
end

YARD::Rake::YardocTask.new

desc "Create platform-specific compiled gem"
task :native_gem do
  system "rake native gem"
end