#include <mediaview/videoview.hpp>
#include <gst/video/videooverlay.h>
#include <iostream>

VideoView::VideoView()
{
    //Initialize gstreamer and create the playbin element
    gst_init(nullptr, nullptr);
    playbin_ = gst_element_factory_make("playbin", "playbin");
    if(!playbin_)
        throw std::runtime_error("Failed to create playbin element");

    //Create the pipeline
    pipeline_ = gst_pipeline_new("pipeline");
    if(!pipeline_)
        throw std::runtime_error("Failed to create pipeline");

    //Add the playbin element to the pipeline
    gst_bin_add(GST_BIN(pipeline_), playbin_);

    //Connect the draw signal
    signal_draw().connect(sigc::mem_fun(*this, &VideoView::on_drawing_area_draw));

    //Set the widget to expand
    set_hexpand(true);
    set_vexpand(true);
    
}

VideoView::~VideoView()
{
    stop();
    gst_element_set_state (playbin_, GST_STATE_NULL);
    gst_object_unref(pipeline_);
}

void VideoView::play()
{
    gst_element_set_state(pipeline_, GST_STATE_PLAYING);
}

void VideoView::pause()
{
    gst_element_set_state(pipeline_, GST_STATE_PAUSED);
}

void VideoView::stop()
{
    gst_element_set_state(pipeline_, GST_STATE_NULL);
}

bool VideoView::on_drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    GstState state;
    gst_element_get_state(pipeline_, &state, nullptr, GST_CLOCK_TIME_NONE);
    if (state < GST_STATE_PAUSED) {
        Gtk::Allocation allocation;

        /* Cairo is a 2D graphics library which we use here to clean the video window.
        * It is used by GStreamer for other reasons, so it will always be available to us. */
        //gtk_widget_get_allocation (*this->gobj(), &allocation);
        allocation = this->get_allocation();
        std::cout << "allocation: " << allocation.get_width() << "x" << allocation.get_height() << std::endl;
        //cairo_set_source_rgb (cr, 0, 0, 0);
        cr->set_source_rgb(0, 0, 0);
        //cairo_rectangle (cr, 0, 0, allocation.width, allocation.height);
        cr->rectangle(0, 0, allocation.get_width(), allocation.get_height());
        //cairo_fill (cr);
        cr->fill();
        cr->stroke();
        cr->paint();
  }

  return true;
}

void VideoView::load_file(const std::string &file_path)
{
    //Set the file path
    g_object_set(G_OBJECT(playbin_), "uri", ("file://" + file_path).c_str(), nullptr);

    //Set the video sink
    g_object_set(G_OBJECT(playbin_), "video-sink", pipeline_, nullptr);

}