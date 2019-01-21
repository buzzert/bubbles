project('bubbles', ['cpp', 'c'])

sources = [
  'src/bubbles/bubbles.h',
  'src/bubbles/core/foundation.h',
  'src/bubbles/core/color.h',
  'src/bubbles/core/defines.h',
  'src/bubbles/core/geometry.h',
  'src/bubbles/core/geometry.cpp',
  'src/bubbles/core/x11_support.h',
  'src/bubbles/core/x11_support.c',

  'src/bubbles/actor/actor.h',
  'src/bubbles/actor/actor.cpp',
  'src/bubbles/actor/actor_grid.h',
  'src/bubbles/actor/actor_grid.cpp',
  'src/bubbles/actor/label_actor.h',
  'src/bubbles/actor/label_actor.cpp',

  'src/bubbles/scene/scene.h',
  'src/bubbles/scene/scene.cpp',
]

dependencies = [
  dependency('cairo'),
  dependency('pango'),
  dependency('pangocairo'),
  dependency('sdl2'),
  dependency('SDL2_image'),
  dependency('freetype2'),
  dependency('pangoft2'),
  dependency('x11')
]

bubbles_includes = include_directories('src')

bubbles_lib = static_library('bubbles',
  sources,
  dependencies: dependencies,
  include_directories: bubbles_includes
)

bubbles_dep = declare_dependency(
  link_with: [ bubbles_lib ],
  include_directories: bubbles_includes
)

subdir('example')


