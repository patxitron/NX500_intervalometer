#include "wsserver.hpp"
#include <string>
#include <syslog.h>
#include <unistd.h>



namespace patxitron {

void startWsServer(stringvector const& args)
{
    ::std::string holamundo_args;
    for (auto const& arg: args) {
        if (holamundo_args.empty()) holamundo_args += "'";
        else holamundo_args += ", '";
        holamundo_args += arg;
        holamundo_args += "'";
    }
    size_t counter = 0;
    while (true) {
        syslog(LOG_ERR | LOG_DAEMON, "DAEMON: %s <%u>", holamundo_args.c_str(), counter);
        counter += 1;
        sleep(5);
    }
}

} // namespace patxitron
