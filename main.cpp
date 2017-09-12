/**
 * @file
 * 
 * Program entry point.
 */

#define _BSD_SOURCE

#ifdef DO_FLTK_EXAMPLE

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(300,180);
    Fl_Box *box = new Fl_Box(20,40,260,100,"Hello, World!");
    box->box(FL_UP_BOX);
    box->labelsize(36);
    box->labelfont(FL_BOLD+FL_ITALIC);
    box->labeltype(FL_SHADOW_LABEL);
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
