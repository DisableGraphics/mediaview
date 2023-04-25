#include <gifview/gifview.hpp>

int main()
{
    auto app = Gtk::Application::create();
    Gtk::Window win;
    gv::GifView gifview;

    gifview.setGif("demo.gif");
    gifview.setLoop(true);
    gifview.start();
    gifview.setResizeAutomatically(true);

    gifview.setMaxHeight(200);
    gifview.setMaxWidth(200);

    win.add(gifview);
    win.show_all();
    win.resize(400, 400);

    return app->run(win);
}