# SPDX Licence-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

project(
  'libmm',
  ['c', 'cpp'],
  default_options: ['c_std=gnu23', 'cpp_std=c++17', 'default_library=static' ],
)

subdir('src')

inc = include_directories('include')

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
