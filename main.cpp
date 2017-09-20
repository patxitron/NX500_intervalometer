/**
 * @file
 * 
 * Program entry point.
 */

#define _BSD_SOURCE

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <cstdlib>
#include <unistd.h>
#include "intervalometer.hpp"

static void salir(Fl_Widget*,void* p)
{
    reinterpret_cast<patxitron::Intervalometer*>(p)->cancel();
}

static void intercbk(Fl_Widget*,void*)
{
    std::exit(0);
}

int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(720,480);
    Fl_Button *button = new Fl_Button(10, 400, 700, 70, "CANCEL / EXIT");
    patxitron::Intervalometer* interval = new patxitron::Intervalometer;
    interval->callback(&intercbk);
    button->callback(&salir, interval);
    button->labelsize(50);
    button->box(FL_BORDER_FRAME);
    button->color(FL_DARK_RED);
    button->labelcolor(FL_DARK_RED);
    window->color(FL_BLACK, FL_DARK_RED);
    window->end();
    window->show(argc, argv);
    return Fl::run();
}
