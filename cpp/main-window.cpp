#include <syslog.h>
#include <unistd.h>
#include <string>
#include <gtkmm.h>
#include <dirent.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "main-window.hpp"
#include "defines.h"

#define X_INICIAL 1
#define X_FINAL   2

using namespace cv;

MainWindow::MainWindow(int width, int height)
    : probablyInFullScreen(false),
      //m_box(Gtk::ORIENTATION_VERTICAL),
      m_adjustment( Gtk::Adjustment::create(0.0, 0.0, 101.0, 0.1, 1.0, 1.0) )//,
{

    this->set_default_size(width, height);

    PackWidgets();
    ConnectSignals();

    //Set widgets configuratios
    spectrumDrawingArea.set_size_request(SPECTRUM_AREA_WIDTH, SPECTRUM_AREA_HEIGHT);
    spectrumDrawingArea.arrayHistograma = arrayHistograma;
    histogramDrwArea.set_size_request(HISTOGRAM_WIDTH, HISTOGRAM_HEIGHT);
    histogramDrwArea.arrayHistograma = arrayHistograma;

    // Add the box in this window:
    add(principalBox);

    ShowWidgets();

    // Activate Key-Press events
    add_events(Gdk::KEY_PRESS_MASK);

    std::string path = "/mnt/ramdisk/log.txt";
    log.setLogPath(path);
    log.clearLog();
    log.writeLog("Hola");

    spectrumDrawingArea.log = &log;
}


void MainWindow::SetAnalisisArea()
{
    if(spectrumDrawingArea.setingArea)
    {
        spectrumDrawingArea.setingArea = false;
    }
    else
    {
        spectrumDrawingArea.setingArea = true;
        machineStateSettingArea = X_INICIAL;
        spectrumDrawingArea.filaClick = -1;//para dejar de graficar actualmente
    }
}

void MainWindow::ConnectSignals()
{
    //This is for zoom in/out the camera frame
    spectrumDrawingArea.add_events(Gdk::SCROLL_MASK);
    spectrumDrawingArea.signal_scroll_event().connect(
        sigc::mem_fun(*this, &MainWindow::scrollMouse)
    );

    //This for move the visible area when zoom in
    spectrumDrawingArea.add_events(Gdk::BUTTON_PRESS_MASK);
    spectrumDrawingArea.signal_button_press_event().connect(
        sigc::mem_fun(*this, &MainWindow::clickDown)
    );
    spectrumDrawingArea.add_events(Gdk::BUTTON_RELEASE_MASK);
    spectrumDrawingArea.signal_button_release_event().connect(
        sigc::mem_fun(*this, &MainWindow::clickUp)
    );
    spectrumDrawingArea.add_events(Gdk::POINTER_MOTION_MASK);
    spectrumDrawingArea.signal_motion_notify_event().connect(
        sigc::mem_fun(*this, &MainWindow::mouseMove)
    );

}



bool MainWindow::clickDown(GdkEventButton* button_event)
{
    spectrumDrawingArea.movingImage = true;

    previousX = button_event->x;
    previousY = button_event->y;

    // if(!spectrumDrawingArea.setingArea)
    //     spectrumDrawingArea.filaClick = (int)button_event->y;
    // else
    // {
    //     switch(machineStateSettingArea)
    //     {
    //         case X_INICIAL:
    //             spectrumDrawingArea.xInicial = (int)button_event->x;
    //             machineStateSettingArea = X_FINAL;

    //             break;
    //         case X_FINAL:
    //             spectrumDrawingArea.xFinal = (int)button_event->x;
    //             machineStateSettingArea = X_INICIAL;
    //             machineStateSettingArea = 0;
    //             spectrumDrawingArea.setingArea = false;
    //             break;
    //     }
    // }


    return false;
}

bool MainWindow::mouseMove(GdkEventMotion* eventMotion)
{
    if(spectrumDrawingArea.movingImage)
    {
        float newDeltaX = previousX - eventMotion->x;
        float newDeltaY = previousY - eventMotion->y;

        spectrumDrawingArea.xUpLeftCorner = spectrumDrawingArea.xUpLeftCorner + newDeltaX ;
        spectrumDrawingArea.yUpLeftCorner = spectrumDrawingArea.yUpLeftCorner + newDeltaY ;

        if(spectrumDrawingArea.xUpLeftCorner + spectrumDrawingArea.scaledFrameWidth >= CAMERA_FRAME_WIDTH)
            spectrumDrawingArea.xUpLeftCorner = CAMERA_FRAME_WIDTH - spectrumDrawingArea.scaledFrameWidth;
        if(spectrumDrawingArea.yUpLeftCorner + spectrumDrawingArea.scaledFrameHeight >= CAMERA_FRAME_HEIGHT)
            spectrumDrawingArea.yUpLeftCorner = CAMERA_FRAME_HEIGHT - spectrumDrawingArea.scaledFrameHeight;

        if(spectrumDrawingArea.xUpLeftCorner < 0 )
            spectrumDrawingArea.xUpLeftCorner = 0;
        if(spectrumDrawingArea.yUpLeftCorner < 0)
            spectrumDrawingArea.yUpLeftCorner = 0; 
    }

    previousX = eventMotion->x;
    previousY = eventMotion->y;

    return false;
}

