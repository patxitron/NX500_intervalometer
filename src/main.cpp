/**
 * @file
 * 
 * Program entry point.
 */

#include <functional>
#include <algorithm>
#include <iostream>
#include "setup-ui.hpp"
#include "daemonize.hpp"
#include "daemon.hpp"



int main(int argc, char **argv) {
    patxitron::types::stringvector args;
    if (argc > 1) {
        args.insert(args.begin(), argv + 1, argv + argc);
    }

    auto arg = std::find(args.begin(), args.end(), "-v");
    if (arg != args.end()) {
        std::cout << APP_VERSION << std::endl;
        return 0;
    }
    arg = std::find(args.begin(), args.end(), "-fullversion");
    if (arg != args.end()) {
        std::cout << APP_NAME << " v" << APP_VERSION << "<" << APP_GIT_COMMIT
                  << ">, build " << APP_BUILD_UUID << " on " << APP_BUILD_TIME
                  << std::endl;
        return 0;
    }

    patxitron::util::daemonize_result daemoinized = patxitron::util::daemonize(
         "intervalometer"
        ,std::bind(&patxitron::daemon, args)
    );
    
    arg = std::find(args.begin(), args.end(), "-d");
    if (arg == args.end()) return patxitron::setupUi(argc, argv);
    return daemoinized == patxitron::util::DAEMON_STARTED ? 0 : 1;
}
