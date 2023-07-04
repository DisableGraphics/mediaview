#include <mediaview/singledimensionresimg.hpp>

int main()
{
    auto app = Gtk::Application::create();
    Gtk::Window win;
    mv::SingleDimensionResImg resimg;

    resimg.load_image("demo.jpg");
    resimg.set_resize_orientation(ResizeDir::HORIZONTAL);
    resimg.set_keep_aspect_ratio(true);

    win.add(resimg);
    win.show_all();
    win.resize(400, 400);

    return app->run(win);
}