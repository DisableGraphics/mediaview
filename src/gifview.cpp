#include <gifview/gifview.hpp>

/**
* \brief Construct a new GifView::GifView object
* \details This is the default constructor for the GifView class.
*/
gv::GifView::GifView()
{

}

/**
* \brief Construct a new GifView::GifView object
* \details This is the constructor for the GifView class that takes a filename.
* \param filename The filename of the GIF to load.
*/
gv::GifView::GifView(const std::string& filename)
{
    setGif(filename);
}

/**
* \brief Construct a new GifView::GifView object
* \details This is the constructor for the GifView class that takes a pixbuf animation.
* \param animation The pixbuf animation to load.
*/
gv::GifView::GifView(const Glib::RefPtr<Gdk::PixbufAnimation> animation)
{
    setGif(animation);
}

/**
* \brief Destroy the GifView::GifView object
* \details This is the destructor for the GifView class.
*/
gv::GifView::~GifView()
{
    stop();
}

/**
* \brief Set the GIF
* \details This function sets the GIF to display.
* \param filename The filename of the GIF to load.
*/
void gv::GifView::setGif(const std::string& filename)
{
    setGif(Gdk::PixbufAnimation::create_from_file(filename));
}

/**
* \brief Set the GIF
* \details This function sets the GIF to display.
* \param animation The pixbuf animation to load.
*/
void gv::GifView::setGif(const Glib::RefPtr<Gdk::PixbufAnimation> animation)
{
    m_animation = animation;
    m_iter = m_animation->get_iter(nullptr);
    m_pixbuf = m_iter->get_pixbuf();
    m_delay = m_iter->get_delay_time();
    m_loop = m_iter->on_currently_loading_frame();
    m_playing = false;
    queue_draw();
}

/**
* \brief Set the delay
* \details This function sets the delay between frames.
* \param delay The delay between frames.
*/
void gv::GifView::setDelay(int delay)
{
    m_delay = delay;
}

/**
* \brief Set the loop
* \details This function sets whether the GIF should loop.
* \param loop Whether the GIF should loop.
*/
void gv::GifView::setLoop(bool loop)
{
    m_loop = loop;
}

/**
* \brief Start the GIF
* \details This function starts the GIF.
*/
void gv::GifView::start()
{
    if (!m_playing)
    {
        m_playing = true;
        m_connection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &gv::GifView::on_timeout), m_delay);
    }
}

/**
* \brief Stop the GIF
* \details This function stops the GIF.
*/
void gv::GifView::stop()
{
    if (m_playing)
    {
        m_playing = false;
        m_connection.disconnect();
    }
}

/**
* \brief Draw the GIF
* \details This function draws the GIF.
* \param cr The Cairo context.
* \return Whether the GIF was drawn.
*/
bool gv::GifView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();
    if(m_pixbuf)
    {
        Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf, ((double)width - m_pixbuf->get_width()) / 2,
            ((double)height - m_pixbuf->get_height()) / 2);
        cr->paint();
    }
    return true;
}

/**
* \brief Timeout
* \details This function is called when the timeout expires.
* \return Whether the timeout should be called again.
*/
bool gv::GifView::on_timeout()
{
    if (m_playing)
    {
        if (m_iter->advance())
        {
            m_pixbuf = m_iter->get_pixbuf();
            m_delay = m_iter->get_delay_time();
            if(m_resize)
                resize_pixbuf();
            queue_draw();
            return true;
        }
        else if (m_loop)
        {
            m_iter.reset();
            m_pixbuf = m_iter->get_pixbuf();
            m_delay = m_iter->get_delay_time();
            if(m_resize)
                resize_pixbuf();
            queue_draw();
            return true;
        }
        else
        {
            m_playing = false;
            return false;
        }
    }
    else
    {
        return false;
    }
}


/**
* \brief Resize pixbuf
* \details This function resizes the pixbuf to the desired size.
* \param width The width of the pixbuf.
* \param height The height of the pixbuf.
* \param preserve_aspect_ratio Whether to preserve the aspect ratio or not.
*/
void gv::GifView::resize_pixbuf(int width, int height, bool preserve_aspect_ratio)
{
    m_pixbuf = m_pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);
    queue_draw();
}

/**
* \brief Resize the pixbuf to fit the widget
* \details This function resizes the pixbuf to fit the widget.
* \param preserve_aspect_ratio Whether to preserve the aspect ratio or not.
*/
void gv::GifView::resize_pixbuf(bool preserve_aspect_ratio)
{
    Gtk::Allocation allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();
    if(m_max_width != -1 || m_max_height != -1)
    {
        if(m_max_width != -1 && width > m_max_width)
            width = m_max_width;
        if(m_max_height != -1 && height > m_max_height)
            height = m_max_height;
    }
    if (preserve_aspect_ratio)
    {
        double aspect_ratio = (double)m_pixbuf->get_width() / m_pixbuf->get_height();
        if (width / aspect_ratio > height)
        {
            width = height * aspect_ratio;
        }
        else
        {
            height = width / aspect_ratio;
        }
    }
    resize_pixbuf(width, height, preserve_aspect_ratio);
}

/**
* \brief Set automatic resize of the pixbuf to fit the widget
* \details This function sets whether the pixbuf should be automatically resized to fit the widget.
* \param resize Whether the pixbuf should be automatically resized to fit the widget.
*/
void gv::GifView::setResizeAutomatically(bool resize)
{
    m_resize = resize;
}

/**
* \brief Set the maximum width of the pixbuf
* \details This function sets the maximum width of the pixbuf.
* \param width The maximum width of the pixbuf.
*/
void gv::GifView::setMaxWidth(int width)
{
    m_max_width = width;
}

/**
* \brief Set the maximum height of the pixbuf
* \details This function sets the maximum height of the pixbuf.
* \param height The maximum height of the pixbuf.
*/
void gv::GifView::setMaxHeight(int height)
{
    m_max_height = height;
}