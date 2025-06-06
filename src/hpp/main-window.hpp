#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include "logger.hpp"
#include "spectrum-drawing-area.hpp"
#include "drwArea_Histogram.hpp"

class MainWindow : public Gtk::Window {
public:
    MainWindow(int width, int height);
    virtual ~MainWindow() = default;

    //Properties
    HistogramDrawingArea histogramDrwArea;
    SpectrumDrawingArea spectrumDrawingArea;

    uint8_t arrayHistograma[HISTOGRAM_WIDTH];


    Logger* logger;
    int machineStateSettingArea = 1;


    //Methods
    void GraficarHistograma(uint8_t*, int);
    void AnalizarFilaDePixelDeFrame(cv::Mat*, int, uint8_t[]);

    void ConnectSignals();
    void ShowWidgets();
    void PackWidgets();

    void SetAnalisisArea();

protected:
    bool on_key_press_event(GdkEventKey* event) override;

private:
    cv::Mat imageToSave;
    void capturarImagen();
    bool clickDown(GdkEventButton*);
    bool scrollMouse(GdkEventScroll*);
    bool clickUp(GdkEventButton*);
    bool mouseMove(GdkEventMotion*);
    bool mouseClick(GdkEventMotion*);

    bool probablyInFullScreen;


    float previousX;
    float previousY;
    float currentX;
    float currentY;

    int settingArea;

    Gtk::VBox principalBox;

    Glib::RefPtr<Gtk::Adjustment> m_adjustment, m_adjustment_digits, m_adjustment_pagesize;
    Glib::RefPtr<Gdk::Pixbuf> histogramaPixBuf;
};

#endif
