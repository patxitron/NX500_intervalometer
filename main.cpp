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
#include "shutter.hpp"

static void exit_callback(Fl_Widget*, void*)
{
    std::exit(0);
}

static void start_callback(Fl_Button* button, patxitron::nx::Shutter* shutter)
{
    // TODO:
}

int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(720,480);
    patxitron::nx::Shutter* shutter = new patxitron::nx::Shutter(0, 346);
    Fl_Button *start_button = new Fl_Button(10, 306, 700, 80, "START");
    Fl_Button *exit_button = new Fl_Button(10, 390, 700, 80, "EXIT");
    start_button->callback(reinterpret_cast<Fl_Callback*>(&start_callback), shutter);
    start_button->labelsize(60);
    start_button->box(FL_BORDER_FRAME);
    start_button->color(FL_RED);
    start_button->labelcolor(FL_RED);
    exit_button->callback(&exit_callback);
    exit_button->labelsize(60);
    exit_button->box(FL_BORDER_FRAME);
    exit_button->color(FL_RED);
    exit_button->labelcolor(FL_RED);
    window->color(FL_BLACK, FL_RED);
    window->fullscreen();
    window->end();
    window->show(argc, argv);
    return Fl::run();
}
