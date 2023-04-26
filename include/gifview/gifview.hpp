#include "gdkmm/pixbufanimation.h"
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
        void setMaxSize(int width, int height);
        void setMinSize(int width, int height);

        // Functions that set the cache's properties
        void setCacheEnabled(bool enabled);

        // Start & Stop the animation
        void start();
        void stop();

        // Getters
        int getDelay() const;
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
        // Suboptimal performance having to resize the pixbuf every frame
        // has left me with no choice but to use a vector of pixbufs as cache
        std::vector<Glib::RefPtr<Gdk::Pixbuf>> m_frames_cache;
        int m_max_width = -1, m_max_height = -1, m_min_width = -1, m_min_height = -1;
        bool m_loaded = false;
        bool m_resize = false;
        int m_delay = 100;
        bool m_loop = true;
        bool m_playing = false;
        bool m_finished_cache = false;
        bool m_cache_enabled = true;
        int m_curr_frame = 0;
        sigc::connection m_connection;
};