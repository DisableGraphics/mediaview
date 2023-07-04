project('mediaview', 'cpp', version : '0.1.0', license : 'MIT', default_options : ['cpp_std=c++17'])

# Add include directories
inc = include_directories('include')

gtkmm_dep = dependency('gtkmm-3.0', version : '>=3.22.0')

# Add source files
src_gif = [
    'src/gifview.cpp',
    'src/resizable-img.cpp',
    'src/singledimensionresimg.cpp'
]

# Add library
lib_gif = library('mediaview', src_gif, include_directories : inc, dependencies : gtkmm_dep)

# Declare dependencies
mediaview_dep = declare_dependency(link_with : lib_gif, include_directories : inc, dependencies : gtkmm_dep)

# Demo executable
demogif = executable('demogif', 'demo/demogif.cpp', dependencies : mediaview_dep)
demoimg = executable('demoimg', 'demo/demoimg.cpp', dependencies : mediaview_dep)
demosdresimg = executable('demosdresimg', 'demo/demosdresimg.cpp', dependencies : mediaview_dep)