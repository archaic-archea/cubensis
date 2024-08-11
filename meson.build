project('cubensis', 'c')

ex_sources = [] # Source files
ex_deps = [] # Library dependencies

example_name = get_option('use_example')

subdir('src')

if example_name != 'none'
    ex_prefix = 'src/examples' / example_name
    include_dir = include_directories('src/examples')

    ex_relsrc = []
    foreach rsrc : ex_sources
        ex_relsrc += ex_prefix / rsrc
    endforeach

    compiler = meson.get_compiler('c')
    ex_libdeps = [] # Actual dependency object array
    foreach ldep : ex_deps
        libdeps += compiler.find_library(ldep, required: true)
    endforeach

    executable('example_' + example_name, ex_relsrc, include_directories: include_dir, dependencies: ex_libdeps)
endif