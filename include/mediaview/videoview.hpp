#include <gtkmm.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

/**
* \brief Video view widget
*/
class VideoView : public Gtk::DrawingArea
{
    public:
        VideoView();
        virtual ~VideoView();
        void load_file(const std::string& file_path);

        void play();
        void pause();
        void stop();

    protected:
        bool on_drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr);

        Glib::RefPtr<Gtk::Adjustment> volume_adjustment_;
        GstElement* pipeline_;
        GstElement* playbin_;
};