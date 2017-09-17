/**
 * @file
 * 
 * Program entry point.
 */

#define _BSD_SOURCE

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
extern "C" {
#include <xdo.h>
}
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

static void salir(Fl_Widget*,void*) { std::exit(0); }

int main(int argc, char **argv) {
    xdo_t* xdo = xdo_new(":0");
    Window camera_app_window = 0;
    if (0 != xdo_get_focused_window(xdo, &camera_app_window)) {
        std::cerr << "xdo_get_focused_window failed." << std::endl;
        std::exit(1);
    }
    if (0 != xdo_send_keysequence_window_down(xdo, camera_app_window, "Super_L", 0)) {
        std::cerr << "xdo_send_keysequence_window_down Super_L failed." << std::endl;
        std::exit(1);
    }
    usleep(100000);
    if (0 != xdo_send_keysequence_window_down(xdo, camera_app_window, "Super_R", 0)) {
        std::cerr << "xdo_send_keysequence_window_down Super_R failed." << std::endl;
        std::exit(1);
    }
    usleep(10000000);
    if (0 != xdo_send_keysequence_window_up(xdo, camera_app_window, "Super_R", 0)) {
        std::cerr << "xdo_send_keysequence_window_up Super_R failed." << std::endl;
        std::exit(1);
    }
    usleep(100000);
    if (0 != xdo_send_keysequence_window_up(xdo, camera_app_window, "Super_L", 0)) {
        std::cerr << "xdo_send_keysequence_window_up Super_L failed." << std::endl;
        std::exit(1);
    }
    Fl_Window *window = new Fl_Window(720,480);
    Fl_Button *button = new Fl_Button(10, 390, 700, 80, "SALIR");
    button->callback(salir);
    button->labelsize(60);
    button->box(FL_BORDER_FRAME);
    button->color(FL_RED);
    button->labelcolor(FL_RED);
    window->color(FL_BLACK, FL_RED);
    window->fullscreen();
    window->end();
    window->show(argc, argv);
    return Fl::run();
}
