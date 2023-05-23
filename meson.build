project('gifview', 'cpp', version : '0.1.0', license : 'MIT', default_options : ['cpp_std=c++17'])

# Add include directories
inc = include_directories('include')

gtkmm_dep = dependency('gtkmm-3.0', version : '>=3.22.0')
mpv_dep = [dependency('gstreamermm-1.0')]


# Add source files
src_gif = [
    'src/gifview.cpp'
]

# Add library
lib_gif = library('gifview', src_gif, include_directories : inc, dependencies : gtkmm_dep)

# Declare dependencies
gifview_dep = declare_dependency(link_with : lib_gif, include_directories : inc, dependencies : gtkmm_dep)

# Demo executable
demogif = executable('demogif', 'demo/demogif.cpp', dependencies : gifview_dep)