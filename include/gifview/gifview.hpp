#include <iostream>
#include <gtkmm-3.0/gtkmm.h>
namespace gv
{
    class GifView;
}
class gv::GifView : public Gtk::DrawingArea
{
    public:
        // Constructors
        GifView();
        GifView(const std::string& filename);
        GifView(const Glib::RefPtr<Gdk::PixbufAnimation> animation);

        // Destructor
        virtual ~GifView();

        // Functions that load the GIF
        void setGif(const std::string& filename);
        void setGif(const Glib::RefPtr<Gdk::PixbufAnimation> animation);

        // Functions that set the GIF's properties
        void setDelay(int delay);
        void setLoop(bool loop);

        // Functions that set the Pixbuf's properties
        void setResizeAutomatically(bool resize);
        void setMaxWidth(int width);
        void setMaxHeight(int height);

        // Start & Stop the animation
        void start();
        void stop();
    private:
        // Events that handle the Pixbuf
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
        bool on_timeout();

        // Functions that handle the Pixbuf resizing
        void resize_pixbuf(int width, int height, bool preserve_aspect_ratio = true);
        void resize_pixbuf(bool preserve_aspect_ratio = true);

        // Attributes
        Glib::RefPtr<Gdk::PixbufAnimation> m_animation;
        Glib::RefPtr<Gdk::PixbufAnimationIter> m_iter;
        Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;
        int m_max_width = -1, m_max_height = -1;
        bool m_resize = false;
        int m_delay = 0;
        bool m_loop = true;
        bool m_playing = false;
        sigc::connection m_connection;
};