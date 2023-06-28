#ifndef GIFVIEW_HPP
#define GIFVIEW_HPP
#include <iostream>
#include <gtkmm-3.0/gtkmm.h>
namespace mv
{
    class GifView;
}
/**
* \brief A widget that displays a GIF
* \details This widget displays an animated GIF in a gtkmm3 application. It can be transparently resized, and the animation can be started and stopped.
*/
class mv::GifView : public Gtk::DrawingArea
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

        // Functions that set the Pixbuf's properties
        void setResizeAutomatically(bool resize);
        void setMaxSize(int width, int height);
        void setMinSize(int width, int height);

        // Functions that set the cache's properties
        void setCacheEnabled(bool enabled);

        // Start & Stop the animation
        void start();
        void stop();

        // Getters
        int getDelay() const;
    protected:
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
        int m_max_width = -1, m_max_height = -1, m_min_width = -1, m_min_height = -1;
        bool m_loaded = false;
        bool m_resize = false;
        int m_delay = 100;
        bool m_playing = false;
        sigc::connection m_connection;
};
#endif //GIFVIEW_HPP