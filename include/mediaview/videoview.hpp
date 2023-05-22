#include <gtkmm.h>
#include <gstreamermm.h>

class VideoPlayerWidget : public Gtk::VBox
{
    public:
    VideoPlayerWidget()
    {
        // Create a DrawingArea widget to display the video
        drawing_area_.property_hexpand() = true;
        drawing_area_.property_vexpand() = true;

        // Create the play/pause button
        play_button_.property_label() = "Play";
        play_button_.signal_clicked().connect(
        sigc::mem_fun(*this, &VideoPlayerWidget::on_play_button_clicked));

        // Create the volume slider
        volume_adjustment_ = Gtk::Adjustment::create(50.0, 0.0, 100.0, 1.0, 1.0, 1.0);
        volume_scale_.set_adjustment(volume_adjustment_);
        volume_scale_.set_value_pos(Gtk::POS_RIGHT);
        volume_scale_.signal_value_changed().connect(
        sigc::mem_fun(*this, &VideoPlayerWidget::on_volume_changed));

        // Pack the widgets into the box
        pack_start(drawing_area_, true, true);
        pack_start(play_button_, Gtk::PACK_SHRINK);
        pack_start(volume_scale_, Gtk::PACK_SHRINK);

        // Connect the signal handler for the drawing area
        drawing_area_.signal_draw().connect(
        sigc::mem_fun(*this, &VideoPlayerWidget::on_drawing_area_draw));

        // Initialize GStreamer
        Gst::init();

        // Create the GStreamer pipeline
        pipeline_ = Gst::Pipeline::create();
        playbin_ = Gst::ElementFactory::create_element("playbin")->
        dynamic_cast_gi<Gst::PlayBin>();

        if (pipeline_ && playbin_)
        {
        // Add playbin to the pipeline
        pipeline_->add(playbin_);

        // Set the video output to the DrawingArea widget
        playbin_->property_video_sink() = Gst::ElementFactory::create_element("gtksink");

        // Set the pipeline to the Playing state
        pipeline_->set_state(Gst::STATE_PLAYING);
        }
    }

    virtual ~VideoPlayerWidget() {}

    protected:
    bool on_drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
    {
        if (pipeline_)
        {
        auto gdk_window = drawing_area_.get_window()->gobj();
        playbin_->property_video_sink().get_handle().set_window_handle(gdk_window);
        playbin_->property_volume() = volume_adjustment_->get_value() / 100.0;
        }

        return false;
    }

    void on_play_button_clicked()
    {
        if (pipeline_)
        {
        Gst::State state;
        pipeline_->get_state(state, Gst::STATE_NULL);

        if (state == Gst::STATE_PLAYING)
        {
            pipeline_->set_state(Gst::STATE_PAUSED);
            play_button_.property_label() = "Play";
        }
        else
        {
            pipeline_->set_state(Gst::STATE_PLAYING);
            play_button_.property_label() = "Pause";
        }
        }
    }

    void on_volume_changed()
    {
        if (pipeline_)
        playbin_->property_volume() = volume_adjustment_->get_value() / 100.0;
    }

    Gtk::DrawingArea drawing_area_;
    Gtk::Button play_button_;
    Gtk::Scale volume_scale_;
    Glib::RefPtr<Gtk::Adjustment> volume_adjustment_;
    Glib::RefPtr<Gst::Pipeline> pipeline_;
    Glib::RefPtr<Gst::PlayBin> playbin_;
};