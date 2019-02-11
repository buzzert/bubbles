# ⓑⓤⓑⓑⓛⓔⓢ

Bubbles is a very basic actor-based layer hierarchy for drawable nodes. It allows you to organize a hierarchy of drawable "actors" (the nodes) into a "scene". The scene then acts as a sort of runloop, responsible for rendering actors in their respective positions. 

## Building
### Dependencies
cairo
pango
pangocairo
freetype2
pangoft2
x11
xfixes

### Compiling 
This project uses the Meson build system.
```
meson build
ninja -C build
```

## Actors

Each actor has a `render` and `update` method. These methods are called by the scene every turn of the runloop. `render` takes a Cairo context `cairo_t` that you draw into. The scene already takes care of raster groups and any parent transformations that need to be applied. 

## Examples

Check out the [example](example/) for some basic usage. 

## Intention

I created this project because I found myself writing this same code over and over again for any project that needed to render a complex drawable hierarchy to a framebuffer (i.e., an X11 window or an LED matrix). Ideally I'd like to avoid using something like GTK or Qt since I am targeting platforms that may not have a high resolution bitmap display (like an LED matrix). Theoretically this could be divorced from the platform completely, as its only dependency is the Cairo Graphics Library.

## Why did you call it bubbles

I don't remember. I think it was the first name that came to my head. I do really like the 1982 _Williams_ classic arcade game [Bubbles](https://en.wikipedia.org/wiki/Bubbles_(video_game))


