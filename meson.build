project('liba', 'c',
  version: '0.1',
  default_options: ['warning_level=3', 'werror=true', 'c_std=c11'])

include_directories = include_directories('include')

install_headers('include/liba.h')

src = [
  'src/liba.c',
]

liba = library(
  'a',
  src,
  install: true,
  include_directories: include_directories,
)

pkg = import('pkgconfig')
pkg.generate(liba)

liba_dep = declare_dependency(
  include_directories: include_directories,
  link_with: liba
)

