project('gifview', 'cpp', version : '0.1.0', license : 'MIT', default_options : ['cpp_std=c++17'])

# Add include directories
inc = include_directories('include')

gtkmm_dep = dependency('gtkmm-3.0', version : '>=3.22.0')
gstreamermm_dep = dependency('gstreamermm-1.0', version : '>=1.10.0')

# Add source files
src_gif = [
    'src/gifview.cpp'
]

# Add source files for VideoView
src_video = [
    'src/videoview.cpp'
]

# Add library
lib_gif = library('gifview', src_gif, include_directories : inc, dependencies : gtkmm_dep)
video_gif = library('videoview', src_video, include_directories : inc, dependencies : [gtkmm_dep, gstreamermm_dep])

# Declare dependencies
gifview_dep = declare_dependency(link_with : lib_gif, include_directories : inc, dependencies : gtkmm_dep)
videoview_dep = declare_dependency(link_with : video_gif, include_directories : inc, dependencies : [gtkmm_dep, gstreamermm_dep])

mediaview_dep = declare_dependency(link_with : [lib_gif, video_gif], include_directories : inc, dependencies : [gtkmm_dep, gstreamermm_dep])

# Demo executable
demo = executable('demogif', 'demo/demogif.cpp', dependencies : gifview_dep)