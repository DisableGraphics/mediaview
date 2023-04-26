# GifView
Simple and effective GIF viewer widget for GTKMM 3
# Installing the library
## Dependencies
* gtkmm 3.0

## Installing dependencies
### Ubuntu
```
sudo apt-get install libgtkmm-3.0-dev
```
### Arch Linux
```
sudo pacman -S gtkmm3
```
### Fedora
```
sudo dnf install gtkmm30-devel
```
# Compiling the library
```
git clone https://github.com/DisableGraphics/GifView.git
cd GifView
meson setup builddir
cd builddir
meson compile # You can change it for "ninja" if your meson doesn't have the compile command
```
# Usage
* Since this library uses meson as its build system, you can use the subproject feature to import it into your project.
## Importing into a project
* Create a meson.build file and add this line to it:
```
gifview_dep = dependency('gifview', fallback: ['gifview', 'gifview_dep'])
```
* If you dont' know how to create a meson.build file, take a look [at this tutorial](https://mesonbuild.com/Tutorial.html)
* Link the library to your executable:
```
executable('your_executable', 'your_executable.cpp', dependencies: gifview_dep)
```
* Include the header file in your code:
```
#include <gifview/gifview.hpp>
```
## Using the library
* This is an very basic example:
```
#include <gtkmm.h>
#include <gifview/gifview.hpp>

int main()
{
    //Create application
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();
    //Create the window
    Gtk::Window window;
    //Create the gifview
    gv::GifView gifview = gv::GifView("/path/to/your/gif/file.gif");

    //Set the GIF to loop
    gifview.setLoop(true);
    //Start the GIF
    gifview.start();

    //Add the gifview widget to the window
    window.add(gifview);
    //Show the window
    window.show_all();
    return app->run(window);
}
```
# Why GifView if I can use Gtk::Image and Gdk::PixbufAnimation?
* GifView can be resized whereas Gtk::Image cannot
* Gifview does not need an external pixbuf
