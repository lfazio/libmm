# SPDX Licence-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2025 Laurent Fazio <laurent.fazio@gmail.com>

cdata.set('TOP_SRCDIR', meson.project_source_root())
cdata.set('TOP_BUILDDIR', meson.project_build_root())

doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',
                          configuration: cdata,
                          install: false)

datadir = join_paths(get_option('datadir'), 'doc', 'libmm')

html_target = custom_target('doxygen',
                            input: doxyfile,
                            output: 'html',
                            command: [doxygen, doxyfile],
                            install: true,
                            install_dir: datadir)