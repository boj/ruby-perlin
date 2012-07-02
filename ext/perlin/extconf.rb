require 'mkmf'

RUBY_VERSION =~ /(\d+.\d+)/
extension_name = "perlin/#{$1}/perlin"

dir_config(extension_name)

# 1.9 compatibility
$CFLAGS += ' -DRUBY_19' if RUBY_VERSION =~ /^1.9/

# let's use c99
#$CFLAGS += " -std=c99"
$CFLAGS += " -std=gnu99"

create_makefile(extension_name)