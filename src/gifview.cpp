#include <mediaview/gifview.hpp>
#include <filesystem>

/**
* \brief Construct a new GifView::GifView object
* \details This is the default constructor for the GifView class.
*/
mv::GifView::GifView()
{

}

/**
* \brief Construct a new GifView::GifView object
* \details This is a copy constructor so std::vector doesn't complain
*/
mv::GifView::GifView(const GifView &other)
{
    setGif(other.m_animation);
}

/**
* \brief Construct a new GifView::GifView object
* \details This is a constructor for the GifView class that takes a filename.
* \param filename The filename of the GIF to load.
*/
mv::GifView::GifView(const std::string& filename)
{
    setGif(filename);
}

/**
* \brief Construct a new GifView::GifView object
* \details This is the constructor for the GifView class that takes a pixbuf animation.
* \param animation The pixbuf animation to load.
*/
mv::GifView::GifView(const Glib::RefPtr<Gdk::PixbufAnimation> animation)
{
    setGif(animation);
}

/**
* \brief Destroy the GifView::GifView object
* \details This is the destructor for the GifView class.
*/
mv::GifView::~GifView()
{
    stop();
}

/**
* \brief Set the GIF
* \details This function sets the GIF to display.
* \param filename The filename of the GIF to load.
* \throws std::runtime_error If the file does not exist.
*/
void mv::GifView::setGif(const std::string& filename)
{
    if(!std::filesystem::exists(filename))
        throw std::runtime_error("Can't load file: File " + filename + " does not exist ");
    setGif(Gdk::PixbufAnimation::create_from_file(filename));
}

/**
* \brief Set the GIF
* \details This function sets the GIF to display.
* \param animation The pixbuf animation to load.
*/
void mv::GifView::setGif(const Glib::RefPtr<Gdk::PixbufAnimation> animation)
{
    m_animation = animation;
    m_iter = m_animation->get_iter(NULL);
    m_pixbuf = m_iter->get_pixbuf();
    m_delay = m_iter->get_delay_time();
    m_playing = false;
    m_loaded = true;
    queue_draw();
}

/**
* \brief Set the delay
* \details This function sets the delay between frames.
* \param delay The delay between frames.
*/
void mv::GifView::setDelay(int delay)
{
    m_delay = delay;
}

/**
* \brief Start the GIF
* \details This function starts the GIF.
*/
void mv::GifView::start()
{
    m_connection.disconnect();
    if (!m_playing && m_loaded)
    {
        m_playing = true;
        m_connection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &mv::GifView::on_timeout), m_delay);
    }
}

/**
* \brief Stop the GIF
* \details This function stops the GIF.
*/
void mv::GifView::stop()
{
    if (m_playing && m_loaded)
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
bool mv::GifView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();
    if(m_pixbuf)
    {
        Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf, ((double)width - m_pixbuf->get_width()) / 2,
            ((double)height - m_pixbuf->get_height()) / 2);
        cr->stroke();
        cr->paint();
    }
    return true;
}

/**
* \brief Timeout
* \details This function is called when the timeout expires.
* \return Whether the timeout should be called again.
*/
bool mv::GifView::on_timeout()
{
    if (m_playing)
    {
        if (m_iter->advance())
        {
            try 
            {
                m_pixbuf = m_iter->get_pixbuf();
            } 
            catch(Glib::Error& e) 
            {
                std::cerr << e.what() << std::endl;
            }
            m_delay = m_iter->get_delay_time();
            if(m_resize)
            {
                resize_pixbuf();
            }
            queue_draw();
            return true;
        }
        else
        {
            return true;
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
void mv::GifView::resize_pixbuf(int width, int height, bool preserve_aspect_ratio)
{
    m_pixbuf = m_pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);
}

/**
* \brief Resize the pixbuf to fit the widget
* \details This function resizes the pixbuf to fit the widget.
* \param preserve_aspect_ratio Whether to preserve the aspect ratio or not.
*/
void mv::GifView::resize_pixbuf(bool preserve_aspect_ratio)
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
* \brief Set automatic resize of the GIF to fit the widget.
* \details This function sets whether the pixbuf should be automatically resized to fit the widget.
* \param resize Whether the pixbuf should be automatically resized to fit the widget.
*/
void mv::GifView::setResizeAutomatically(bool resize)
{
    m_resize = resize;
}

/**
* \brief Set the maximum size of the GIF
* \details This function sets the maximum size of the pixbuf
* \param width The maximum width.
* \param height The maximum height.
*/
void mv::GifView::setMaxSize(int width, int height)
{
    m_max_width = width;
    m_max_height = height;
}

/**
* \brief Set the minimum size of the GIF.
* \details This function sets the minimum size of the pixbuf.
* \param width The minimum width of the pixbuf.
* \param height The minimum height of the pixbuf.
*/
void mv::GifView::setMinSize(int width, int height)
{
    set_size_request(width, height);
}

/**
* \brief Get delay of GIF
* \details This function gets the delay of the animation.
* \return The delay of the animation.
*/
int mv::GifView::getDelay() const
{
    return m_delay;
}