project('gifview', 'cpp', version : '0.1.0', license : 'MIT', default_options : ['cpp_std=c++17'])

# Add include directories
inc = include_directories('include')

gtkmm_dep = dependency('gtkmm-3.0', version : '>=3.22.0')

# Add source files
src = [
    'src/gifview.cpp'
]

# Add library
lib = library('gifview', src, include_directories : inc, dependencies : gtkmm_dep)

# Declare dependencies
gifview_dep = declare_dependency(link_with : lib, include_directories : inc, dependencies : gtkmm_dep)

# Demo executable
demo = executable('demo', 'demo/demo.cpp', dependencies : gifview_dep)