bool MainWindow::clickUp(GdkEventButton* eventScroll)
{
    spectrumDrawingArea.movingImage = false;

    return false;
}

bool MainWindow::scrollMouse(GdkEventScroll* eventScroll)
{

    switch(eventScroll->direction)
    {
        case GDK_SCROLL_DOWN:
            if(this->spectrumDrawingArea.zoomFactor >= 1.0)
            {
                this->spectrumDrawingArea.zoomFactor = 1.0;
                return false;
            }            
            this->spectrumDrawingArea.zoomFactor += 0.01;

            break;
        case GDK_SCROLL_UP:
            if(this->spectrumDrawingArea.zoomFactor <= 0.1)
            {
                this->spectrumDrawingArea.zoomFactor = 0.1;
                return false;
            }
            this->spectrumDrawingArea.zoomFactor -= 0.01; 
            
            break;
        default:
            break;
    }


    //Width & heigth of the visible image with zoom factor aplyed
    float previousWidth = spectrumDrawingArea.scaledFrameWidth;
    float previousHeight = spectrumDrawingArea.scaledFrameHeight;

    this->spectrumDrawingArea.scaledFrameWidth = SPECTRUM_AREA_WIDTH * this->spectrumDrawingArea.zoomFactor;
    this->spectrumDrawingArea.scaledFrameHeight = SPECTRUM_AREA_HEIGHT * this->spectrumDrawingArea.zoomFactor;

    //validations for evade a overflow
    if(this->spectrumDrawingArea.scaledFrameWidth >= CAMERA_FRAME_WIDTH)
        this->spectrumDrawingArea.scaledFrameWidth = CAMERA_FRAME_WIDTH;
    if(this->spectrumDrawingArea.scaledFrameHeight >= CAMERA_FRAME_HEIGHT)
        this->spectrumDrawingArea.scaledFrameHeight = CAMERA_FRAME_HEIGHT;



    //Set the x,y coordinates in the camera frame to be the up left corner of the visible image
    spectrumDrawingArea.xUpLeftCorner +=  ((previousWidth - this->spectrumDrawingArea.scaledFrameWidth ) ) / 2.0;
    spectrumDrawingArea.yUpLeftCorner +=  (( previousHeight -this->spectrumDrawingArea.scaledFrameHeight  ))/ 2.0;

    if(spectrumDrawingArea.xUpLeftCorner < 0)
        spectrumDrawingArea.xUpLeftCorner = 0;
    if(spectrumDrawingArea.yUpLeftCorner < 0)
        spectrumDrawingArea.yUpLeftCorner = 0;

    if(this->spectrumDrawingArea.xUpLeftCorner + this->spectrumDrawingArea.scaledFrameWidth > CAMERA_FRAME_WIDTH)
        this->spectrumDrawingArea.xUpLeftCorner = CAMERA_FRAME_WIDTH - this->spectrumDrawingArea.scaledFrameWidth;
    if(this->spectrumDrawingArea.yUpLeftCorner + this->spectrumDrawingArea.scaledFrameHeight > CAMERA_FRAME_HEIGHT)
        this->spectrumDrawingArea.yUpLeftCorner = CAMERA_FRAME_HEIGHT - this->spectrumDrawingArea.scaledFrameHeight;

    return false;
}

void MainWindow::PackWidgets()
{
    principalBox.pack_start(histogramDrwArea, false, false);
    principalBox.pack_start(spectrumDrawingArea, Gtk::PACK_SHRINK);
    //principalBox.pack_start(spectrumDrawingArea, false, false);

}

void MainWindow::ShowWidgets()
{
    spectrumDrawingArea.show();
    histogramDrwArea.show();

    principalBox.show();
}

void MainWindow::capturarImagen()
{
    spectrumDrawingArea.capturar = true;
}

bool MainWindow::on_key_press_event(GdkEventKey* event)
{
    switch (event->keyval)
    {
    // Ctrl + C: Ends the app:
    case GDK_KEY_C:
    case GDK_KEY_c:
        if ((event->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK)
        {
            get_application()->quit();
        }
        return true;

    // [F] toggles fullscreen mode:
    case GDK_KEY_F:
    case GDK_KEY_f:
        if (probablyInFullScreen)
        {
            unfullscreen();
            probablyInFullScreen = false;
        }
        else
        {
            fullscreen();
            probablyInFullScreen = true;
        }
        return true;

    // [esc] exits fullscreen mode:
    case GDK_KEY_Escape:
        unfullscreen();
        probablyInFullScreen = false;
        return true;
    }

    return false;
}

