#include <mediaview/resizable-img.hpp>
#include <iostream>
/**
* \brief Construct a new ResImg::ResImg object
* \details This is the default constructor for the ResImg class.
*/
mv::ResImg::ResImg()
{
  
}
/**
* \brief Construct a new ResImg::ResImg object
* \details This is a constructor for the ResImg class that takes a filename.
* \param path The path of the image to load.
*/
mv::ResImg::ResImg(const std::string &path)
{
    load_image(path);
}
/**
* \brief Construct a new ResImg::ResImg object
* \details This is a constructor for the ResImg class that takes a pixbuf.
* \param image The pixbuf to load.
*/
mv::ResImg::ResImg(const Glib::RefPtr<Gdk::Pixbuf> image)
{
    m_image = image;
}
/**
* \brief Load an image from a file
* \details This function loads an image from a file and tries to create a pixbuf from it.
* \param path The path of the image to load.
*/
void mv::ResImg::load_image(const std::string &path)
{
    try
    {
        m_image = Gdk::Pixbuf::create_from_file(path);
    }
    catch(const Gio::ResourceError& ex)
    {
        std::cerr << "ResourceError: " << ex.what() << std::endl;
    }
    catch(const Gdk::PixbufError& ex)
    {
        std::cerr << "PixbufError: " << ex.what() << std::endl;
    }
    catch(const Glib::FileError& ex)
    {
        std::cerr << "FileError: " << ex.what() << std::endl;
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}
/**
* \brief Destructor for the ResImg::ResImg class.
*/
mv::ResImg::~ResImg()
{
}

void mv::ResImg::set_keep_aspect_ratio(bool keep)
{
    aspect_ratio = keep;
}

bool mv::ResImg::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    if (!m_image)
        return false;

    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    resize_img(width, height);

    // Draw the image in the middle of the drawing area, or (if the image is
    // larger than the drawing area) draw the middle part of the image.
    if(width > 0 && height > 0)
    {
        Gdk::Cairo::set_source_pixbuf(cr, m_resized_image,
            (width - m_resized_image->get_width())/2, (height - m_resized_image->get_height())/2);
        cr->paint();
    }

    return true;
}

void mv::ResImg::resize_img(int width, int height)
{
    if(width > 0 && height > 0)
    {
        if(aspect_ratio)
        {
            //Maintain aspect ratio and resize m_image to the maximum possible size
            //that fits into the drawing area.
            double aspect_ratio = (double)m_image->get_width() / m_image->get_height();
            if (width / aspect_ratio > height)
            {
                width = height * aspect_ratio;
            }
            else
            {
                height = width / aspect_ratio;
            }
        }
        if(width > 0 && height > 0)
            m_resized_image = m_image->scale_simple(width, height, Gdk::INTERP_BILINEAR);
    }
}