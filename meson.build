project('argument-parser', 'cpp',
  version: '0.0.1',
  license: 'BSD 2-Clause',
  meson_version: '>=0.50.0',
  default_options: [
    'cpp_std=c++17',
    'cpp_eh=none',
    'cpp_rtti=false',
    'default_library=static',
    'warning_level=3',
    'buildtype=release'
  ])

subdir('src')

if get_option('build-examples').enabled()
  subdir('examples')
endif
