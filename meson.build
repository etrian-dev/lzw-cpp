# Meson build file
project('lzw', 'cpp', license: 'MIT License')
# list all source files
all_sources = files(
  'main.cpp', 'encoder.cpp', 'decoder.cpp', 'init-dict.cpp', 'loguru.cpp')
inc = include_directories('.')
# Finally specifies the name of the executable to be produced,
# all the source files needed to build it
# and the dependencies to be satisfied
executable(
  'lzw', 
  all_sources,
  include_directories: inc,
  cpp_args: '-DLOGURU_WITH_STREAMS=1')
