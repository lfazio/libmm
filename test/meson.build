# SPDX Licence-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

gtest_dep = dependency('gtest', required : true)

test_string = executable('test_string',
  'test_string.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('string_test', test_string)

test_alloc = executable('test_alloc',
  'test_alloc.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('alloc_test', test_alloc)

test_alloc_track = executable('test_alloc_track',
  'test_alloc_track.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('alloc_track_test', test_alloc_track)
