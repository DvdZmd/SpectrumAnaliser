#include <syslog.h>
#include <iostream>

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "../hpp/spectrum-drawing-area.hpp"

using namespace cv;

/*
    CONSTRUCTOR
*/
SpectrumDrawingArea::SpectrumDrawingArea()
{
    videoCapture.open(0); // open the default camera

    // Lets refresh drawing area very now and then.
    everyNowAndThenConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &SpectrumDrawingArea::everyNowAndThen), 10);

    //Sucede algo raro al setear las resoluciones, al mostrarlas en el drawImage se ve totalmente deforme
    videoCapture.set(CAP_PROP_CONVERT_RGB, true);
    videoCapture.set(CAP_PROP_FRAME_WIDTH, CAMERA_FRAME_WIDTH);
    videoCapture.set(CAP_PROP_FRAME_HEIGHT, CAMERA_FRAME_HEIGHT);
    videoCapture.set(CAP_PROP_BRIGHTNESS, 0.5);

    //Width & heigth of the visible image with zoom factor aplyed
    scaledFrameWidth = SPECTRUM_AREA_WIDTH * zoomFactor;
    scaledFrameHeight = SPECTRUM_AREA_HEIGHT * zoomFactor;

    //validations for evade a overflow
    if(scaledFrameWidth >= CAMERA_FRAME_WIDTH)
        scaledFrameWidth = CAMERA_FRAME_WIDTH;
    if(scaledFrameHeight >= CAMERA_FRAME_HEIGHT)
        scaledFrameHeight = CAMERA_FRAME_HEIGHT;

    //Set the x,y coordinates in the camera frame to be the up left corner of the visible image
    xUpLeftCorner = (CAMERA_FRAME_WIDTH / 2) - (scaledFrameWidth / 2);
    yUpLeftCorner = (CAMERA_FRAME_HEIGHT / 2) - (scaledFrameHeight / 2);
}

SpectrumDrawingArea::~SpectrumDrawingArea()
{
    everyNowAndThenConnection.disconnect();
}


/**
 * Every now and then, we invalidate the whole Widget rectangle,
 * forcing a complete refresh.
 */
bool SpectrumDrawingArea::everyNowAndThen()
{
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, width, height);
        win->invalidate_rect(r, false);
    }

    // Don't stop calling me:
    return true;
}

/**
 * Called every time the widget has its allocation changed.
 */
void SpectrumDrawingArea::on_size_allocate(Gtk::Allocation& allocation)
{
    // Call the parent to do whatever needs to be done:
    DrawingArea::on_size_allocate(allocation);

    // Remember the new allocated size for resizing operation:
    width = allocation.get_width();
    height = allocation.get_height();
}

/**
 * Called every time the widget needs to be redrawn.
 * This happens when the Widget got resized, or obscured by
 * another object, or every now and then.
 */
bool SpectrumDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    if(modo == 1)
        onDrawCameraCapture(cr);
    else
        onDrawFromImage();

    // Don't stop calling me.
    return true;
}

void SpectrumDrawingArea::onDrawCameraCapture(const Cairo::RefPtr<Cairo::Context>& cr)
{
    // Prevent the drawing if size is 0:
    if (width <= 1 || height <= 1)
        return;
    
    // Capture camera frame:
    videoCapture.read(publicRawFrame);

    cv::Mat privateRaw = publicRawFrame(Rect(xUpLeftCorner, yUpLeftCorner, scaledFrameWidth, scaledFrameHeight));
    resize(privateRaw, privateRaw, cv::Size(SPECTRUM_AREA_WIDTH, SPECTRUM_AREA_HEIGHT), 0, 0, cv::INTER_LINEAR);
    
    ///Creo el pixBuf con el frame de la cámara para mostrarlo en DrawingArea
    publicPixBuf = Gdk::Pixbuf::create_from_data(
        (guint8*)privateRaw.data,//data: Image data in 8-bit/sample packed format.
        Gdk::COLORSPACE_RGB,         //colorspace: 	Colorspace for the image data.
        false,                       //has_alpha: Whether the data has an opacity channel.
        8,                           //bits_per_sample: Number of bits per sample.
        privateRaw.cols,         //width: Width of the image in pixels.
        privateRaw.rows,         //height: Height of the image in pixels.
        (int)privateRaw.step);   //rowstride: Distance in bytes between rows.           
    
    SetAnalizeProperties();

    //This is for swith the frame from BGR to RGB
    for(int i = 0; i < cols; i++)
    {
        if(filaClick < 0)
            filaAnalizar = publicPixBuf->get_height()/2;
        else
            filaAnalizar = filaClick;

        int offset = filaAnalizar * bytesPerFila + i * bytesPerPixel;
        char b = pixelsPtr[offset];
        char g = pixelsPtr[offset + 1];
        char r = pixelsPtr[offset + 2];

        uint8_t val = (uint8_t)((b+g+r) / 3);
        arrayHistograma[i] = val;
    }

    ///Muestro el frame el DrawingArea
    Gdk::Cairo::set_source_pixbuf(cr, publicPixBuf);
    cr->paint();
}

void SpectrumDrawingArea::SetAnalizeProperties()
{
    //Propiedades del frame que usaré para analizar
    pixelsPtr      = (uint8_t*)publicPixBuf->get_pixels();
    cols           = publicPixBuf->get_width();
    rows           = publicPixBuf->get_height();
    bytesPerPixel  = publicPixBuf->get_n_channels();
    bytesPerFila   = publicPixBuf->get_rowstride();    
}

void SpectrumDrawingArea::onDrawFromImage()
{

}

uint8_t* SpectrumDrawingArea::getHistograma()
{
    return arrayHistograma;
}
