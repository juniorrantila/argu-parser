install_headers('parser.h', subdir: 'argu')

argu_parser_lib = library('argu-parser', [
  'parser.cpp'
], install: true)

argu_parser_dep = declare_dependency(
  link_with: argu_parser_lib,
  include_directories: '..'
)
