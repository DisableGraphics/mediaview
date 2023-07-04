#ifndef SINGLEDIMRESIZABLE_IMG_HPP
#define SINGLEDIMRESIZABLE_IMG_HPP
#include "resizable-img.hpp"

namespace mv
{
    class SingleDimensionResImg;
}

enum ResizeDir
{
    VERTICAL,
    HORIZONTAL
};
/**
* \brief Static image that resizes only in a specific direction.
* \details This static image only resizes trying to fit in a specific direction, ignoring other directions. For example, if we set an object of this class 
* to only resize in a horizontal dimension, it will automatically fill the rest of the other space, ignoring any widgets in the way.
*/
class mv::SingleDimensionResImg : public ResImg
{
    public:
        /**
        * \brief Constructor
        * \details Default constructor
        */
        SingleDimensionResImg() : ResImg() {}
        /**
        * \brief Constructor
        * \details Constructs the object with a filename and a resize direction
        */
        SingleDimensionResImg(const std::string &file, ResizeDir resdir) : ResImg(file), resdir(resdir) {}
        /**
        * \brief Constructor
        * \details Constructs the object with a pixbuf and a resize direction
        */
        SingleDimensionResImg(Glib::RefPtr<Gdk::Pixbuf> pixbuf, ResizeDir resdir) : ResImg(pixbuf), resdir(resdir) {}
        /**
        * \brief Constructor
        * \details Copy constructor so std::vector doesn't complain
        */
        SingleDimensionResImg(const SingleDimensionResImg& other) : ResImg(other.m_image), resdir(other.resdir) {}

        virtual ~SingleDimensionResImg(){}

        void set_resize_orientation(ResizeDir resdir){this->resdir = resdir;}
    private:
        ResizeDir resdir;
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
        void resize_img(int width, int height);
};

#endif //SINGLEDIMRESIZABLE_IMG_HPP