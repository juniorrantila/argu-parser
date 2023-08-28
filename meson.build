project('argu-parser', 'cpp',
  version: '0.0.1',
  license: 'BSD 2-Clause',
  meson_version: '>=0.57.0',
  default_options: [
    'cpp_std=c++17',
    'cpp_eh=none',
    'cpp_rtti=false',
    'default_library=static',
    'warning_level=3',
    'buildtype=release'
  ])

should_install_pkg = not meson.is_subproject()
should_install_library = should_install_pkg or get_option('default_library') != 'static'

subdir('src')

if get_option('build-examples').enabled()
  subdir('examples')
endif
