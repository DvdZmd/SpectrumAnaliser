#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "main-window.hpp"

int main(int argc, char* argv[])
{
    //Se crea un contexto de gtk que es donde correrá toda nuestra applicación
    auto app = Gtk::Application::create(argc, argv, "raspberry-cpp-gtk-opencv");

    MainWindow mainWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);

    return app->run(mainWindow);

    return 0;
}