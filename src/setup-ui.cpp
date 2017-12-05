#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <cstdlib>
#include <unistd.h>
#include "intervalometer-ui.hpp"
#include "setup-ui.hpp"

namespace patxitron {

namespace {

static void salir(Fl_Widget*,void* p)
{
    reinterpret_cast<patxitron::IntervalometerUi*>(p)->cancel();
}

static void intercbk(Fl_Widget*,void*)
{
    std::exit(0);
}

} // Anonymous



int setupUi(int argc, char **argv)
{
    Fl_Window *window = new Fl_Window(720,480);
    Fl_Button *cancel = new Fl_Button(10, 400, 350, 70, "CANCEL");
    Fl_Button *hide = new Fl_Button(370, 400, 700, 70, "HIDE");
    patxitron::IntervalometerUi* interval = new patxitron::IntervalometerUi;
    interval->callback(&intercbk);
    cancel->callback(&salir, interval);
    cancel->labelsize(50);
    cancel->box(FL_BORDER_FRAME);
    cancel->color(FL_DARK_RED);
    cancel->labelcolor(FL_DARK_RED);
    hide->callback(&salir, interval);
    hide->labelsize(50);
    hide->box(FL_BORDER_FRAME);
    hide->color(FL_DARK_RED);
    hide->labelcolor(FL_DARK_RED);
    window->color(FL_BLACK, FL_DARK_RED);
    window->end();
    window->show(argc, argv);
    return Fl::run();
}

} // namespace patxitron
