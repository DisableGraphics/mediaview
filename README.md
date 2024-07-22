# MediaView
Simple and easy to use media viewing library for Gtkmm3
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
git clone https://github.com/DisableGraphics/mediaview.git mediaview
cd mediaview
meson setup builddir
cd builddir
meson compile # You can change it for "ninja" if your meson doesn't have the compile command
```

# Features
- ResImg: Automatically resizable static image widget. (It resizes automatically to fit the widget)
- GifView: GIF viewer that can be automatically resized, or set a maximum width and height.

# Usage
* Since this library uses meson as its build system, you can use the subproject feature to import it into your project.
* You'll need to create a folder named "subprojects" in your project's root directory, and place the "mediaview" folder inside it.
## Importing into a project
* Create a meson.build file and add this line to it:
```
mediaview_dep = dependency('mediaview', fallback: ['mediaview', 'mediaview_dep'])
```
* If you dont' know how to create a meson.build file, take a look [at this tutorial](https://mesonbuild.com/Tutorial.html)
* Link the library to your executable:
```
executable('your_executable', 'your_executable.cpp', dependencies: mediaview_dep)
```
* Include the header file in your code:
```
#include <mediaview/mediaview.hpp>
```
## Using the library
* This is an very basic example:
```
#include <gtkmm.h>
#include <mediaview/mediaview.hpp>

int main()
{
    //Create application
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();
    //Create the window
    Gtk::Window window;
    //Create the gifview
    mv::GifView gifview = mv::GifView("/path/to/your/gif/file.gif");

    //Start the GIF
    gifview.start();

    //Add the gifview widget to the window
    window.add(gifview);
    //Show the window
    window.show_all();
    return app->run(window);
}
```
## Why MediaView?
- MediaView is easy to use. Taking into account how convoluted Gtkmm can be for some things, this is an achievement.
- MediaView was made with automatic resizing in mind. Gtkmm isn't and you have to implement it yourself.

## Notes
- Thread safety: Gtkmm is not thread safe. Since I'm using it as a base for my library, MediaView isn't thread safe too. I've found that ResImg in particular spits out exception messages instead of loading images when creating it inside a thread.
