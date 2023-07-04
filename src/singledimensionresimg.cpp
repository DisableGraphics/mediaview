#include <mediaview/singledimensionresimg.hpp>

void mv::SingleDimentionResImg::resize_img(int width, int height)
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
            m_resized_image = m_image->scale_simple(resdir == HORIZONTAL ? width : -1, resdir == VERTICAL ? height : -1, Gdk::INTERP_BILINEAR);
    }
}

bool mv::SingleDimentionResImg::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
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