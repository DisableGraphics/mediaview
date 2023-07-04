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
        SingleDimensionResImg() : ResImg() {}
        SingleDimensionResImg(const std::string &file, ResizeDir resdir) : ResImg(file), resdir(resdir) {}
        SingleDimensionResImg(Glib::RefPtr<Gdk::Pixbuf> pixbuf, ResizeDir resdir) : ResImg(pixbuf), resdir(resdir) {}

        virtual ~SingleDimensionResImg(){}

        void set_resize_orientation(ResizeDir resdir){this->resdir = resdir;}
    private:
        ResizeDir resdir;
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
        void resize_img(int width, int height);
};

#endif //SINGLEDIMRESIZABLE_IMG_HPP