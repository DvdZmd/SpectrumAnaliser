#ifndef HISTOGRAM_DRAWING_AREA_H
#define HISTOGRAM_DRAWING_AREA_H

#include <gtkmm.h>
#include "defines.h"

class HistogramDrawingArea : public Gtk::DrawingArea
{
public:
    HistogramDrawingArea();
    virtual ~HistogramDrawingArea();
    Glib::RefPtr<Gdk::Pixbuf> pixBufHistogram;

protected:

    int width, height;


    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void on_size_allocate(Gtk::Allocation& allocation) override;
    bool everyNowAndThen();


    sigc::connection everyNowAndThenConnection; //https://developer.gnome.org/libsigc%2B%2B/2.2/structsigc_1_1connection.html#details

};





#endif
