argu_parser_lib = library('argu-parser', [
  'parser.cpp'
], install: should_install_library)

argu_parser_dep = declare_dependency(
  link_with: argu_parser_lib,
  include_directories: '..'
)

if should_install_pkg
  install_headers('parser.h', subdir: 'argu')
  pkg = import('pkgconfig')
  pkg.generate(
     libraries: argu_parser_lib,
     subdirs: '.',
     version: meson.project_version(),
     name: 'lib' + meson.project_name(),
     filebase: meson.project_name(),
     description: 'A simple C++17 CLI argument parser',
   )
endif

