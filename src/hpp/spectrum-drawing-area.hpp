#ifndef CAMERA_DRAWING_AREA_H
#define CAMERA_DRAWING_AREA_H

#include <gtkmm.h>
#include <opencv2/highgui.hpp>
#include "defines.h"
#include "loger.hpp"



class SpectrumDrawingArea : public Gtk::DrawingArea {
public:
    cv::VideoCapture videoCapture;
    SpectrumDrawingArea();
    virtual ~SpectrumDrawingArea();

    Glib::RefPtr<Gdk::Pixbuf> publicPixBuf;
    cv::Mat publicRawFrame; 
    cv::Mat privateRaw;
    bool capturar = false;
    uint8_t* getHistograma();
    int modo = 1;
    int filaClick = -1;
    bool setingArea = false;
    int xInicial = 0, xFinal = 640;

    Loger* log;

    //for image move and zoom
    float zoomFactor = 0.5;
    bool applyZoom = false;
    float scaledFrameWidth;
    float scaledFrameHeight;
    int xUpLeftCorner;
    int yUpLeftCorner;

    bool movingImage = false;
    float newDeltaX;
    float newDeltaY;


    uint8_t *arrayHistograma;
    
protected:

    void BlankHistogramZone(const Cairo::RefPtr<Cairo::Context>& cr);
    void onDrawFromImage();
    void GraphHistograma(const Cairo::RefPtr<Cairo::Context>& cr);
    void SetAnalizeProperties();
    void GenerateHistogramArray();

    void onDrawCameraCapture(const Cairo::RefPtr<Cairo::Context>&);
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void on_size_allocate(Gtk::Allocation& allocation) override;
    bool everyNowAndThen();

private:
    sigc::connection everyNowAndThenConnection; //https://developer.gnome.org/libsigc%2B%2B/2.2/structsigc_1_1connection.html#details
    cv::Mat webcam;
    cv::Mat output;
    int width, height;

    uint8_t* pixelsPtr;
    int cols,rows;
    int bytesPerPixel;
    int bytesPerFila;
    int filaAnalizar = SPECTRUM_AREA_HEIGHT / 2;


};

#endif
