#include <mediaview/resizable-img.hpp>

int main()
{
    auto app = Gtk::Application::create();
    Gtk::Window win;
    mv::ResImg resimg;

    resimg.load_image("demo.jpg");
    resimg.set_keep_aspect_ratio(true);

    win.add(resimg);
    win.show_all();
    win.resize(400, 400);

    return app->run(win);
}