#include <syslog.h>
#include <iostream>

#include "drwArea_Histogram.hpp"

HistogramDrawingArea::HistogramDrawingArea()
{
   // Lets refresh drawing area very now and then.
   everyNowAndThenConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &HistogramDrawingArea::everyNowAndThen), 10);

}


HistogramDrawingArea::~HistogramDrawingArea()
{
    everyNowAndThenConnection.disconnect();
}

void HistogramDrawingArea::on_size_allocate(Gtk::Allocation& allocation)
{
    // Call the parent to do whatever needs to be done:
    DrawingArea::on_size_allocate(allocation);

    // Remember the new allocated size for resizing operation:
    width = allocation.get_width();
    height = allocation.get_height();}

/**
 * Every now and then, we invalidate the whole Widget rectangle,
 * forcing a complete refresh.
 */
bool HistogramDrawingArea::everyNowAndThen()
{
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, width, height);
        win->invalidate_rect(r, false);
    }

    // Don't stop calling me:
    return true;
}

bool HistogramDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    
    cr->set_source_rgb(1.0, 0.0, 0.0);
    cr->set_line_width(5);
    cr->move_to(0,0);
    cr->line_to(0,height);
    cr->line_to(width,height);
    cr->line_to(width,0);
    cr->line_to(0,0);


    cr->stroke();


    // //Me paro al final del frame y continuo desde ahí porque estoy usando el mismo Drawing Area
    // for(int j = publicRawFrame.rows; j < publicRawFrame.rows+256; j++)
    // {
    //     cr->move_to(0,j);
    //     cr->line_to(cols,j);
    // }


    // Don't stop calling me.
    return true;
}