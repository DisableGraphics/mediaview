#include <gtkmm.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <iostream>
#include <gdk/gdkx.h>

class VideoView : public Gtk::DrawingArea
{
	public:
		VideoView()
		{
			// Initialize GStreamer
			gst_init(nullptr, nullptr);

			// Connect the signal handlers for the DrawingArea
			signal_draw().connect(sigc::mem_fun(*this, &VideoView::on_draw));
			signal_realize().connect(sigc::mem_fun(*this, &VideoView::on_realize));
			}

			virtual ~VideoView()
			{
				if (pipeline_)
				{
					gst_element_set_state(pipeline_, GST_STATE_NULL);
					gst_object_unref(pipeline_);
				}
			}

			void load_file(const std::string& file_path)
			{
			if (!pipeline_)
				return;

			GstElement* source = gst_element_factory_make("filesrc", nullptr);
			GstElement* decodebin = gst_element_factory_make("decodebin", nullptr);
			GstElement* videosink = gst_element_factory_make("gtksink", nullptr);

			if (!source || !decodebin || !videosink)
			{
				std::cerr << "Failed to create GStreamer elements" << std::endl;
				return;
			}

			g_object_set(G_OBJECT(source), "location", file_path.c_str(), nullptr);

			gst_bin_add_many(GST_BIN(pipeline_), source, decodebin, videosink, nullptr);
			gst_element_link(source, decodebin);
			gst_element_link(decodebin, videosink);

			g_signal_connect(decodebin, "pad-added", G_CALLBACK(on_pad_added), videosink);

			gst_element_set_state(pipeline_, GST_STATE_PLAYING);
			GdkWindow* gdkWindow = get_window()->gobj();

			gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(pipeline_), GDK_WINDOW_XID(gdkWindow));
			std::cout << "finished" << std::endl;
		}

	protected:
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  cr->set_source_rgb(0.0, 0.0, 0.0);
  cr->rectangle(0, 0, width, height);
  cr->fill();

  if (pipeline_)
  {
    GstElement* videosink = gst_bin_get_by_name(GST_BIN(pipeline_), "videosink");
    if (videosink)
    {
      auto gdkWindow = get_window();

      if (gdkWindow)
      {
        Gdk::Rectangle area(0, 0, width, height);
        gdkWindow->begin_paint_rect(area);

        // Retrieve the Gdk window handle
        GdkWindow* gdkWin = gdkWindow->gobj();
        if (gdkWin)
        {
          // Retrieve the Gdk window XID
          Window xid = GDK_WINDOW_XID(gdkWin);

          // Set the videosink's window handle
          g_object_set(G_OBJECT(videosink), "force-aspect-ratio", TRUE, "window-id", xid, nullptr);

          // Render the video frame onto the Gdk window
          gst_element_set_state(pipeline_, GST_STATE_PLAYING);
          gst_element_sync_state_with_parent(videosink);

          // End painting on the Gdk window
          gdkWindow->end_paint();
        }
      }

      gst_object_unref(videosink);
    }
  }

  return true;
}



		static void on_pad_added(GstElement* element, GstPad* pad, gpointer data)
		{
			GstElement* videosink = static_cast<GstElement*>(data);
			GstPad* sinkpad = gst_element_get_static_pad(videosink, "sink");

			if (!gst_pad_is_linked(sinkpad))
				gst_pad_link(pad, sinkpad);

			gst_object_unref(sinkpad);
		}

		GstElement* pipeline_ = nullptr;
};