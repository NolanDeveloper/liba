project(
  'liba',
  'c',
  version: '0.1',
  default_options: [
    'warning_level=3',
    'werror=true',
    'c_std=c11'
  ])

include_directories = include_directories('include')

install_headers('include/liba.h')

munit_dep = dependency('munit', fallback: ['munit', 'munit_dep'])

src = [
  'src/liba.c',
]

liba = library(
  'a',
  src,
  install: true,
  include_directories: include_directories,
)

liba_test_unit = executable(
  'liba_test_unit',
  ['test-unit/main.c'],
  link_with: liba,
  dependencies: [munit_dep],
  include_directories: include_directories
)

test('unit test', liba_test_unit)

pkg = import('pkgconfig')
pkg.generate(liba)

liba_dep = declare_dependency(
  include_directories: include_directories,
  link_with: liba
)

