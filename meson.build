# SPDX Licence-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024-2025 Laurent Fazio <laurent.fazio@gmail.com>

project(
  'libmm',
  ['c', 'cpp'],
  license: ['Apache-2.0'],
  license_files: ['LICENCE'],
  default_options: ['c_std=gnu2x', 'cpp_std=c++17', 'default_library=static' ],
  version: '1.0',
)

if get_option('buildtype').startswith('debug')
  add_global_arguments('-DDEBUG', language : ['c', 'cpp'])
endif

subdir('src')

inc = include_directories('include', 'include/freebsd/sys')

install_subdir('include', install_dir: 'include')

thread_dep = dependency('threads')

libmm = library(
  'libmm',
  libmm_src,
  include_directories: inc,
  version: '1.0.0',
  soversion: '1',
  install: true,
)

libmm_dep = declare_dependency(include_directories: inc, link_with: libmm, dependencies: thread_dep)

gtest_dep = dependency('gtest', required: true, fallback: [ 'gtest', 'gtest_dep'])

subdir('test')

doxygen = find_program('doxygen', required : false)
if not doxygen.found()
  error('MESON_SKIP_TEST doxygen not found.')
else
cdata = configuration_data()
cdata.set('VERSION', meson.project_version())

if find_program('dot', required : false).found()
  # In the real world this would set the variable
  # to YES. However we set it to NO so that the
  # list of generated files is always the same
  # so tests always pass.
  cdata.set('HAVE_DOT', 'NO')
else
  cdata.set('HAVE_DOT', 'NO')
endif

subdir('doc')
endif
