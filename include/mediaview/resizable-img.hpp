#ifndef RESIZABLE_IMG_HPP
#define RESIZABLE_IMG_HPP
#include <gtkmm.h>

namespace mv
{
    class ResImg;
}
/**
* \brief A widget that displays an image and automatically resizes it.
* \details This widget displays a static image in a gtkmm3 application. It automatically resizes the image to fit the widget's size.
*/
class mv::ResImg : public Gtk::DrawingArea
{
    public:
        ResImg();
        ResImg(const ResImg &other);
        ResImg(const std::string &path);
        ResImg(const Glib::RefPtr<Gdk::Pixbuf> image);
        virtual ~ResImg();
        void load_image(const std::string &path);
        void load_image(const Glib::RefPtr<Gdk::Pixbuf> image);
        void set_keep_aspect_ratio(bool keep);

        Glib::RefPtr<Gdk::Pixbuf> get_pixbuf();

    protected:
        //Override default signal handler:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
        void resize_img(int width, int height);
        bool aspect_ratio = true;

        Glib::RefPtr<Gdk::Pixbuf> m_image;
        Glib::RefPtr<Gdk::Pixbuf> m_resized_image;
};

#endif //RESIZABLE_IMG_HPP