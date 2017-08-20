/**
 * @file
 * 
 * Program entry point.
 */

#define _BSD_SOURCE

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "daemonize.hpp"
#include "wsserver.hpp"
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
        ,std::bind(&patxitron::wsServer, args)
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
