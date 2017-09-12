/**
 * @file
 * 
 * Program entry point.
 */

#define _BSD_SOURCE

#ifdef DO_FLTK_EXAMPLE

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
extern "C" {
#include <xdo.h>
}
#include <iostream>
#include <cstdlib>
#include <cstring>

static void salir(Fl_Widget*,void*) { std::exit(0); }

int main(int argc, char **argv) {
    xdo_t* xdo = xdo_new(nullptr);
    xdo_search_t search;
    Window *list = nullptr;
    unsigned int n_windows;
    std::memset(&search, 0, sizeof(xdo_search_t));
    search.winname = "di-camera-app";
    search.searchmask = SEARCH_CLASS;
    search.limit = 1;
    search.require = xdo_search::SEARCH_ANY;
    search.only_visible = 0;
    search.max_depth = 1;
    if (0 != xdo_search_windows(xdo, &search, &list, &n_windows) || n_windows != 1) {
        std::cerr << "xdo_window_search failed." << std::endl;
        std::exit(1);
    }
    std::cout << "Found " << n_windows << " windows with di-camera-app class" << std::endl;
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

#else // !DO_FLTK_EXAMPLE

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "daemonize.hpp"
#include "startwsserver.hpp"
#include "stringvector.hpp"


int main(int argc, char* argv[])
{
    std::string holamundo_args;
    patxitron::stringvector args;
    if (argc > 1) {
        args.insert(args.begin(), argv + 1, argv + argc);
        for (auto const& arg: args) {
            if (holamundo_args.empty()) holamundo_args += ": '";
            else holamundo_args += " '";
            holamundo_args += arg;
            holamundo_args += "'";
        }
    }
    std::cout << "Hello world" << holamundo_args << "\n" << holamundo_args << std::endl;
    patxitron::daemonize_result daemoinized = patxitron::daemonize(
         "IntervalometerWS"
        ,std::bind(&patxitron::startWsServer, args)
    );
    if (daemoinized != patxitron::DAEMON_ERROR) {
        for (size_t i = 10; i > 0; i--) {
            std::cout << "Daemonized, exiting in " << i << " seconds" << std::endl;
            sleep(1);
        }
    } else {
        std::cerr << "Error starting daemon" << std::endl;
        return 1;
    }
    return 0;
}

#endif // DO_FLTK_EXAMPLE
