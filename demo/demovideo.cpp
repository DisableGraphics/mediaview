#include <mediaview/videoview.hpp>

int main()
{
    auto app = Gtk::Application::create();

    Gtk::Window win;
    VideoView view;
    win.add(view);
    view.load_file("demo.mp4");
    
    win.show_all();
    view.play();
    return app->run(win);
}